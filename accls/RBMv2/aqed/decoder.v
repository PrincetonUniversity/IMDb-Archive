module decoder(
__DECODER_CLOCK__,
__DECODER_RESET__,
__ISSUE__,
clk,
conf_done,
conf_num_hidden,
conf_num_loops,
conf_num_movies,
conf_num_testusers,
conf_num_users,
conf_num_visible,
data_in_data,
data_in_valid,
data_out_ready,
rd_grant,
rst,
wr_grant,
__ILA_RBM_decode_of_Configure__,
__ILA_RBM_decode_of_Reset__,
__ILA_VAR_conf_done,
__ILA_VAR_rst,
__ILA_RBM_decode_of_Reset___D_,
__ILA_RBM_decode_of_Configure___D_
);
input            __DECODER_CLOCK__;
input            __DECODER_RESET__;
input            __ISSUE__;
input            clk;
input            conf_done;
input     [31:0] conf_num_hidden;
input     [31:0] conf_num_loops;
input     [31:0] conf_num_movies;
input     [31:0] conf_num_testusers;
input     [31:0] conf_num_users;
input     [31:0] conf_num_visible;
input     [31:0] data_in_data;
input            data_in_valid;
input            data_out_ready;
input            rd_grant;
input            rst;
input            wr_grant;
output            __ILA_RBM_decode_of_Configure__;
output            __ILA_RBM_decode_of_Reset__;
output            __ILA_VAR_conf_done;
output            __ILA_VAR_rst;
output reg            __ILA_RBM_decode_of_Reset___D_;
output reg            __ILA_RBM_decode_of_Configure___D_;
wire            __DECODER_CLOCK__;
wire            __DECODER_RESET__;
wire            __ILA_RBM_decode_of_Configure__;
wire            __ILA_RBM_decode_of_Reset__;
(* keep *) wire            __ILA_VAR_conf_done;
(* keep *) wire            __ILA_VAR_rst;
wire            __ISSUE__;
wire            bv_1_0_n0;
wire            bv_1_1_n2;
wire            n1;
wire            n3;
wire            n4;
wire            n5;
assign bv_1_0_n0 = 1'h0 ;
assign n1 =  ( __ILA_VAR_rst ) == ( bv_1_0_n0 )  ;
assign bv_1_1_n2 = 1'h1 ;
assign n3 =  ( __ILA_VAR_rst ) == ( bv_1_1_n2 )  ;
assign n4 =  ( __ILA_VAR_conf_done ) == ( bv_1_1_n2 )  ;
assign n5 =  ( n3 ) & (n4 )  ;
// START OF DECODES //
assign __ILA_RBM_decode_of_Reset__ = n1 ;
assign __ILA_RBM_decode_of_Configure__ = n5 ;
assign __ILA_VAR_conf_done = conf_done ;
assign __ILA_VAR_rst = rst ;
// START OF ASSUMPTIONS : Valid Sequences //
assume property ( !__ISSUE__ || (~ __ILA_RBM_decode_of_Reset__|| ((!__ILA_RBM_decode_of_Reset___D_&&!__ILA_RBM_decode_of_Configure___D_)||__ILA_RBM_decode_of_Reset___D_||__ILA_RBM_decode_of_Configure___D_) ) );
assume property ( !__ISSUE__ || (~ __ILA_RBM_decode_of_Configure__|| (__ILA_RBM_decode_of_Reset___D_||(!__ILA_RBM_decode_of_Reset___D_&&!__ILA_RBM_decode_of_Configure___D_)) ) );
// START OF ASSUMPTIONS : SequenceAssumtionsAny //
any_valid_instr : assume property ( !__ISSUE__ || (__ILA_RBM_decode_of_Reset__||__ILA_RBM_decode_of_Configure__) );
// START OF ASSUMPTIONS : GenSequenceOneAtATime //
assume property ( !__ISSUE__ || (! (__ILA_RBM_decode_of_Reset__&&__ILA_RBM_decode_of_Configure__) ) );
always @(posedge __DECODER_CLOCK__) begin
   if(__DECODER_RESET__) begin
       __ILA_RBM_decode_of_Reset___D_ <= 1'b0; 
       __ILA_RBM_decode_of_Configure___D_ <= 1'b0; 
   end
   else if(1) begin
       if (__ISSUE__) begin
           __ILA_RBM_decode_of_Reset___D_ <= __ILA_RBM_decode_of_Reset__ ; ;
       end
       if (__ISSUE__) begin
           __ILA_RBM_decode_of_Configure___D_ <= __ILA_RBM_decode_of_Configure__ ; ;
       end
   end
end
endmodule
