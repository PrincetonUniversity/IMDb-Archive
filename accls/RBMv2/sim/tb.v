module tb;

reg clk;
reg rst;

initial begin

	$dumpfile("test.vcd");
	$dumpvars;
	
    clk = 0;
    rst = 0;
    #5; 
    clk = 1;
    #5;
    clk = 0;
    #3;
    rst = 1;
    #2;
    clk = 1;
    #5;
    clk = 0;
    #1;
    rst = 0;
    #4;
    forever #5 clk = ~clk;
end

reg[31:0] cnt;

always @(posedge clk) begin
    if (rst) begin
        cnt <= 0;
    end
    else 
        cnt <= cnt +1;
end


always @(posedge clk) begin
	if(cnt == 32'd60000)
		$finish;
end
/*
always @(posedge clk ) begin
    if (rst) begin
        rd_requested <= 1'b0;
        rd_trans <= 0;
    end
    else if (rd_request) begin
        rd_requested <= 1'b1;       
        rd_trans <=  rd_length;
    end
    else if (rd_trans == 1 && data_in_trans ) begin
        rd_requested <= 1'b0;
    end
end*/


reg conf_done;
reg [31:0] conf_num_hidden;
reg [31:0] conf_num_loops;
reg [31:0] conf_num_movies;
reg [31:0] conf_num_testusers;
reg [31:0] conf_num_users;
reg [31:0] conf_num_visible;


reg rd_grant;
reg wr_grant;

reg data_in_valid = 1'b1;
reg [31:0] data_in_data;
wire data_in_ready;
wire data_in_trans = data_in_valid & data_in_ready;

reg data_out_ready = 1'b1;
wire [31:0] data_out_data;
wire data_out_valid;
wire data_out_trans = data_out_ready & data_out_valid;

wire done;

wire [31:0] rd_index;
wire [31:0] rd_length;
wire rd_request;

reg rd_rq_rand;
reg rd_requested;
reg [31:0] rd_trans;

wire [31:0] wr_index;
wire [31:0] wr_length;
wire wr_request;


reg wr_rq_rand;


rbm_0_top_cmos32soi_rtl dut(
    .clk(clk),
    .rst(~rst),

    .conf_done(conf_done),
    .conf_num_hidden(conf_num_hidden),
    .conf_num_loops(conf_num_loops),
    .conf_num_movies(conf_num_movies),
    .conf_num_testusers(conf_num_testusers),
    .conf_num_users(conf_num_users),
    .conf_num_visible(conf_num_visible),

    .rd_grant(rd_grant),
    .wr_grant(wr_grant),

    .data_in_valid(data_in_valid),
    .data_in_data(data_in_data),
    .data_in_ready(data_in_ready),

    .data_out_ready(data_out_ready),
    .data_out_data(data_out_data),
    .data_out_valid(data_out_valid),

    .done(done),

    .rd_index(rd_index),
    .rd_length(rd_length),
    .rd_request(rd_request),

    .wr_index(wr_index),
    .wr_length(wr_length),
    .wr_request(wr_request)

    );





reg [31:0] nm;
always @(posedge clk) begin
    if(rst) begin
        conf_done <= 1'b0;    	
    end
    else if(cnt == 10) begin     // nv == 5*nm : mismatch nv == 5*nm
        nm = 5;
        conf_done <= 1'b1;
        conf_num_hidden <= 10;
        conf_num_loops <= 2;
        conf_num_movies <= nm; 
        conf_num_testusers <= 2;
        conf_num_users <= 2;
        conf_num_visible <= 5*nm;
    end
    else if(cnt == 11) begin
    	conf_done <= 1'b0;
    end
end



always @(posedge clk) begin
    rd_grant <= $random;
    wr_grant <= $random;
    data_in_valid <= $random;
    data_in_data <= $random;
    data_out_ready <= 1'b1;
    //rd_rq_rand <= ;
    rd_requested <= $random;
    //rd_trans <= ;
end

endmodule
