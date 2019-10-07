

// `include "param-ClkEnBuf.v"



module param_DeserializedSeqLog
(
  input clk,
  input subword_0_en,
  input subword_1_en,
  input subword_2_en,
  input subword_3_en,
  input subword_4_en,
  input subword_5_en,
  input subword_6_en,
  input subword_7_en,
  input [3:0] write_data,
  output reg [31:0] out

);

  reg [3:0] reg_array[7:0];
  // Subword 0 Sequential
  always @ (posedge clk) begin
    reg_array[0] <= subword_0_en ? write_data : reg_array[0];
  end

  // Subword 1 Sequential
  always @ (posedge clk) begin
    reg_array[1] <= subword_1_en ? write_data : reg_array[1];
  end

  // Subword 2 Sequential
  always @ (posedge clk) begin
    reg_array[2] <= subword_2_en ? write_data : reg_array[2];
  end

  // Subword 3 Sequential
  always @ (posedge clk) begin
    reg_array[3] <= subword_3_en ? write_data : reg_array[3];
  end

  // Subword 4 Sequential
  always @ (posedge clk) begin
    reg_array[4] <= subword_4_en ? write_data : reg_array[4];
  end

  // Subword 5 Sequential
  always @ (posedge clk) begin
    reg_array[5] <= subword_5_en ? write_data : reg_array[5];
  end

  // Subword 6 Sequential
  always @ (posedge clk) begin
    reg_array[6] <= subword_6_en ? write_data : reg_array[6];
  end

  // Subword 7 Sequential
  always @ (posedge clk) begin
    reg_array[7] <= subword_7_en ? write_data : reg_array[7];
  end



  // Concatenate register array for convenience later
  always @(*) begin
    out = {
    reg_array[7],
    reg_array[6],
    reg_array[5],
    reg_array[4],
    reg_array[3],
    reg_array[2],
    reg_array[1],
    reg_array[0]
    };
  end

endmodule

module param_DeserializedReg
(
  input reset,
  input clk,

  input [2:0] subword_en_idx,
  input [3:0] data_in,

  output reg [31:0] out
);

  wire [3:0] write_data = reset ? 4'b0 : data_in;


// Convert subword enable index to one-hot enable signal
reg [7:0] subword_off_en;

always @(*) begin
  subword_off_en = 8'b0;

  case (subword_en_idx)
    3'd0: subword_off_en[0] = 1'b1;
    3'd1: subword_off_en[1] = 1'b1;
    3'd2: subword_off_en[2] = 1'b1;
    3'd3: subword_off_en[3] = 1'b1;
    3'd4: subword_off_en[4] = 1'b1;
    3'd5: subword_off_en[5] = 1'b1;
    3'd6: subword_off_en[6] = 1'b1;
    3'd7: subword_off_en[7] = 1'b1;
    default: subword_off_en = 8'b0;

  endcase
end

wire[31:0] seq_log_out;
param_DeserializedSeqLog seq_log
(
  .clk (clk),
  .subword_0_en (subword_off_en[0]),
  .subword_1_en (subword_off_en[1]),
  .subword_2_en (subword_off_en[2]),
  .subword_3_en (subword_off_en[3]),
  .subword_4_en (subword_off_en[4]),
  .subword_5_en (subword_off_en[5]),
  .subword_6_en (subword_off_en[6]),
  .subword_7_en (subword_off_en[7]),
  .write_data (write_data),
  .out (seq_log_out)

);


always @(*) begin
  out = seq_log_out;
end


endmodule
