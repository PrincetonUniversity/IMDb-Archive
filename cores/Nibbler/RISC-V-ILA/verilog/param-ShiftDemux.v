`ifndef PARAM_SHIFT_DEMUX_V
`define PARAM_SHIFT_DEMUX_V

module param_ShiftDemux
(
  input reset,
  input clk,

  input direction, //1 = +1 = indexing one left (towards MSB).
  input en,

  output reg [2:0]  idx  // Current index (to be read from)

);

// Address Calculation based on inputs
reg [2:0] idx_next;

always @(*) begin
  idx_next = idx;
  if (reset) begin
    idx_next = 3'b0;

  end else if(en) begin
    if (direction) begin
      idx_next = idx + 3'b1;

    end else begin
      idx_next = idx - 3'b1;

    end
  end
end

// Note: not gating or now since it's in ctrl, not critical.
always @ (posedge clk) begin
  idx <= idx_next;
end

// // Handle "shifting in" from either the left or the right
// < %
// print 'wire ['+str(p.C_OFFBITS-1)+':0] idx_out = direction ? idx : idx - '+str(p.C_OFFBITS)+'\'b1;'
// % >

// always @(*)
// begin
// < %
// print '  en_out = '+str(p.C_N_OFF)+'\'b0;'
// % >
//   case (idx)
// < % 
// for i in range(p.C_N_OFF):
//   print '    '+str(p.C_OFFBITS)+'\'d'+str(i)+': en_out['+str(i)+'] = 1\'b1;'
// print '    default: en_out = '+str(p.C_N_OFF)+'\'b0;'
// % >    
//   endcase
// end


endmodule

`endif