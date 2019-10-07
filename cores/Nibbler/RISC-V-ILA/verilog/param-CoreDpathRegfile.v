//=========================================================================
// 5-Stage PARC Register File
//=========================================================================



module param_CoreDpathRegfile
(
  input            clk,
  input  [ 4:0]    raddr0_Rhl, // Read 0 address (combinational input)
  input  [2:0]     roff0_Rhl,  // Offset within a register to read from
  output reg [3:0] rdata0_Rhl, // Read 0 data (combinational on raddr)

  
  input  [ 4:0]      raddr1_Rhl, // Read 1 address (combinational input)
  input  [2:0]       roff1_Rhl,  // Offset within a register to read from
  output reg [3:0]   rdata1_Rhl, // Read 0 data (combinational on raddr)

  input          wen_Xhl,    // Write enable (sample on rising clk edge)
  input  [ 4:0]  waddr_Xhl,  // Write address (sample on rising clk edge)
  input  [2:0]   woffset_Xhl,  // Offset within a register to write to 
  input  [3:0]   wdata_Xhl    // Write data (sample on rising clk edge)

);

  // We use an array of 32 bit register for the regfile itself
  reg [31:0] registers[31:0];

  // Combinational read ports

  wire [31:0] rdata0_word = ( raddr0_Rhl == 0 ) ? 32'b0 : registers[raddr0_Rhl];
  wire [31:0] rdata1_word = ( raddr1_Rhl == 0 ) ? 32'b0 : registers[raddr1_Rhl];

  always @(*) begin
    case(roff0_Rhl)
      3'd0: rdata0_Rhl = rdata0_word[3:0];
      3'd1: rdata0_Rhl = rdata0_word[7:4];
      3'd2: rdata0_Rhl = rdata0_word[11:8];
      3'd3: rdata0_Rhl = rdata0_word[15:12];
      3'd4: rdata0_Rhl = rdata0_word[19:16];
      3'd5: rdata0_Rhl = rdata0_word[23:20];
      3'd6: rdata0_Rhl = rdata0_word[27:24];
      3'd7: rdata0_Rhl = rdata0_word[31:28];
      default: rdata0_Rhl = 3'd0;
    endcase
  end

  always @(*) begin
    case(roff1_Rhl)
      3'd0: rdata1_Rhl = rdata1_word[3:0];
      3'd1: rdata1_Rhl = rdata1_word[7:4];
      3'd2: rdata1_Rhl = rdata1_word[11:8];
      3'd3: rdata1_Rhl = rdata1_word[15:12];
      3'd4: rdata1_Rhl = rdata1_word[19:16];
      3'd5: rdata1_Rhl = rdata1_word[23:20];
      3'd6: rdata1_Rhl = rdata1_word[27:24];
      3'd7: rdata1_Rhl = rdata1_word[31:28];
      default: rdata1_Rhl = 3'd0;
    endcase
  end

  
  // Write port is active only when wen is asserted

  //Register 0:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd0) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[0][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd0) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[0][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd0) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[0][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd0) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[0][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd0) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[0][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd0) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[0][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd0) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[0][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd0) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[0][31:28] <= wdata_Xhl;
    end
  end

  //Register 1:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd1) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[1][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd1) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[1][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd1) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[1][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd1) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[1][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd1) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[1][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd1) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[1][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd1) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[1][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd1) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[1][31:28] <= wdata_Xhl;
    end
  end

  //Register 2:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd2) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[2][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd2) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[2][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd2) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[2][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd2) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[2][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd2) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[2][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd2) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[2][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd2) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[2][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd2) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[2][31:28] <= wdata_Xhl;
    end
  end

  //Register 3:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd3) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[3][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd3) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[3][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd3) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[3][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd3) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[3][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd3) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[3][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd3) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[3][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd3) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[3][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd3) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[3][31:28] <= wdata_Xhl;
    end
  end

  //Register 4:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd4) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[4][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd4) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[4][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd4) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[4][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd4) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[4][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd4) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[4][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd4) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[4][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd4) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[4][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd4) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[4][31:28] <= wdata_Xhl;
    end
  end

  //Register 5:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd5) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[5][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd5) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[5][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd5) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[5][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd5) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[5][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd5) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[5][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd5) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[5][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd5) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[5][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd5) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[5][31:28] <= wdata_Xhl;
    end
  end

  //Register 6:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd6) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[6][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd6) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[6][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd6) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[6][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd6) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[6][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd6) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[6][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd6) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[6][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd6) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[6][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd6) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[6][31:28] <= wdata_Xhl;
    end
  end

  //Register 7:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd7) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[7][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd7) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[7][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd7) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[7][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd7) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[7][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd7) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[7][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd7) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[7][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd7) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[7][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd7) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[7][31:28] <= wdata_Xhl;
    end
  end

  //Register 8:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd8) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[8][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd8) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[8][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd8) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[8][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd8) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[8][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd8) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[8][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd8) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[8][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd8) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[8][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd8) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[8][31:28] <= wdata_Xhl;
    end
  end

  //Register 9:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd9) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[9][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd9) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[9][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd9) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[9][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd9) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[9][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd9) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[9][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd9) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[9][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd9) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[9][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd9) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[9][31:28] <= wdata_Xhl;
    end
  end

  //Register 10:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd10) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[10][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd10) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[10][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd10) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[10][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd10) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[10][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd10) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[10][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd10) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[10][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd10) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[10][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd10) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[10][31:28] <= wdata_Xhl;
    end
  end

  //Register 11:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd11) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[11][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd11) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[11][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd11) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[11][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd11) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[11][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd11) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[11][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd11) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[11][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd11) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[11][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd11) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[11][31:28] <= wdata_Xhl;
    end
  end

  //Register 12:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd12) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[12][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd12) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[12][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd12) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[12][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd12) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[12][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd12) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[12][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd12) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[12][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd12) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[12][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd12) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[12][31:28] <= wdata_Xhl;
    end
  end

  //Register 13:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd13) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[13][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd13) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[13][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd13) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[13][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd13) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[13][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd13) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[13][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd13) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[13][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd13) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[13][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd13) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[13][31:28] <= wdata_Xhl;
    end
  end

  //Register 14:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd14) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[14][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd14) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[14][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd14) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[14][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd14) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[14][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd14) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[14][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd14) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[14][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd14) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[14][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd14) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[14][31:28] <= wdata_Xhl;
    end
  end

  //Register 15:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd15) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[15][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd15) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[15][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd15) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[15][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd15) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[15][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd15) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[15][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd15) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[15][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd15) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[15][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd15) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[15][31:28] <= wdata_Xhl;
    end
  end

  //Register 16:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd16) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[16][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd16) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[16][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd16) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[16][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd16) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[16][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd16) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[16][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd16) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[16][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd16) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[16][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd16) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[16][31:28] <= wdata_Xhl;
    end
  end

  //Register 17:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd17) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[17][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd17) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[17][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd17) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[17][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd17) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[17][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd17) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[17][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd17) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[17][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd17) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[17][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd17) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[17][31:28] <= wdata_Xhl;
    end
  end

  //Register 18:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd18) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[18][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd18) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[18][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd18) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[18][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd18) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[18][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd18) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[18][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd18) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[18][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd18) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[18][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd18) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[18][31:28] <= wdata_Xhl;
    end
  end

  //Register 19:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd19) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[19][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd19) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[19][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd19) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[19][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd19) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[19][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd19) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[19][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd19) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[19][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd19) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[19][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd19) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[19][31:28] <= wdata_Xhl;
    end
  end

  //Register 20:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd20) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[20][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd20) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[20][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd20) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[20][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd20) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[20][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd20) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[20][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd20) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[20][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd20) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[20][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd20) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[20][31:28] <= wdata_Xhl;
    end
  end

  //Register 21:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd21) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[21][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd21) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[21][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd21) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[21][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd21) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[21][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd21) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[21][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd21) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[21][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd21) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[21][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd21) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[21][31:28] <= wdata_Xhl;
    end
  end

  //Register 22:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd22) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[22][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd22) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[22][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd22) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[22][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd22) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[22][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd22) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[22][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd22) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[22][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd22) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[22][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd22) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[22][31:28] <= wdata_Xhl;
    end
  end

  //Register 23:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd23) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[23][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd23) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[23][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd23) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[23][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd23) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[23][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd23) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[23][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd23) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[23][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd23) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[23][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd23) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[23][31:28] <= wdata_Xhl;
    end
  end

  //Register 24:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd24) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[24][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd24) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[24][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd24) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[24][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd24) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[24][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd24) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[24][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd24) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[24][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd24) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[24][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd24) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[24][31:28] <= wdata_Xhl;
    end
  end

  //Register 25:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd25) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[25][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd25) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[25][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd25) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[25][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd25) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[25][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd25) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[25][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd25) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[25][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd25) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[25][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd25) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[25][31:28] <= wdata_Xhl;
    end
  end

  //Register 26:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd26) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[26][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd26) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[26][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd26) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[26][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd26) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[26][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd26) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[26][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd26) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[26][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd26) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[26][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd26) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[26][31:28] <= wdata_Xhl;
    end
  end

  //Register 27:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd27) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[27][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd27) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[27][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd27) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[27][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd27) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[27][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd27) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[27][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd27) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[27][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd27) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[27][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd27) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[27][31:28] <= wdata_Xhl;
    end
  end

  //Register 28:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd28) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[28][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd28) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[28][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd28) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[28][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd28) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[28][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd28) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[28][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd28) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[28][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd28) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[28][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd28) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[28][31:28] <= wdata_Xhl;
    end
  end

  //Register 29:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd29) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[29][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd29) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[29][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd29) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[29][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd29) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[29][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd29) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[29][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd29) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[29][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd29) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[29][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd29) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[29][31:28] <= wdata_Xhl;
    end
  end

  //Register 30:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd30) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[30][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd30) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[30][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd30) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[30][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd30) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[30][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd30) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[30][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd30) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[30][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd30) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[30][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd30) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[30][31:28] <= wdata_Xhl;
    end
  end

  //Register 31:
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd31) && (woffset_Xhl == 3'd0) && (wen_Xhl == 1'b1)) begin
      registers[31][3:0] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd31) && (woffset_Xhl == 3'd1) && (wen_Xhl == 1'b1)) begin
      registers[31][7:4] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd31) && (woffset_Xhl == 3'd2) && (wen_Xhl == 1'b1)) begin
      registers[31][11:8] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd31) && (woffset_Xhl == 3'd3) && (wen_Xhl == 1'b1)) begin
      registers[31][15:12] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd31) && (woffset_Xhl == 3'd4) && (wen_Xhl == 1'b1)) begin
      registers[31][19:16] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd31) && (woffset_Xhl == 3'd5) && (wen_Xhl == 1'b1)) begin
      registers[31][23:20] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd31) && (woffset_Xhl == 3'd6) && (wen_Xhl == 1'b1)) begin
      registers[31][27:24] <= wdata_Xhl;
    end
  end
  always @(posedge clk) begin
    if ((waddr_Xhl == 5'd31) && (woffset_Xhl == 3'd7) && (wen_Xhl == 1'b1)) begin
      registers[31][31:28] <= wdata_Xhl;
    end
  end


endmodule
