//========================================================================
// vc-MemReqMsg : Memory Request Message
//========================================================================
// Memory request messages can either be for a read or write. Read
// requests include an address and the number of bytes to read, while
// write requests include an address, the number of bytes to write, and
// the actual data to write.
//
// Message Format:
//
//    1b     p_addr_sz   calc   p_data_sz
//  +------+-----------+------+-----------+
//  | type | addr      | len  | data      |
//  +------+-----------+------+-----------+
//
// The message type is parameterized by the number of bits in the address
// and the number of bits in the data. Note that the size of the length
// field is caclulated from the number of bits in the data field, and that
// the length field is expressed in _bytes_. If the value of the length
// field is zero, then the read or write should be for the full width of
// the data field.
//
// For example, if the address size is 32 bits and the data size is also
// 32 bits, then the message format is as follows:
//
//   66  66 65       34 33  32 31        0
//  +------+-----------+------+-----------+
//  | type | addr      | len  | data      |
//  +------+-----------+------+-----------+
//
// The length field is two bits. A length value of one means read or write
// a single byte, a length value of two means read or write two bytes, and
// so on. A length value of zero means read or write all four bytes. Note
// that not all memories will necessarily support any alignment and/or any
// value for the length field.

`ifndef VC_MEM_REQ_MSG_V
`define VC_MEM_REQ_MSG_V

//------------------------------------------------------------------------
// Message fields ordered from right to left
//------------------------------------------------------------------------

// Data field

`define VC_MEM_REQ_MSG_DATA_SZ( p_addr_sz, p_data_sz )                  \
  p_data_sz

`define VC_MEM_REQ_MSG_DATA_MSB( p_addr_sz, p_data_sz )                 \
  ( `VC_MEM_REQ_MSG_DATA_SZ( p_addr_sz, p_data_sz ) - 1 )

`define VC_MEM_REQ_MSG_DATA_FIELD( p_addr_sz, p_data_sz )               \
  (`VC_MEM_REQ_MSG_DATA_MSB( p_addr_sz, p_data_sz )):                   \
  0

// Length field

`define VC_MEM_REQ_MSG_LEN_SZ( p_addr_sz, p_data_sz )                   \
  ($clog2(p_data_sz/8))

`define VC_MEM_REQ_MSG_LEN_MSB( p_addr_sz, p_data_sz )                  \
  (   `VC_MEM_REQ_MSG_DATA_MSB( p_addr_sz, p_data_sz )                  \
    + `VC_MEM_REQ_MSG_LEN_SZ( p_addr_sz, p_data_sz ) )

`define VC_MEM_REQ_MSG_LEN_FIELD( p_addr_sz, p_data_sz )                \
  (`VC_MEM_REQ_MSG_LEN_MSB( p_addr_sz, p_data_sz )):                    \
  (`VC_MEM_REQ_MSG_DATA_MSB( p_addr_sz, p_data_sz ) + 1)

// Address field

`define VC_MEM_REQ_MSG_ADDR_SZ( p_addr_sz, p_data_sz )                  \
  p_addr_sz

`define VC_MEM_REQ_MSG_ADDR_MSB( p_addr_sz, p_data_sz )                 \
  (   `VC_MEM_REQ_MSG_LEN_MSB( p_addr_sz, p_data_sz )                   \
    + `VC_MEM_REQ_MSG_ADDR_SZ( p_addr_sz, p_data_sz ) )

`define VC_MEM_REQ_MSG_ADDR_FIELD( p_addr_sz, p_data_sz )               \
  (`VC_MEM_REQ_MSG_ADDR_MSB( p_addr_sz, p_data_sz )):                   \
  (`VC_MEM_REQ_MSG_LEN_MSB( p_addr_sz, p_data_sz ) + 1)

// Type field

`define VC_MEM_REQ_MSG_TYPE_SZ( p_addr_sz, p_data_sz ) 1
`define VC_MEM_REQ_MSG_TYPE_READ                       1'd0
`define VC_MEM_REQ_MSG_TYPE_WRITE                      1'd1

`define VC_MEM_REQ_MSG_TYPE_MSB( p_addr_sz, p_data_sz )                 \
  (   `VC_MEM_REQ_MSG_ADDR_MSB( p_addr_sz, p_data_sz )                  \
    + `VC_MEM_REQ_MSG_TYPE_SZ( p_addr_sz, p_data_sz ) )

`define VC_MEM_REQ_MSG_TYPE_FIELD( p_addr_sz, p_data_sz )               \
  (`VC_MEM_REQ_MSG_TYPE_MSB( p_addr_sz, p_data_sz )):                   \
  (`VC_MEM_REQ_MSG_ADDR_MSB( p_addr_sz, p_data_sz ) + 1)

// Total size of message

`define VC_MEM_REQ_MSG_SZ( p_addr_sz, p_data_sz )                       \
  (   `VC_MEM_REQ_MSG_TYPE_SZ( p_addr_sz, p_data_sz )                   \
    + `VC_MEM_REQ_MSG_ADDR_SZ( p_addr_sz, p_data_sz )                   \
    + `VC_MEM_REQ_MSG_LEN_SZ(  p_addr_sz, p_data_sz )                   \
    + `VC_MEM_REQ_MSG_DATA_SZ( p_addr_sz, p_data_sz ) )

//------------------------------------------------------------------------
// Convert message to bits
//------------------------------------------------------------------------

module vc_MemReqMsgToBits
#(
  parameter p_addr_sz = 32,
  parameter p_data_sz = 32
)(
  // Input message

  input [`VC_MEM_REQ_MSG_TYPE_SZ(p_addr_sz,p_data_sz)-1:0] type,
  input [`VC_MEM_REQ_MSG_ADDR_SZ(p_addr_sz,p_data_sz)-1:0] addr,
  input [`VC_MEM_REQ_MSG_LEN_SZ( p_addr_sz,p_data_sz)-1:0] len,
  input [`VC_MEM_REQ_MSG_DATA_SZ(p_addr_sz,p_data_sz)-1:0] data,

  // Output bits

  output [`VC_MEM_REQ_MSG_SZ(p_addr_sz,p_data_sz)-1:0] bits
);

  assign bits[`VC_MEM_REQ_MSG_TYPE_FIELD(p_addr_sz,p_data_sz)] = type;
  assign bits[`VC_MEM_REQ_MSG_ADDR_FIELD(p_addr_sz,p_data_sz)] = addr;
  assign bits[`VC_MEM_REQ_MSG_LEN_FIELD( p_addr_sz,p_data_sz)] = len;
  assign bits[`VC_MEM_REQ_MSG_DATA_FIELD(p_addr_sz,p_data_sz)] = data;

