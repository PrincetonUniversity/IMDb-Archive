
`ifndef PARAM_CLKENBUF_V
`define PARAM_CLKENBUF_V

module param_ClkEnBuf (clk, rclk, en_l);
output clk;
input  rclk, en_l;
reg    clken;

  always @ (rclk or en_l)
    if (!rclk)  //latch opens on rclk low phase
      clken = !en_l ;
  assign clk = clken & rclk;

endmodule

`endif