/*
 * SHA module that interfaces with oc8051.
 *
 * This wrapper was written by Pramod Subramanyan.
 * Email: pramod.subramanyan@gmail.com
 */ 

// synopsys translate_off
`include "oc8051_timescale.v"
// synopsys translate_on

module sha_top (
    clk, 
    rst, 
    wr, 
    addr, 
    data_in, 
    data_out, 
    ack, 
    stb, 
    in_addr_range,
    xram_addr,      // SHA ==> XRAM
    xram_data_out,  // SHA ==> XRAM
    xram_data_in,   // XRAM ==> SHA
    xram_ack,       // XRAM ==> SHA
    xram_stb,       // SHA ==> XRAM
    xram_wr
);

//
// This is the exact same interface as oc8051_xram.
//
// clk          (in)  clock
// addr         (in)  addres
// data_in      (out) data input
// data_out     (in)  data output
// wr           (in)  write
// ack          (out) acknowlage
// stb          (in)  strobe
//

// 8051 <=> SHA
input clk, wr, stb, rst;
input [7:0] data_in;
input [15:0] addr;
output [7:0] data_out;
output ack;
output in_addr_range;
// SHA <=> XRAM
output [15:0] xram_addr;
output [7:0] xram_data_out;
input [7:0] xram_data_in;
input xram_ack;
output xram_stb;
output xram_wr;
// verif output
wire [2:0] sha_state;
wire [15:0] sha_rdaddr, sha_wraddr, sha_len;
wire sha_step;
wire sha_core_assumps_valid;

// FIRST ADDRESS ALLOCATED TO THIS UNIT.
localparam SHA_ADDR_START  = 16'hfe00;
// see also SHA_ADDR_END.

// The addresses of the registers.
localparam SHA_REG_START   = 16'hfe00; // 1 byte.
localparam SHA_REG_STATE   = 16'hfe01; // 1 byte.
localparam SHA_REG_RD_ADDR = 16'hfe02; // 2 bytes
localparam SHA_REG_WR_ADDR = 16'hfe04; // 2 bytes
localparam SHA_REG_LEN     = 16'hfe06; // 2 bytes.

// END OF THE ADDRESS RANGE ALLOCATED TO THIS UNIT.
localparam SHA_ADDR_END    = 16'hfe10;
// see also SHA_ADDR_START.

// response to the xiommu module.
wire in_addr_range = addr >= SHA_ADDR_START && addr < SHA_ADDR_END;
wire ack = stb && in_addr_range;

// state register.
reg [2:0]  sha_reg_state;
wire [2:0] sha_state = sha_reg_state;

// state predicates.
wire sha_state_idle       = sha_reg_state == SHA_STATE_IDLE;
wire sha_state_read_data  = sha_reg_state == SHA_STATE_READ_DATA;
wire sha_state_op1        = sha_reg_state == SHA_STATE_OP1;
wire sha_state_op2        = sha_reg_state == SHA_STATE_OP2;
wire sha_state_write_data = sha_reg_state == SHA_STATE_WRITE_DATA;

// register selector predicates.
wire sel_reg_start = addr == SHA_REG_START;
wire sel_reg_state = addr == SHA_REG_STATE;
wire sel_reg_rd_addr  = {addr[15:1], 1'b0} == SHA_REG_RD_ADDR;
wire sel_reg_wr_addr  = {addr[15:1], 1'b0} == SHA_REG_WR_ADDR;
wire sel_reg_len      = {addr[15:1], 1'b0} == SHA_REG_LEN;
wire wren = wr && sha_state_idle;

wire start_op         = sel_reg_start && data_in[0] && stb && wren;

// The current state of the AES module.
localparam SHA_STATE_IDLE       = 3'd0;
localparam SHA_STATE_READ_DATA  = 3'd1;
localparam SHA_STATE_OP1        = 3'd2;
localparam SHA_STATE_OP2        = 3'd3;
localparam SHA_STATE_WRITE_DATA = 3'd4;

// BEGIN: next state computation
wire [2:0] sha_state_next;

wire [2:0] sha_state_next_idle;
wire [2:0] sha_state_next_read_data;
wire [2:0] sha_state_next_op1;
wire [2:0] sha_state_next_op2;
wire [2:0] sha_state_next_write_data;

assign sha_state_next = 
    sha_state_idle       ? sha_state_next_idle       : 
    sha_state_read_data  ? sha_state_next_read_data  : 
    sha_state_op1        ? sha_state_next_op1        : 
    sha_state_op2        ? sha_state_next_op2        : 
    sha_state_write_data ? sha_state_next_write_data : 3'b0;

wire sha_step = sha_reg_state != sha_state_next;

// Go to the read data state if we get a start signal.  
assign sha_state_next_idle = start_op ? SHA_STATE_READ_DATA : SHA_STATE_IDLE; 
// We will continue to be in the read data state until all the data is read.
assign sha_state_next_read_data = read_last_byte_acked ? SHA_STATE_OP1 : SHA_STATE_READ_DATA;
// We always go from the op1 to the op2 state in a cycle.
assign sha_state_next_op1 = SHA_STATE_OP2;
// We will continue to be in the op2 state until we received a digest_ready signal from the SHA core.
assign sha_state_next_op2 = sha_finished    ? SHA_STATE_WRITE_DATA :
                            sha_more_blocks ? SHA_STATE_READ_DATA  : SHA_STATE_OP2;
// We will leave the write data state when we are finished writing into the XRAM.
assign sha_state_next_write_data = write_last_byte_acked ? SHA_STATE_IDLE : SHA_STATE_WRITE_DATA;

// Keeping track of the current byte.
reg [5:0] byte_counter;
wire [5:0] byte_counter_next;
wire [5:0] byte_counter_next_rw;
assign byte_counter_next_rw = xram_ack ? byte_counter + 1 : byte_counter;
assign byte_counter_next = 
    sha_state_idle || sha_state_op1 || sha_state_op2 ? 6'b0                 : 
    sha_state_read_data                              ? byte_counter_next_rw : 
    sha_state_write_data                             ? byte_counter_next_rw : byte_counter;

// Keeping track of the number of bytes processed.
reg [15:0] reg_bytes_read;
wire [15:0] bytes_read_next;
assign bytes_read_next = 
    sha_state_idle                  ? 16'b0              : 
    sha_state_read_data && xram_ack ? reg_bytes_read + 1 : reg_bytes_read;

// Keeping track of the current block.
reg [15:0] block_counter;
wire [15:0] block_counter_next;
assign block_counter_next = 
    sha_state_idle                   ? 16'b0              : 
    sha_state_op2 && sha_more_blocks ? block_counter + 64 : block_counter;

// Are we reading the last byte?
wire reading_last_byte = (byte_counter == 6'd63) || (bytes_read_next == sha_reg_len);
wire read_last_byte_acked = reading_last_byte && xram_ack;

// Are we done reading or do we have more blocks?
wire sha_more_blocks = sha_core_digest_valid && (reg_bytes_read <  sha_reg_len);
wire sha_finished    = sha_core_digest_valid && (reg_bytes_read >= sha_reg_len);

// SHA operation logic.
wire [511:0] sha_core_block_read_data_next;

assign sha_core_block_read_data_next[7   : 0   ] = byte_counter == 63 ? xram_data_in : sha_core_block[7   : 0   ];
assign sha_core_block_read_data_next[15  : 8   ] = byte_counter == 62 ? xram_data_in : sha_core_block[15  : 8   ];
assign sha_core_block_read_data_next[23  : 16  ] = byte_counter == 61 ? xram_data_in : sha_core_block[23  : 16  ];
assign sha_core_block_read_data_next[31  : 24  ] = byte_counter == 60 ? xram_data_in : sha_core_block[31  : 24  ];
assign sha_core_block_read_data_next[39  : 32  ] = byte_counter == 59 ? xram_data_in : sha_core_block[39  : 32  ];
assign sha_core_block_read_data_next[47  : 40  ] = byte_counter == 58 ? xram_data_in : sha_core_block[47  : 40  ];
assign sha_core_block_read_data_next[55  : 48  ] = byte_counter == 57 ? xram_data_in : sha_core_block[55  : 48  ];
assign sha_core_block_read_data_next[63  : 56  ] = byte_counter == 56 ? xram_data_in : sha_core_block[63  : 56  ];
assign sha_core_block_read_data_next[71  : 64  ] = byte_counter == 55 ? xram_data_in : sha_core_block[71  : 64  ];
assign sha_core_block_read_data_next[79  : 72  ] = byte_counter == 54 ? xram_data_in : sha_core_block[79  : 72  ];
assign sha_core_block_read_data_next[87  : 80  ] = byte_counter == 53 ? xram_data_in : sha_core_block[87  : 80  ];
assign sha_core_block_read_data_next[95  : 88  ] = byte_counter == 52 ? xram_data_in : sha_core_block[95  : 88  ];
assign sha_core_block_read_data_next[103 : 96  ] = byte_counter == 51 ? xram_data_in : sha_core_block[103 : 96  ];
assign sha_core_block_read_data_next[111 : 104 ] = byte_counter == 50 ? xram_data_in : sha_core_block[111 : 104 ];
assign sha_core_block_read_data_next[119 : 112 ] = byte_counter == 49 ? xram_data_in : sha_core_block[119 : 112 ];
assign sha_core_block_read_data_next[127 : 120 ] = byte_counter == 48 ? xram_data_in : sha_core_block[127 : 120 ];
assign sha_core_block_read_data_next[135 : 128 ] = byte_counter == 47 ? xram_data_in : sha_core_block[135 : 128 ];
assign sha_core_block_read_data_next[143 : 136 ] = byte_counter == 46 ? xram_data_in : sha_core_block[143 : 136 ];
assign sha_core_block_read_data_next[151 : 144 ] = byte_counter == 45 ? xram_data_in : sha_core_block[151 : 144 ];
assign sha_core_block_read_data_next[159 : 152 ] = byte_counter == 44 ? xram_data_in : sha_core_block[159 : 152 ];
assign sha_core_block_read_data_next[167 : 160 ] = byte_counter == 43 ? xram_data_in : sha_core_block[167 : 160 ];
assign sha_core_block_read_data_next[175 : 168 ] = byte_counter == 42 ? xram_data_in : sha_core_block[175 : 168 ];
assign sha_core_block_read_data_next[183 : 176 ] = byte_counter == 41 ? xram_data_in : sha_core_block[183 : 176 ];
assign sha_core_block_read_data_next[191 : 184 ] = byte_counter == 40 ? xram_data_in : sha_core_block[191 : 184 ];
assign sha_core_block_read_data_next[199 : 192 ] = byte_counter == 39 ? xram_data_in : sha_core_block[199 : 192 ];
assign sha_core_block_read_data_next[207 : 200 ] = byte_counter == 38 ? xram_data_in : sha_core_block[207 : 200 ];
assign sha_core_block_read_data_next[215 : 208 ] = byte_counter == 37 ? xram_data_in : sha_core_block[215 : 208 ];
assign sha_core_block_read_data_next[223 : 216 ] = byte_counter == 36 ? xram_data_in : sha_core_block[223 : 216 ];
assign sha_core_block_read_data_next[231 : 224 ] = byte_counter == 35 ? xram_data_in : sha_core_block[231 : 224 ];
assign sha_core_block_read_data_next[239 : 232 ] = byte_counter == 34 ? xram_data_in : sha_core_block[239 : 232 ];
assign sha_core_block_read_data_next[247 : 240 ] = byte_counter == 33 ? xram_data_in : sha_core_block[247 : 240 ];
assign sha_core_block_read_data_next[255 : 248 ] = byte_counter == 32 ? xram_data_in : sha_core_block[255 : 248 ];
assign sha_core_block_read_data_next[263 : 256 ] = byte_counter == 31 ? xram_data_in : sha_core_block[263 : 256 ];
assign sha_core_block_read_data_next[271 : 264 ] = byte_counter == 30 ? xram_data_in : sha_core_block[271 : 264 ];
assign sha_core_block_read_data_next[279 : 272 ] = byte_counter == 29 ? xram_data_in : sha_core_block[279 : 272 ];
assign sha_core_block_read_data_next[287 : 280 ] = byte_counter == 28 ? xram_data_in : sha_core_block[287 : 280 ];
assign sha_core_block_read_data_next[295 : 288 ] = byte_counter == 27 ? xram_data_in : sha_core_block[295 : 288 ];
assign sha_core_block_read_data_next[303 : 296 ] = byte_counter == 26 ? xram_data_in : sha_core_block[303 : 296 ];
assign sha_core_block_read_data_next[311 : 304 ] = byte_counter == 25 ? xram_data_in : sha_core_block[311 : 304 ];
assign sha_core_block_read_data_next[319 : 312 ] = byte_counter == 24 ? xram_data_in : sha_core_block[319 : 312 ];
assign sha_core_block_read_data_next[327 : 320 ] = byte_counter == 23 ? xram_data_in : sha_core_block[327 : 320 ];
assign sha_core_block_read_data_next[335 : 328 ] = byte_counter == 22 ? xram_data_in : sha_core_block[335 : 328 ];
assign sha_core_block_read_data_next[343 : 336 ] = byte_counter == 21 ? xram_data_in : sha_core_block[343 : 336 ];
assign sha_core_block_read_data_next[351 : 344 ] = byte_counter == 20 ? xram_data_in : sha_core_block[351 : 344 ];
assign sha_core_block_read_data_next[359 : 352 ] = byte_counter == 19 ? xram_data_in : sha_core_block[359 : 352 ];
assign sha_core_block_read_data_next[367 : 360 ] = byte_counter == 18 ? xram_data_in : sha_core_block[367 : 360 ];
assign sha_core_block_read_data_next[375 : 368 ] = byte_counter == 17 ? xram_data_in : sha_core_block[375 : 368 ];
assign sha_core_block_read_data_next[383 : 376 ] = byte_counter == 16 ? xram_data_in : sha_core_block[383 : 376 ];
assign sha_core_block_read_data_next[391 : 384 ] = byte_counter == 15 ? xram_data_in : sha_core_block[391 : 384 ];
assign sha_core_block_read_data_next[399 : 392 ] = byte_counter == 14 ? xram_data_in : sha_core_block[399 : 392 ];
assign sha_core_block_read_data_next[407 : 400 ] = byte_counter == 13 ? xram_data_in : sha_core_block[407 : 400 ];
assign sha_core_block_read_data_next[415 : 408 ] = byte_counter == 12 ? xram_data_in : sha_core_block[415 : 408 ];
assign sha_core_block_read_data_next[423 : 416 ] = byte_counter == 11 ? xram_data_in : sha_core_block[423 : 416 ];
assign sha_core_block_read_data_next[431 : 424 ] = byte_counter == 10 ? xram_data_in : sha_core_block[431 : 424 ];
assign sha_core_block_read_data_next[439 : 432 ] = byte_counter == 9  ? xram_data_in : sha_core_block[439 : 432 ];
assign sha_core_block_read_data_next[447 : 440 ] = byte_counter == 8  ? xram_data_in : sha_core_block[447 : 440 ];
assign sha_core_block_read_data_next[455 : 448 ] = byte_counter == 7  ? xram_data_in : sha_core_block[455 : 448 ];
assign sha_core_block_read_data_next[463 : 456 ] = byte_counter == 6  ? xram_data_in : sha_core_block[463 : 456 ];
assign sha_core_block_read_data_next[471 : 464 ] = byte_counter == 5  ? xram_data_in : sha_core_block[471 : 464 ];
assign sha_core_block_read_data_next[479 : 472 ] = byte_counter == 4  ? xram_data_in : sha_core_block[479 : 472 ];
assign sha_core_block_read_data_next[487 : 480 ] = byte_counter == 3  ? xram_data_in : sha_core_block[487 : 480 ];
assign sha_core_block_read_data_next[495 : 488 ] = byte_counter == 2  ? xram_data_in : sha_core_block[495 : 488 ];
assign sha_core_block_read_data_next[503 : 496 ] = byte_counter == 1  ? xram_data_in : sha_core_block[503 : 496 ];
assign sha_core_block_read_data_next[511 : 504 ] = byte_counter == 0  ? xram_data_in : sha_core_block[511 : 504 ];

wire [511:0] sha_core_block_next = sha_state_idle       ? 512'b0                        :
                                   sha_state_read_data  ? sha_core_block_read_data_next : sha_core_block;


// Writing logic.
wire writing_last_byte = byte_counter == 6'd19;
wire write_last_byte_acked = writing_last_byte && xram_ack;

wire [7:0] data_out_state,
           data_out_rd_addr,
           data_out_wr_addr,
           data_out_len;

// allow the processor to read the current state.
assign data_out_state = {5'd0, sha_reg_state};

wire [7:0] data_out;
assign data_out = sel_reg_state     ? data_out_state
                : sel_reg_rd_addr   ? data_out_rd_addr
                : sel_reg_wr_addr   ? data_out_wr_addr
                : sel_reg_len       ? data_out_len
                : 8'd0;

// rd address register.
wire [15:0] sha_reg_rd_addr;
reg2byte sha_reg_rd_addr_i(
    .clk        (clk),
    .rst        (rst),
    .en         (sel_reg_rd_addr),
    .wr         (sel_reg_rd_addr && wren),
    .addr       (addr[0]),
    .data_in    (data_in),
    .data_out   (data_out_rd_addr),
    .reg_out    (sha_reg_rd_addr)
);

// wr address register.
wire [15:0] sha_reg_wr_addr;
reg2byte sha_reg_wr_addr_i(
    .clk        (clk),
    .rst        (rst),
    .en         (sel_reg_wr_addr),
    .wr         (sel_reg_wr_addr && wren),
    .addr       (addr[0]),
    .data_in    (data_in),
    .data_out   (data_out_wr_addr),
    .reg_out    (sha_reg_wr_addr)
);

// length register.
wire [15:0] sha_reg_len;
reg2byte sha_reg_len_i(
    .clk        (clk),
    .rst        (rst),
    .en         (sel_reg_len),
    .wr         (sel_reg_len && wren),
    .addr       (addr[0]),
    .data_in    (data_in),
    .data_out   (data_out_len),
    .reg_out    (sha_reg_len)
);

wire [15:0] sha_rdaddr = sha_reg_rd_addr;
wire [15:0] sha_wraddr = sha_reg_wr_addr;
wire [15:0] sha_len = sha_reg_len;

// Active low reset.
wire sha_core_rst_n = !rst; 
// Set to one to start hashing the first block of data.
wire sha_core_init = sha_state_op1 && sha_core_ready_r && (block_counter == 0);
// Set to one to start hashing the next blocks of data.
wire sha_core_next = sha_state_op1 && sha_core_ready_r && (block_counter != 0);
// Output from core signalling that it is ready for init/next to be set to 1.
wire sha_core_ready;
// flopped version.
reg sha_core_ready_r;
// Output from core signalling that the hash result is ready.
wire sha_core_digest_valid;
// Register which holds the data to be hashed.
reg [511:0] sha_core_block;
// Hash output.
wire [159:0] sha_core_digest;
// Hash output flops.
reg [159:0] sha_reg_digest;
// The MUX which selects the input to the flops.
wire [159:0] sha_reg_digest_next;
assign sha_reg_digest_next = sha_core_digest_valid ? sha_core_digest : sha_reg_digest;
    
reg sha_core_assumps_valid_reg;
wire sha_core_assumps_valid_reg_next = 
    sha_core_assumps_valid_reg ? !(sha_core_ready_r != sha_core_digest_valid) : 0;

wire sha_core_assumps_valid = 1; // sha_core_assumps_valid_reg && sha_core_assumps_valid_reg_next;

always @(posedge clk)
begin
    if (rst) begin
        sha_core_assumps_valid_reg <= 1;
    end
    else begin
        sha_core_assumps_valid_reg <= sha_core_assumps_valid_reg_next;
    end
end

// instantiate the SHA1 core.
sha1_core sha1_core_i (
    .clk          ( clk                   ),
    .reset_n      ( sha_core_rst_n        ),
    .init         ( sha_core_init         ),
    .next         ( sha_core_next         ),
    .ready        ( sha_core_ready        ),
    .digest_valid ( sha_core_digest_valid ),
    .block        ( sha_core_block        ),
    .digest       ( sha_core_digest       ) 
);

// XRAM interface.
assign xram_addr = sha_state_read_data  ? sha_reg_rd_addr + {10'b0, byte_counter} + block_counter :
                   sha_state_write_data ? sha_reg_wr_addr + {10'b0, byte_counter}                 : 16'b0;
assign xram_data_out = 
    byte_counter == 19 ? sha_reg_digest[7   : 0]   : 
    byte_counter == 18 ? sha_reg_digest[15  : 8]   : 
    byte_counter == 17 ? sha_reg_digest[23  : 16]  : 
    byte_counter == 16 ? sha_reg_digest[31  : 24]  : 
    byte_counter == 15 ? sha_reg_digest[39  : 32]  : 
    byte_counter == 14 ? sha_reg_digest[47  : 40]  : 
    byte_counter == 13 ? sha_reg_digest[55  : 48]  : 
    byte_counter == 12 ? sha_reg_digest[63  : 56]  : 
    byte_counter == 11 ? sha_reg_digest[71  : 64]  : 
    byte_counter == 10 ? sha_reg_digest[79  : 72]  : 
    byte_counter == 9  ? sha_reg_digest[87  : 80]  : 
    byte_counter == 8  ? sha_reg_digest[95  : 88]  : 
    byte_counter == 7  ? sha_reg_digest[103 : 96]  : 
    byte_counter == 6  ? sha_reg_digest[111 : 104] : 
    byte_counter == 5  ? sha_reg_digest[119 : 112] : 
    byte_counter == 4  ? sha_reg_digest[127 : 120] : 
    byte_counter == 3  ? sha_reg_digest[135 : 128] : 
    byte_counter == 2  ? sha_reg_digest[143 : 136] : 
    byte_counter == 1  ? sha_reg_digest[151 : 144] : 
    byte_counter == 0  ? sha_reg_digest[159 : 152] : 8'b0;

assign xram_stb = sha_state_read_data || sha_state_write_data;
assign xram_wr = sha_state_write_data;

// Registers.
always @(posedge clk)
begin
    if (rst) begin
        sha_reg_state       <= SHA_STATE_IDLE;
        byte_counter        <= 0;
        sha_core_block      <= 0;
        reg_bytes_read      <= 0;
        block_counter       <= 0;
        sha_reg_digest      <= 0;
        sha_core_ready_r    <= 0;
    end
    else begin
        sha_reg_state       <= sha_state_next;
        byte_counter        <= byte_counter_next;
        sha_core_block      <= sha_core_block_next;
        reg_bytes_read      <= bytes_read_next;
        block_counter       <= block_counter_next;
        sha_reg_digest      <= sha_reg_digest_next;
        sha_core_ready_r    <= sha_core_ready;
    end
end
endmodule
