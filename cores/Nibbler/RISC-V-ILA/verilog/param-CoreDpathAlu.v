//============================================================================
// Paramaterizable bit-width ALU containing add/sub, logical, shifter. (mul?)
//============================================================================



//-------------------------------------------------------------------------
// Main alu
//-------------------------------------------------------------------------

module  param_CoreDpathAlu
// #(
//   parameter P_NBITS = 4
// )
(
  input [3:0] in_a,
  input [3:0] in_b,

  input               in_c,      // [P_NBITS-1:0]? 
  input               addsub_fn, // 0=add, 1=sub
  input [1:0]         logic_fn,  // XOR=00, OR=10, AND=11 (bits [14:13] of IR)
  input [1:0]         shift_fn,  //TODO confirm #
  output [3:0]     sum_out,

  output                   carry_out,  // TODO: add more flag signals or change logical meaning of signal as necessary
  output                   a_b_not_eq, // Indicates in_a != in_b
  output reg [3:0] fn_out

);

  localparam P_NBITS = 4;
  localparam C_N_OFF = 8;
  localparam C_OFFBITS = 3;


// Add/Sub unit
wire [P_NBITS-1:0] b_mux_out = addsub_fn ? ~in_b : in_b;
assign {carry_out, sum_out} = in_a + b_mux_out + in_c; // TODO: Confirm the different bit widths doesn't cause bug

// Logical Unit
localparam FN_XOR = 2'b00;
localparam FN_AND = 2'b11;
localparam FN_OR  = 2'b10;

wire [P_NBITS-1:0] xor_out = in_a ^ in_b;
wire [P_NBITS-1:0] and_out = in_a & in_b;
wire [P_NBITS-1:0] or_out  = in_a | in_b;

always @(*) begin
  case (logic_fn)
    FN_XOR:  fn_out = xor_out;
    FN_AND:  fn_out = and_out;
    FN_OR:   fn_out = or_out;
    default: fn_out = and_out;
  endcase
end

assign a_b_not_eq = |xor_out;

endmodule

