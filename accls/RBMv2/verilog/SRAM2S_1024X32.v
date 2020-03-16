module SRAM2S_1024X32(
	input CLK,
	
	input CE0,
	input[9:0] A0,
	input[31:0] D0,
	input WE0,
	input [31:0] WEM0,
	output reg[31:0] Q0,
	
	input CE1,
	input[9:0] A1,
	input[31:0] D1,
	input WE1,
	input [31:0] WEM1,
	output reg[31:0] Q1
	);
	
	reg[31:0] mem[0:1023];
	
	always @(posedge CLK) begin
		if(CE0) begin
			Q0 <= mem[A0];
			if(WE0)
				mem[A0] <= D0;
		end
		else
			Q0 <= 32'hxxxxxxxx;
	end
	
	always @(posedge CLK) begin
		if(CE1) begin
			Q1 <= mem[A1];
			if(WE1)
				mem[A1] <= D1;
		end
		else
			Q1 <= 32'hxxxxxxxx;
	end
	
endmodule