endmodule

//------------------------------------------------------------------------
// Convert message from bits
//------------------------------------------------------------------------

module vc_MemReqMsgFromBits
#(
  parameter p_addr_sz = 32,
  parameter p_data_sz = 32
)(
  // Input bits

  input [`VC_MEM_REQ_MSG_SZ(p_addr_sz,p_data_sz)-1:0] bits,

  // Output message

  output [`VC_MEM_REQ_MSG_TYPE_SZ(p_addr_sz,p_data_sz)-1:0] type,
  output [`VC_MEM_REQ_MSG_ADDR_SZ(p_addr_sz,p_data_sz)-1:0] addr,
  output [`VC_MEM_REQ_MSG_LEN_SZ( p_addr_sz,p_data_sz)-1:0] len,
  output [`VC_MEM_REQ_MSG_DATA_SZ(p_addr_sz,p_data_sz)-1:0] data
);

  assign type = bits[`VC_MEM_REQ_MSG_TYPE_FIELD(p_addr_sz,p_data_sz)];
  assign addr = bits[`VC_MEM_REQ_MSG_ADDR_FIELD(p_addr_sz,p_data_sz)];
  assign len  = bits[`VC_MEM_REQ_MSG_LEN_FIELD( p_addr_sz,p_data_sz)];
  assign data = bits[`VC_MEM_REQ_MSG_DATA_FIELD(p_addr_sz,p_data_sz)];

endmodule

//------------------------------------------------------------------------
// Convert message to string
//------------------------------------------------------------------------

`ifndef SYNTHESIS
module vc_MemReqMsgToStr
#(
  parameter p_addr_sz = 32,
  parameter p_data_sz = 32
)(
  input [`VC_MEM_REQ_MSG_SZ(p_addr_sz,p_data_sz)-1:0] msg
);

  // Extract fields

  wire [`VC_MEM_REQ_MSG_TYPE_SZ(p_addr_sz,p_data_sz)-1:0] type;
  wire [`VC_MEM_REQ_MSG_ADDR_SZ(p_addr_sz,p_data_sz)-1:0] addr;
  wire [`VC_MEM_REQ_MSG_LEN_SZ( p_addr_sz,p_data_sz)-1:0] len;
  wire [`VC_MEM_REQ_MSG_DATA_SZ(p_addr_sz,p_data_sz)-1:0] data;

  vc_MemReqMsgFromBits#(p_addr_sz,p_data_sz) mem_req_msg_from_bits
  (
    .bits (msg),
    .type (type),
    .addr (addr),
    .len  (len),
    .data (data)
  );

  // Short names

  localparam c_msg_sz = `VC_MEM_REQ_MSG_SZ(p_addr_sz,p_data_sz);
  localparam c_read   = `VC_MEM_REQ_MSG_TYPE_READ;
  localparam c_write  = `VC_MEM_REQ_MSG_TYPE_WRITE;

  // Full string sized for 14 characters

  reg [4*8-1:0]  addr_str;
  reg [1*8-1:0]  len_str;
  reg [4*8-1:0]  data_str;

  reg [14*8-1:0] full_str;
  always @(*) begin

    $sformat( addr_str, "%x", addr );
    $sformat( len_str,  "%x", len  );
    $sformat( data_str, "%x", data );

    if ( msg === {c_msg_sz{1'bx}} )
      $sformat( full_str, "x          ");
    else begin
      case ( type )
        c_read  : $sformat( full_str, "rd:%s:%s     ", addr_str, len_str );
        c_write : $sformat( full_str, "wr:%s:%s:%s", addr_str, len_str, data_str );
        default : $sformat( full_str, "undefined type" );
      endcase
    end

  end

  // Tiny string sized for 2 characters

  reg [2*8-1:0] tiny_str;
  always @(*) begin

    if ( msg === {c_msg_sz{1'bx}} )
      $sformat( tiny_str, "x ");
    else begin
      case ( type )
        c_read  : $sformat( tiny_str, "rd" );
        c_write : $sformat( tiny_str, "wr" );
        default : $sformat( tiny_str, "??" );
      endcase
    end

  end

endmodule
`endif /* SYNTHESIS */

`endif /* VC_MEM_REQ_MSG_V */

