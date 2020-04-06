module decoder(
__DECODER_CLOCK__,
__DECODER_RESET__,
__ISSUE__,
addr,
clk,
data_in,
rst,
stb,
wr,
xram_ack,
xram_data_in,
__ILA_AES_decode_of_GET_STATUS__,
__ILA_AES_decode_of_READ_ADDRESS__,
__ILA_AES_decode_of_READ_COUNTER__,
__ILA_AES_decode_of_READ_KEY__,
__ILA_AES_decode_of_READ_LENGTH__,
__ILA_AES_decode_of_START_ENCRYPT__,
__ILA_AES_decode_of_WRITE_ADDRESS__,
__ILA_AES_decode_of_WRITE_COUNTER__,
__ILA_AES_decode_of_WRITE_KEY__,
__ILA_AES_decode_of_WRITE_LENGTH__,
__ILA_VAR_cmd,
__ILA_VAR_cmdaddr,
__ILA_VAR_cmddata
);
input            __DECODER_CLOCK__;
input            __DECODER_RESET__;
input            __ISSUE__;
input     [15:0] addr;
input            clk;
input      [7:0] data_in;
input            rst;
input            stb;
input            wr;
input            xram_ack;
input      [7:0] xram_data_in;
output            __ILA_AES_decode_of_GET_STATUS__;
output            __ILA_AES_decode_of_READ_ADDRESS__;
output            __ILA_AES_decode_of_READ_COUNTER__;
output            __ILA_AES_decode_of_READ_KEY__;
output            __ILA_AES_decode_of_READ_LENGTH__;
output            __ILA_AES_decode_of_START_ENCRYPT__;
output            __ILA_AES_decode_of_WRITE_ADDRESS__;
output            __ILA_AES_decode_of_WRITE_COUNTER__;
output            __ILA_AES_decode_of_WRITE_KEY__;
output            __ILA_AES_decode_of_WRITE_LENGTH__;
output      [1:0] __ILA_VAR_cmd;
output     [15:0] __ILA_VAR_cmdaddr;
output      [7:0] __ILA_VAR_cmddata;
wire            __DECODER_CLOCK__;
wire            __DECODER_RESET__;
wire            __ILA_AES_decode_of_GET_STATUS__;
wire            __ILA_AES_decode_of_READ_ADDRESS__;
wire            __ILA_AES_decode_of_READ_COUNTER__;
wire            __ILA_AES_decode_of_READ_KEY__;
wire            __ILA_AES_decode_of_READ_LENGTH__;
wire            __ILA_AES_decode_of_START_ENCRYPT__;
wire            __ILA_AES_decode_of_WRITE_ADDRESS__;
wire            __ILA_AES_decode_of_WRITE_COUNTER__;
wire            __ILA_AES_decode_of_WRITE_KEY__;
wire            __ILA_AES_decode_of_WRITE_LENGTH__;
(* keep *) wire      [1:0] __ILA_VAR_cmd;
(* keep *) wire     [15:0] __ILA_VAR_cmdaddr;
(* keep *) wire      [7:0] __ILA_VAR_cmddata;
wire            __ISSUE__;
wire     [15:0] bv_16_65280_n11;
wire     [15:0] bv_16_65282_n2;
wire     [15:0] bv_16_65284_n7;
wire     [15:0] bv_16_65286_n23;
wire     [15:0] bv_16_65296_n34;
wire     [15:0] bv_16_65312_n39;
wire     [15:0] bv_16_65328_n47;
wire      [1:0] bv_2_1_n17;
wire      [1:0] bv_2_2_n0;
wire      [7:0] bv_8_1_n14;
wire            n1;
wire            n10;
wire            n12;
wire            n13;
wire            n15;
wire            n16;
wire            n18;
wire            n19;
wire            n20;
wire            n21;
wire            n22;
wire            n24;
wire            n25;
wire            n26;
wire            n27;
wire            n28;
wire            n29;
wire            n3;
wire            n30;
wire            n31;
wire            n32;
wire            n33;
wire            n35;
wire            n36;
wire            n37;
wire            n38;
wire            n4;
wire            n40;
wire            n41;
wire            n42;
wire            n43;
wire            n44;
wire            n45;
wire            n46;
wire            n48;
wire            n49;
wire            n5;
wire            n50;
wire            n51;
wire            n52;
wire            n53;
wire            n54;
wire            n55;
wire            n56;
wire            n57;
wire            n58;
wire            n59;
wire            n6;
wire            n60;
wire            n61;
wire            n62;
wire            n63;
wire            n64;
wire            n65;
wire            n66;
wire            n67;
wire            n68;
wire            n69;
wire            n70;
wire            n71;
wire            n72;
wire            n73;
wire            n74;
wire            n75;
wire            n76;
wire            n77;
wire            n8;
wire            n9;
assign bv_2_2_n0 = 2'h2 ;
assign n1 =  ( __ILA_VAR_cmd ) == ( bv_2_2_n0 )  ;
assign bv_16_65282_n2 = 16'hff02 ;
assign n3 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65282_n2 )  ;
assign n4 =  ( __ILA_VAR_cmdaddr ) > ( bv_16_65282_n2 )  ;
assign n5 =  ( n3 ) | ( n4 )  ;
assign n6 =  ( n1 ) & (n5 )  ;
assign bv_16_65284_n7 = 16'hff04 ;
assign n8 =  ( __ILA_VAR_cmdaddr ) < ( bv_16_65284_n7 )  ;
assign n9 =  ( n6 ) & (n8 )  ;
assign n10 =  ( __ILA_VAR_cmd ) == ( bv_2_2_n0 )  ;
assign bv_16_65280_n11 = 16'hff00 ;
assign n12 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65280_n11 )  ;
assign n13 =  ( n10 ) & (n12 )  ;
assign bv_8_1_n14 = 8'h1 ;
assign n15 =  ( __ILA_VAR_cmddata ) == ( bv_8_1_n14 )  ;
assign n16 =  ( n13 ) & (n15 )  ;
assign bv_2_1_n17 = 2'h1 ;
assign n18 =  ( __ILA_VAR_cmd ) == ( bv_2_1_n17 )  ;
assign n19 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65284_n7 )  ;
assign n20 =  ( __ILA_VAR_cmdaddr ) > ( bv_16_65284_n7 )  ;
assign n21 =  ( n19 ) | ( n20 )  ;
assign n22 =  ( n18 ) & (n21 )  ;
assign bv_16_65286_n23 = 16'hff06 ;
assign n24 =  ( __ILA_VAR_cmdaddr ) < ( bv_16_65286_n23 )  ;
assign n25 =  ( n22 ) & (n24 )  ;
assign n26 =  ( __ILA_VAR_cmd ) == ( bv_2_1_n17 )  ;
assign n27 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65282_n2 )  ;
assign n28 =  ( __ILA_VAR_cmdaddr ) > ( bv_16_65282_n2 )  ;
assign n29 =  ( n27 ) | ( n28 )  ;
assign n30 =  ( n26 ) & (n29 )  ;
assign n31 =  ( __ILA_VAR_cmdaddr ) < ( bv_16_65284_n7 )  ;
assign n32 =  ( n30 ) & (n31 )  ;
assign n33 =  ( __ILA_VAR_cmd ) == ( bv_2_1_n17 )  ;
assign bv_16_65296_n34 = 16'hff10 ;
assign n35 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65296_n34 )  ;
assign n36 =  ( __ILA_VAR_cmdaddr ) > ( bv_16_65296_n34 )  ;
assign n37 =  ( n35 ) | ( n36 )  ;
assign n38 =  ( n33 ) & (n37 )  ;
assign bv_16_65312_n39 = 16'hff20 ;
assign n40 =  ( __ILA_VAR_cmdaddr ) < ( bv_16_65312_n39 )  ;
assign n41 =  ( n38 ) & (n40 )  ;
assign n42 =  ( __ILA_VAR_cmd ) == ( bv_2_1_n17 )  ;
assign n43 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65312_n39 )  ;
assign n44 =  ( __ILA_VAR_cmdaddr ) > ( bv_16_65312_n39 )  ;
assign n45 =  ( n43 ) | ( n44 )  ;
assign n46 =  ( n42 ) & (n45 )  ;
assign bv_16_65328_n47 = 16'hff30 ;
assign n48 =  ( __ILA_VAR_cmdaddr ) < ( bv_16_65328_n47 )  ;
assign n49 =  ( n46 ) & (n48 )  ;
assign n50 =  ( __ILA_VAR_cmd ) == ( bv_2_1_n17 )  ;
assign n51 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65312_n39 )  ;
assign n52 =  ( __ILA_VAR_cmdaddr ) > ( bv_16_65312_n39 )  ;
assign n53 =  ( n51 ) | ( n52 )  ;
assign n54 =  ( n50 ) & (n53 )  ;
assign n55 =  ( __ILA_VAR_cmdaddr ) < ( bv_16_65328_n47 )  ;
assign n56 =  ( n54 ) & (n55 )  ;
assign n57 =  ( __ILA_VAR_cmd ) == ( bv_2_2_n0 )  ;
assign n58 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65284_n7 )  ;
assign n59 =  ( __ILA_VAR_cmdaddr ) > ( bv_16_65284_n7 )  ;
assign n60 =  ( n58 ) | ( n59 )  ;
assign n61 =  ( n57 ) & (n60 )  ;
assign n62 =  ( __ILA_VAR_cmdaddr ) < ( bv_16_65286_n23 )  ;
assign n63 =  ( n61 ) & (n62 )  ;
assign n64 =  ( __ILA_VAR_cmd ) == ( bv_2_2_n0 )  ;
assign n65 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65296_n34 )  ;
assign n66 =  ( __ILA_VAR_cmdaddr ) > ( bv_16_65296_n34 )  ;
assign n67 =  ( n65 ) | ( n66 )  ;
assign n68 =  ( n64 ) & (n67 )  ;
assign n69 =  ( __ILA_VAR_cmdaddr ) < ( bv_16_65312_n39 )  ;
assign n70 =  ( n68 ) & (n69 )  ;
assign n71 =  ( __ILA_VAR_cmd ) == ( bv_2_2_n0 )  ;
assign n72 =  ( __ILA_VAR_cmdaddr ) == ( bv_16_65312_n39 )  ;
assign n73 =  ( __ILA_VAR_cmdaddr ) > ( bv_16_65312_n39 )  ;
assign n74 =  ( n72 ) | ( n73 )  ;
assign n75 =  ( n71 ) & (n74 )  ;
assign n76 =  ( __ILA_VAR_cmdaddr ) < ( bv_16_65328_n47 )  ;
assign n77 =  ( n75 ) & (n76 )  ;
// START OF DECODES //
assign __ILA_AES_decode_of_WRITE_ADDRESS__ = n9 ;
assign __ILA_AES_decode_of_START_ENCRYPT__ = n16 ;
assign __ILA_AES_decode_of_READ_LENGTH__ = n25 ;
assign __ILA_AES_decode_of_READ_ADDRESS__ = n32 ;
assign __ILA_AES_decode_of_READ_KEY__ = n41 ;
assign __ILA_AES_decode_of_READ_COUNTER__ = n49 ;
assign __ILA_AES_decode_of_GET_STATUS__ = n56 ;
assign __ILA_AES_decode_of_WRITE_LENGTH__ = n63 ;
assign __ILA_AES_decode_of_WRITE_KEY__ = n70 ;
assign __ILA_AES_decode_of_WRITE_COUNTER__ = n77 ;
assign __ILA_VAR_cmdaddr = addr ;
assign __ILA_VAR_cmddata = data_in ;
assume property ((~ ( __ILA_VAR_cmd == 2 )) || ( wr == 1));
assume property ((~ ( __ILA_VAR_cmd == 1 )) || ( wr == 0));
// START OF ASSUMPTIONS : SequenceAssumtionsAny //
any_valid_instr : assume property ( !__ISSUE__ || (__ILA_AES_decode_of_WRITE_ADDRESS__||__ILA_AES_decode_of_START_ENCRYPT__||__ILA_AES_decode_of_READ_LENGTH__||__ILA_AES_decode_of_READ_ADDRESS__||__ILA_AES_decode_of_READ_KEY__||__ILA_AES_decode_of_READ_COUNTER__||__ILA_AES_decode_of_GET_STATUS__||__ILA_AES_decode_of_WRITE_LENGTH__||__ILA_AES_decode_of_WRITE_KEY__||__ILA_AES_decode_of_WRITE_COUNTER__) );
// START OF ASSUMPTIONS : GenSequenceOneAtATime //
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_ADDRESS__&&__ILA_AES_decode_of_START_ENCRYPT__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_ADDRESS__&&__ILA_AES_decode_of_READ_LENGTH__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_ADDRESS__&&__ILA_AES_decode_of_READ_ADDRESS__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_ADDRESS__&&__ILA_AES_decode_of_READ_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_ADDRESS__&&__ILA_AES_decode_of_READ_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_ADDRESS__&&__ILA_AES_decode_of_GET_STATUS__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_ADDRESS__&&__ILA_AES_decode_of_WRITE_LENGTH__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_ADDRESS__&&__ILA_AES_decode_of_WRITE_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_ADDRESS__&&__ILA_AES_decode_of_WRITE_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_START_ENCRYPT__&&__ILA_AES_decode_of_READ_LENGTH__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_START_ENCRYPT__&&__ILA_AES_decode_of_READ_ADDRESS__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_START_ENCRYPT__&&__ILA_AES_decode_of_READ_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_START_ENCRYPT__&&__ILA_AES_decode_of_READ_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_START_ENCRYPT__&&__ILA_AES_decode_of_GET_STATUS__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_START_ENCRYPT__&&__ILA_AES_decode_of_WRITE_LENGTH__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_START_ENCRYPT__&&__ILA_AES_decode_of_WRITE_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_START_ENCRYPT__&&__ILA_AES_decode_of_WRITE_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_LENGTH__&&__ILA_AES_decode_of_READ_ADDRESS__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_LENGTH__&&__ILA_AES_decode_of_READ_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_LENGTH__&&__ILA_AES_decode_of_READ_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_LENGTH__&&__ILA_AES_decode_of_GET_STATUS__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_LENGTH__&&__ILA_AES_decode_of_WRITE_LENGTH__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_LENGTH__&&__ILA_AES_decode_of_WRITE_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_LENGTH__&&__ILA_AES_decode_of_WRITE_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_ADDRESS__&&__ILA_AES_decode_of_READ_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_ADDRESS__&&__ILA_AES_decode_of_READ_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_ADDRESS__&&__ILA_AES_decode_of_GET_STATUS__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_ADDRESS__&&__ILA_AES_decode_of_WRITE_LENGTH__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_ADDRESS__&&__ILA_AES_decode_of_WRITE_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_ADDRESS__&&__ILA_AES_decode_of_WRITE_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_KEY__&&__ILA_AES_decode_of_READ_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_KEY__&&__ILA_AES_decode_of_GET_STATUS__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_KEY__&&__ILA_AES_decode_of_WRITE_LENGTH__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_KEY__&&__ILA_AES_decode_of_WRITE_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_KEY__&&__ILA_AES_decode_of_WRITE_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_COUNTER__&&__ILA_AES_decode_of_GET_STATUS__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_COUNTER__&&__ILA_AES_decode_of_WRITE_LENGTH__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_COUNTER__&&__ILA_AES_decode_of_WRITE_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_READ_COUNTER__&&__ILA_AES_decode_of_WRITE_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_GET_STATUS__&&__ILA_AES_decode_of_WRITE_LENGTH__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_GET_STATUS__&&__ILA_AES_decode_of_WRITE_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_GET_STATUS__&&__ILA_AES_decode_of_WRITE_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_LENGTH__&&__ILA_AES_decode_of_WRITE_KEY__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_LENGTH__&&__ILA_AES_decode_of_WRITE_COUNTER__) ) );
assume property ( !__ISSUE__ || (! (__ILA_AES_decode_of_WRITE_KEY__&&__ILA_AES_decode_of_WRITE_COUNTER__) ) );
endmodule
