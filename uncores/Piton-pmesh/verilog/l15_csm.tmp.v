/*
Copyright (c) 2015 Princeton University
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Princeton University nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY PRINCETON UNIVERSITY "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL PRINCETON UNIVERSITY BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//==================================================================================================
//  Filename      : l15_csm.v
//  Created On    : 2014-01-31 18:24:47
//  Revision      :
//  Author        : Tri Nguyen
//  Company       : Princeton University
//  Email         : trin@princeton.edu
//
//  Description   :
//
//
//==================================================================================================

//`timescale 1 ns / 10 ps
`include "l15.tmp.h"
`include "define.tmp.h"


module l15_csm(
   input wire clk,
   input wire rst_n,
   input wire csm_en,

   // static system input / configure registers
   input wire [`HOME_ID_WIDTH-1:0] system_tile_count,
   input wire [`HOME_ALLOC_METHOD_WIDTH-1:0] home_alloc_method, 
   input wire [`L15_HMT_BASE_ADDR_WIDTH-1:0] l15_hmt_base_reg,

   // interface to noc1buffer
   input wire [`L15_CSM_NUM_TICKETS_LOG2-1:0] l15_csm_read_ticket,
   input wire [`L15_CSM_NUM_TICKETS_LOG2-1:0] l15_csm_clear_ticket,
   input wire l15_csm_clear_ticket_val,
   output wire [`PACKET_HOME_ID_WIDTH-1:0] csm_l15_read_res_data,
   output wire csm_l15_read_res_val,

   // interface with pipeline
   input wire [`PHY_ADDR_WIDTH-1:0] l15_csm_req_address_s2,
   input wire l15_csm_req_val_s2,
   input wire l15_csm_stall_s3,
   input wire [`L15_CSM_NUM_TICKETS_LOG2-1:0] l15_csm_req_ticket_s2,
   input wire  l15_csm_req_type_s2,     //0 for load, 1 for store
   input wire [`L15_HMC_DATA_IN_WIDTH-1:0] l15_csm_req_data_s2,
   input wire [`TLB_CSM_WIDTH-1:0] l15_csm_req_pcx_data_s2, //
   output reg csm_l15_res_val_s3,
   output reg [63:0] csm_l15_res_data_s3,

   // noc1 interface
   input wire noc1encoder_csm_req_ack,
   output wire csm_noc1encoder_req_val,
   output wire [`L15_NOC1_REQTYPE_WIDTH-1:0] csm_noc1encoder_req_type,
   output wire [`L15_CSM_NUM_TICKETS_LOG2-1:0] csm_noc1encoder_req_mshrid,
   output wire [`PHY_ADDR_WIDTH-1:0] csm_noc1encoder_req_address,
   output wire csm_noc1encoder_req_non_cacheable,
   output wire  [`PCX_SIZE_WIDTH-1:0] csm_noc1encoder_req_size

);
`ifndef NOMEM
// storage for ticketed ghid translations
reg [`PACKET_HOME_ID_WIDTH-1:0] ghid_ticketed_cache [`L15_CSM_NUM_TICKETS-1:0];
reg [`L15_HMC_ADDR_WIDTH-1:0] ghid_ticketed_cache_addr [`L15_CSM_NUM_TICKETS-1:0];
`endif //NOMEM

reg [`L15_CSM_NUM_TICKETS-1:0] ghid_ticketed_cache_val;
// write to the ticketed ghid trans
reg [`L15_CSM_NUM_TICKETS_LOG2-1:0] write_index_s2;
reg write_val_s2;
reg [`L15_CSM_NUM_TICKETS_LOG2-1:0] read_index_s2;
reg read_val_s2;

reg [`HOME_ID_WIDTH-1:0] num_homes_s2;
wire [`HOME_ID_WIDTH-1:0] lhid_s2;
reg [`PACKET_HOME_ID_WIDTH-1:0] ghid_s2;
reg ghid_val_s2;

//Stage 2

reg diag_en_s2;
reg flush_en_s2;
reg rd_en_s2;
reg wr_en_s2;
reg [`L15_ADDR_TYPE_WIDTH-1:0] addr_type_s2;
reg [`L15_HMC_ADDR_WIDTH-1:0] addr_in_s2;
reg [`L15_HMC_ADDR_WIDTH-1:0] addr_in_s2_next;
reg [`HOME_ID_WIDTH-1:0] home_addr_bits_s2;
reg special_l2_addr_s2;

reg [`HOME_ID_WIDTH-1:0] l15_csm_clump_tile_count_s2;
reg [`MSG_HDID_WIDTH-1:0] l15_csm_req_clump_id_s2;
reg [`NOC_CHIPID_WIDTH-1:0] l15_csm_chipid_s2;
reg [`NOC_X_WIDTH-1:0] l15_csm_x_s2;
reg [`NOC_Y_WIDTH-1:0] l15_csm_y_s2;
reg l15_csm_clump_sel_s2;


always @ *
begin
    l15_csm_clump_sel_s2 = l15_csm_req_pcx_data_s2[`TLB_CSM_STATE];
end


always @ *
begin
    if (l15_csm_clump_sel_s2 == `TLB_CSM_STATE_LOCAL)
    begin
        l15_csm_clump_tile_count_s2 = l15_csm_req_pcx_data_s2[`TLB_CSM_HD_SIZE];
        l15_csm_req_clump_id_s2 =  l15_csm_req_pcx_data_s2[`TLB_CSM_HDID];
    end
    else
    begin
        l15_csm_clump_tile_count_s2 = 0;
        l15_csm_req_clump_id_s2 =  0;
    end
end

always @ *
begin
    if (l15_csm_clump_sel_s2 == `TLB_CSM_STATE_GLOBAL)
    begin
        l15_csm_chipid_s2 = l15_csm_req_pcx_data_s2[`TLB_CSM_CHIPID];
        l15_csm_x_s2 =  l15_csm_req_pcx_data_s2[`TLB_CSM_X];
        l15_csm_y_s2 =  l15_csm_req_pcx_data_s2[`TLB_CSM_Y];
    end
    else
    begin
        l15_csm_chipid_s2 = 0;
        l15_csm_x_s2 =  0;
        l15_csm_y_s2 =  0;
    end
end


always @ *
begin
    addr_type_s2 = l15_csm_req_address_s2[`L15_ADDR_TYPE];
end

always @ *
begin
    //special l2 addresses start with 0xA
    special_l2_addr_s2 = (l15_csm_req_address_s2[39:36] == 4'b1010);
end

always @ *
begin
    if (special_l2_addr_s2)
    begin
        home_addr_bits_s2 = l15_csm_req_address_s2[`HOME_ID_ADDR_POS_HIGH];
    end
    else
    begin
        case (home_alloc_method)
        `HOME_ALLOC_LOW_ORDER_BITS:
        begin
            home_addr_bits_s2 = l15_csm_req_address_s2[`HOME_ID_ADDR_POS_LOW];
        end
        `HOME_ALLOC_MIDDLE_ORDER_BITS:
        begin
            home_addr_bits_s2 = l15_csm_req_address_s2[`HOME_ID_ADDR_POS_MIDDLE];
        end
        `HOME_ALLOC_HIGH_ORDER_BITS:
        begin
            home_addr_bits_s2 = l15_csm_req_address_s2[`HOME_ID_ADDR_POS_HIGH];
        end
        `HOME_ALLOC_MIXED_ORDER_BITS:
        begin
            home_addr_bits_s2 = (l15_csm_req_address_s2[`HOME_ID_ADDR_POS_LOW] ^ l15_csm_req_address_s2[`HOME_ID_ADDR_POS_MIDDLE]);
        end
        endcase
    end
end


always @ *
begin
    diag_en_s2 = l15_csm_req_val_s2 && (addr_type_s2 == `L15_ADDR_TYPE_HMC_ACCESS);
    flush_en_s2 = l15_csm_req_val_s2 && (addr_type_s2 == `L15_ADDR_TYPE_HMC_FLUSH);
    rd_en_s2 = l15_csm_req_val_s2 && (l15_csm_req_type_s2 == 1'b0)
            && ~(~diag_en_s2 && ~flush_en_s2 && (l15_csm_clump_sel_s2 == `TLB_CSM_STATE_GLOBAL));
    wr_en_s2 = l15_csm_req_val_s2 && (l15_csm_req_type_s2 == 1'b1);
end

always @ *
begin
   if(diag_en_s2 || flush_en_s2)
   begin
      addr_in_s2 = l15_csm_req_address_s2[`L15_HMC_ADDR_WIDTH+3:4];
   end
   else
   begin
      addr_in_s2 = {l15_csm_req_clump_id_s2, lhid_s2};
   end
end

always @ *
begin
   if (csm_en)
   begin
      num_homes_s2 = l15_csm_clump_tile_count_s2;
   end
   else
   begin
      num_homes_s2 = system_tile_count;
   end
end

l15_home_encoder    l15_home_encoder(
   // .clk            (clk),
   // .rst_n          (rst_n),
   .home_in        (home_addr_bits_s2),
   .num_homes      (num_homes_s2),
   .lhid_out       (lhid_s2)
);

always @ *
begin
   // write to the ticketed storage. This would normally be written to when the module
   //  gets back the response from memory
   write_index_s2 = l15_csm_req_ticket_s2;
   write_val_s2 = l15_csm_req_val_s2 && wr_en_s2 && (~diag_en_s2) && (~flush_en_s2);
end

always @ *
begin
   read_index_s2 = l15_csm_req_ticket_s2;
   read_val_s2 = l15_csm_req_val_s2 && (l15_csm_req_type_s2 == 1'b0);
end



`ifndef NOMEM
always @ *
begin  
    case(ghid_ticketed_cache_addr[write_index_s2][`L15_HMC_ADDR_OFFSET])
    2'd0:
    begin
        ghid_s2 = l15_csm_req_data_s2;
        ghid_val_s2 = l15_csm_req_data_s2 >> ((`L15_HMC_DATA_IN_WIDTH/`L15_HMC_DATA_NUM) - 1);
    end
    2'd1:
    begin
        ghid_s2 = l15_csm_req_data_s2 >> (`L15_HMC_DATA_IN_WIDTH/`L15_HMC_DATA_NUM);
        ghid_val_s2 = l15_csm_req_data_s2 >> ((`L15_HMC_DATA_IN_WIDTH/`L15_HMC_DATA_NUM) * 2 - 1);
    end
    2'd2:
    begin
        ghid_s2 = l15_csm_req_data_s2 >> ((`L15_HMC_DATA_IN_WIDTH/`L15_HMC_DATA_NUM) * 2);
        ghid_val_s2 = l15_csm_req_data_s2 >> ((`L15_HMC_DATA_IN_WIDTH/`L15_HMC_DATA_NUM) * 3 - 1);
    end
    2'd3:
    begin
        ghid_s2 = l15_csm_req_data_s2 >> ((`L15_HMC_DATA_IN_WIDTH/`L15_HMC_DATA_NUM) * 3);
        ghid_val_s2 = l15_csm_req_data_s2 >> ((`L15_HMC_DATA_IN_WIDTH/`L15_HMC_DATA_NUM) * 4 - 1);
    end
    endcase
end

always @ (posedge clk)
begin
   if (!rst_n)
   begin
      ghid_ticketed_cache_val[0] <= 0;
ghid_ticketed_cache[0] <= 0;
ghid_ticketed_cache_val[1] <= 0;
ghid_ticketed_cache[1] <= 0;
ghid_ticketed_cache_val[2] <= 0;
ghid_ticketed_cache[2] <= 0;
ghid_ticketed_cache_val[3] <= 0;
ghid_ticketed_cache[3] <= 0;
ghid_ticketed_cache_val[4] <= 0;
ghid_ticketed_cache[4] <= 0;
ghid_ticketed_cache_val[5] <= 0;
ghid_ticketed_cache[5] <= 0;
ghid_ticketed_cache_val[6] <= 0;
ghid_ticketed_cache[6] <= 0;
ghid_ticketed_cache_val[7] <= 0;
ghid_ticketed_cache[7] <= 0;

   end
   else
   begin
      if (write_val_s2)
      begin
         ghid_ticketed_cache[write_index_s2] <= ghid_s2;
         ghid_ticketed_cache_val[write_index_s2] <= ghid_val_s2;
         if (l15_csm_clear_ticket_val && (l15_csm_clear_ticket != write_index_s2))
         begin
            ghid_ticketed_cache_val[l15_csm_clear_ticket] <= 1'b0;
         end
      end
      else if (read_val_s2)
      begin
         ghid_ticketed_cache[read_index_s2] <= 0;
         ghid_ticketed_cache_val[read_index_s2] <=1'b0;
         if (l15_csm_clear_ticket_val && (l15_csm_clear_ticket != read_index_s2))
         begin
            ghid_ticketed_cache_val[l15_csm_clear_ticket] <= 1'b0;
         end
      end
      else if (l15_csm_clear_ticket_val)
      begin
          ghid_ticketed_cache_val[l15_csm_clear_ticket] <= 1'b0;
      end
   end
end

always @ *
begin
    if (write_val_s2)
    begin
        addr_in_s2_next = ghid_ticketed_cache_addr[write_index_s2];
    end
    else
    begin
        addr_in_s2_next = addr_in_s2;
    end
end
`endif //NOMEM

//Stage 2 => Stage 3

reg [`PHY_ADDR_WIDTH-1:0] l15_csm_req_address_s3;
reg [`MSG_HDID_WIDTH-1:0] l15_csm_req_clump_id_s3;
reg l15_csm_req_val_s3;
reg [`L15_CSM_NUM_TICKETS_LOG2-1:0] l15_csm_req_ticket_s3;
reg [`L15_HMC_ADDR_WIDTH-1:0] addr_in_s3;
reg [`L15_HMC_DATA_IN_WIDTH-1:0] data_in_s3;
reg [`HOME_ID_WIDTH-1:0] lhid_s3;
reg diag_en_s3;
reg flush_en_s3;
reg rd_en_s3;
reg wr_en_s3;
reg [`NOC_CHIPID_WIDTH-1:0] l15_csm_chipid_s3;
reg [`NOC_X_WIDTH-1:0] l15_csm_x_s3;
reg [`NOC_Y_WIDTH-1:0] l15_csm_y_s3;
reg l15_csm_clump_sel_s3;

always @ (posedge clk)
begin
   if (!rst_n)
   begin
      l15_csm_req_address_s3 <= 0;
      l15_csm_req_clump_id_s3 <= 0;
      l15_csm_req_val_s3 <= 0;
      l15_csm_req_ticket_s3 <= 0;
      addr_in_s3 <= 0;
      data_in_s3 <= 0;
      lhid_s3 <= 0;
      diag_en_s3 <= 0;
      flush_en_s3 <= 0;
      rd_en_s3 <= 0;
      wr_en_s3 <= 0;
      l15_csm_chipid_s3 <= 0;
      l15_csm_x_s3 <= 0;
      l15_csm_y_s3 <= 0;
      l15_csm_clump_sel_s3 <= 0;
   end
   else
   begin
      if (!l15_csm_stall_s3)
      begin
         l15_csm_req_address_s3 <= l15_csm_req_address_s2;
         l15_csm_req_clump_id_s3 <= l15_csm_req_clump_id_s2;
         l15_csm_req_val_s3 <= l15_csm_req_val_s2;
         l15_csm_req_ticket_s3 <= l15_csm_req_ticket_s2;
         addr_in_s3 <= addr_in_s2_next;
         data_in_s3 <= l15_csm_req_data_s2;
         lhid_s3 <= lhid_s2;
         diag_en_s3 <= diag_en_s2;
         flush_en_s3 <= flush_en_s2;
         rd_en_s3 <= rd_en_s2;
         wr_en_s3 <= wr_en_s2;
         l15_csm_chipid_s3 <= l15_csm_chipid_s2;
         l15_csm_x_s3 <= l15_csm_x_s2;
         l15_csm_y_s3 <= l15_csm_y_s2;
         l15_csm_clump_sel_s3 <= l15_csm_clump_sel_s2;
      end
   end
end


//Stage 3

`ifndef NO_RTL_CSM

reg [`L15_HMC_ADDR_OP_WIDTH-1:0] addr_op_s3;
reg refill_req_val_s3;
wire hit_s3;
wire [`L15_HMC_DATA_OUT_WIDTH-1:0] data_out_s3;
wire [`L15_HMC_VALID_WIDTH-1:0] valid_out_s3;
wire [`L15_HMC_TAG_WIDTH-1:0] tag_out_s3;

wire [`NOC_X_WIDTH-1:0] lhid_s3_x;
wire [`NOC_Y_WIDTH-1:0] lhid_s3_y;


always @ *
begin
    addr_op_s3 = l15_csm_req_address_s3[`L15_HMC_ADDR_OP];
end


l15_hmc l15_hmc(
   .clk            (clk),
   .rst_n          (rst_n),
   .rd_en          (rd_en_s3),
   .wr_en          (wr_en_s3),
   .rd_diag_en     (diag_en_s3),
   .wr_diag_en     (diag_en_s3),
   .flush_en       (flush_en_s3),
   .addr_op        (addr_op_s3),
   .rd_addr_in     (addr_in_s3),
   .wr_addr_in     (addr_in_s3),
   .data_in        (data_in_s3),
   .hit            (hit_s3),
   .data_out       (data_out_s3),
   .valid_out      (valid_out_s3),
   .tag_out        (tag_out_s3)
);


always @ *
begin
    refill_req_val_s3 = csm_en && rd_en_s3 && (~diag_en_s3) && (~flush_en_s3) && (~hit_s3);
end

always @ *
begin
    if (csm_en)
    begin
        csm_l15_res_val_s3 = rd_en_s3 && ~refill_req_val_s3;
    end
    else
    begin
        csm_l15_res_val_s3 = l15_csm_req_val_s3;
    end
end

always @ *
begin
    if (diag_en_s3)
    begin
        if (addr_op_s3 == 0)
        begin
            csm_l15_res_data_s3 = data_out_s3;
        end
        else if (addr_op_s3 == 1)
        begin
            csm_l15_res_data_s3 = valid_out_s3;
        end
        else if (addr_op_s3 == 2)
        begin
            csm_l15_res_data_s3 = tag_out_s3;
        end
        else
        begin
            csm_l15_res_data_s3 = 0;
        end
    end
    else
    begin
        if (csm_en)
        begin
            if (~diag_en_s3 && ~flush_en_s3 && (l15_csm_clump_sel_s3 == `TLB_CSM_STATE_GLOBAL))
            begin
                csm_l15_res_data_s3 = 0;
                csm_l15_res_data_s3[`PACKET_HOME_ID_CHIP_MASK] = l15_csm_chipid_s3;
                //TODO, need to make x, y position consistant
                csm_l15_res_data_s3[`PACKET_HOME_ID_X_MASK] = l15_csm_y_s3;
                csm_l15_res_data_s3[`PACKET_HOME_ID_Y_MASK] = l15_csm_x_s3;
            end
            else
            begin
                csm_l15_res_data_s3 = data_out_s3;
            end
        end
        else
        begin
            // csm_l15_res_data_s3 = {{`NOC_CHIPID_WIDTH{1'b0}},
            //                       {(`NOC_X_WIDTH-`HOME_ID_Y_POS_WIDTH){1'b0}}, lhid_s3[`HOME_ID_Y_POS],
            //                       {(`NOC_X_WIDTH-`HOME_ID_X_POS_WIDTH){1'b0}}, lhid_s3[`HOME_ID_X_POS]};
            csm_l15_res_data_s3 = 0;
            csm_l15_res_data_s3[`PACKET_HOME_ID_CHIP_MASK] = 1'b0; // non-csm mode only has 1 chip alone
            csm_l15_res_data_s3[`PACKET_HOME_ID_Y_MASK] = lhid_s3_y;
            csm_l15_res_data_s3[`PACKET_HOME_ID_X_MASK] = lhid_s3_x;
        end
    end
end

flat_id_to_xy lhid_to_xy (
    .flat_id(lhid_s3[`HOME_ID_Y_POS_WIDTH+`HOME_ID_X_POS_WIDTH-1:0]),
    .x_coord(lhid_s3_x),
    .y_coord(lhid_s3_y)
    );

/*
always @ *
begin
    if (csm_en)
    begin
      refill_req_val_s3 = rd_en_s3 && (~diag_en_s3) && (~flush_en_s3) && (~hit_s3);
       ;
      // csm_l15_res_data_s3 = data_out_s3;
      // csm_l15_res_valid_s3 = valid_out_s3;
      // csm_l15_res_tag_s3 = tag_out_s3;
      if (diag_en_s3 && rd_en_s3 && (addr_op_s3 == 1))
      begin
        csm_l15_res_data_s3 = valid_out_s3;
      end
      else if (diag_en_s3 && rd_en_s3 && (addr_op_s3 == 2))
      begin
        csm_l15_res_data_s3 = tag_out_s3;
      end
      else
      begin
        csm_l15_res_data_s3 = data_out_s3; // TODO: mux this with valid and tag
      end
    end
    else
    begin
      // non-csm mode
      refill_req_val_s3 = 1'b0;
      csm_l15_res_val_s3 = l15_csm_req_val_s3;
      // csm_l15_res_data_s3 = {{`NOC_CHIPID_WIDTH{1'b0}},
      //                       {(`NOC_X_WIDTH-`HOME_ID_Y_POS_WIDTH){1'b0}}, lhid_s3[`HOME_ID_Y_POS],
      //                       {(`NOC_X_WIDTH-`HOME_ID_X_POS_WIDTH){1'b0}}, lhid_s3[`HOME_ID_X_POS]};
      csm_l15_res_data_s3 = 0;
      csm_l15_res_data_s3[`PACKET_HOME_ID_CHIP_MASK] = 1'b0; // non-csm mode only has 1 chip alone
      csm_l15_res_data_s3[`PACKET_HOME_ID_Y_MASK] = lhid_s3[`HOME_ID_Y_POS];
      csm_l15_res_data_s3[`PACKET_HOME_ID_X_MASK] = lhid_s3[`HOME_ID_X_POS];
    end
end
*/


`ifndef NOMEM
always @ (posedge clk)
begin
   if (!rst_n)
   begin
      ghid_ticketed_cache_addr[0] <= 0;
ghid_ticketed_cache_addr[1] <= 0;
ghid_ticketed_cache_addr[2] <= 0;
ghid_ticketed_cache_addr[3] <= 0;
ghid_ticketed_cache_addr[4] <= 0;
ghid_ticketed_cache_addr[5] <= 0;
ghid_ticketed_cache_addr[6] <= 0;
ghid_ticketed_cache_addr[7] <= 0;

   end
   else
   begin
      if (refill_req_val_s3)
      begin
         ghid_ticketed_cache_addr[l15_csm_req_ticket_s3] <= addr_in_s3;
      end
   end
end


// read port for ghid
assign csm_l15_read_res_data = ghid_ticketed_cache[l15_csm_read_ticket];
assign csm_l15_read_res_val = ghid_ticketed_cache_val[l15_csm_read_ticket];
`endif //NOMEM

//Output buffer

reg [`PHY_ADDR_WIDTH-1:0] refill_req_addr_buf [`L15_CSM_NUM_TICKETS-1:0];
reg [`L15_CSM_NUM_TICKETS_LOG2-1:0] refill_req_ticket_buf [`L15_CSM_NUM_TICKETS-1:0];
reg [`L15_CSM_NUM_TICKETS-1:0] refill_req_val_buf;

reg [`L15_CSM_NUM_TICKETS_LOG2-1:0] refill_req_buf_rd_ptr;
reg [`L15_CSM_NUM_TICKETS_LOG2-1:0] refill_req_buf_rd_ptr_next;
reg [`L15_CSM_NUM_TICKETS_LOG2-1:0] refill_req_buf_wr_ptr;
reg [`L15_CSM_NUM_TICKETS_LOG2-1:0] refill_req_buf_wr_ptr_next;
reg [`L15_CSM_NUM_TICKETS_LOG2:0] refill_req_buf_counter;
reg [`L15_CSM_NUM_TICKETS_LOG2:0] refill_req_buf_counter_next;

always @ *
begin
    if (!rst_n)
    begin
        refill_req_buf_counter_next = 0;
    end
    else if (refill_req_val_s3 && noc1encoder_csm_req_ack)
    begin
        refill_req_buf_counter_next = refill_req_buf_counter;
    end
    else if (refill_req_val_s3)
    begin
        refill_req_buf_counter_next = refill_req_buf_counter + 1;
    end
    else if (noc1encoder_csm_req_ack)
    begin
        refill_req_buf_counter_next = refill_req_buf_counter - 1;
    end
    else
    begin
        refill_req_buf_counter_next = refill_req_buf_counter;
    end
end

always @ (posedge clk)
begin
    refill_req_buf_counter <= refill_req_buf_counter_next;
end

always @ *
begin
    if (!rst_n)
    begin
        refill_req_buf_rd_ptr_next = 0;
    end
    else if (noc1encoder_csm_req_ack)
    begin
        refill_req_buf_rd_ptr_next = refill_req_buf_rd_ptr + 1;
    end
    else
    begin
        refill_req_buf_rd_ptr_next = refill_req_buf_rd_ptr;
    end
end

always @ (posedge clk)
begin
    refill_req_buf_rd_ptr <= refill_req_buf_rd_ptr_next;
end

always @ *
begin
    if (!rst_n)
    begin
        refill_req_buf_wr_ptr_next = 0;
    end
    else if (refill_req_val_s3)
    begin
        refill_req_buf_wr_ptr_next = refill_req_buf_wr_ptr + 1;
    end
    else
    begin
        refill_req_buf_wr_ptr_next = refill_req_buf_wr_ptr;
    end
end

always @ (posedge clk)
begin
    refill_req_buf_wr_ptr <= refill_req_buf_wr_ptr_next;
end

always @ (posedge clk)
begin
   if (!rst_n)
   begin
      refill_req_addr_buf[0] <= 0;
refill_req_ticket_buf[0] <= 0;
refill_req_val_buf[0] <= 0;
refill_req_addr_buf[1] <= 0;
refill_req_ticket_buf[1] <= 0;
refill_req_val_buf[1] <= 0;
refill_req_addr_buf[2] <= 0;
refill_req_ticket_buf[2] <= 0;
refill_req_val_buf[2] <= 0;
refill_req_addr_buf[3] <= 0;
refill_req_ticket_buf[3] <= 0;
refill_req_val_buf[3] <= 0;
refill_req_addr_buf[4] <= 0;
refill_req_ticket_buf[4] <= 0;
refill_req_val_buf[4] <= 0;
refill_req_addr_buf[5] <= 0;
refill_req_ticket_buf[5] <= 0;
refill_req_val_buf[5] <= 0;
refill_req_addr_buf[6] <= 0;
refill_req_ticket_buf[6] <= 0;
refill_req_val_buf[6] <= 0;
refill_req_addr_buf[7] <= 0;
refill_req_ticket_buf[7] <= 0;
refill_req_val_buf[7] <= 0;

   end
   else
   begin
      if (refill_req_val_s3)
      begin
        refill_req_addr_buf[refill_req_buf_wr_ptr] <= {l15_hmt_base_reg, addr_in_s3[`L15_HMC_ADDR_TAG], 4'd0};
        refill_req_ticket_buf[refill_req_buf_wr_ptr] <= l15_csm_req_ticket_s3;
        refill_req_val_buf[refill_req_buf_wr_ptr] <= refill_req_val_s3;
      end
   end
end



assign csm_noc1encoder_req_val = refill_req_val_buf[refill_req_buf_rd_ptr] && (refill_req_buf_counter > 0);
assign csm_noc1encoder_req_type = `L15_NOC1_REQTYPE_LD_REQUEST;
assign csm_noc1encoder_req_mshrid = refill_req_ticket_buf[refill_req_buf_rd_ptr];
assign csm_noc1encoder_req_address = refill_req_addr_buf[refill_req_buf_rd_ptr];
assign csm_noc1encoder_req_non_cacheable = 1'b1;
assign csm_noc1encoder_req_size = `PCX_SZ_16B; // use pcx def because noc1enc translate it to packet def

/*
// calculate (or look up ghid from address)
// basic calculation derived from old model in L1.5 noc1 buffer/encoder
always @ *
begin
   // Stage 2 calculation
   if (`HOME_ID_MASK_X_ENABLE)
      dest_x_s2[`L15_CSM_GHID_XPOS_MASK] = l15_csm_req_address_s2[`HOME_ID_MASK_X];
   else
      dest_x_s2 = 0;
   if (`HOME_ID_MASK_Y_ENABLE)
      dest_y_s2[`L15_CSM_GHID_YPOS_MASK] = l15_csm_req_address_s2[`HOME_ID_MASK_Y];
   else
      dest_y_s2 = 0;
   dest_chip_s2 = 0;

   ghid_s2[`L15_CSM_GHID_XPOS_MASK] = dest_x_s2;
   ghid_s2[`L15_CSM_GHID_YPOS_MASK] = dest_y_s2;
   ghid_s2[`L15_CSM_GHID_CHIP_MASK] = dest_chip_s2;
   req_val_s3_next = l15_csm_req_val_s2 ? 1'b1 : 1'b0;

   // Stage 3 (response) calculation
   // res_val_s3 = 1'b1; // no cache miss in this model
   res_val_s3 = 1'b1; // or simulate all cache miss model
   csm_l15_res_data_s3 = ghid_s3;
   csm_l15_res_val_s3 = req_val_s3 && res_val_s3;

   // write to the ticketed storage. This would normally be written to when the module
   //  gets back the response from memory
   write_index = ticketid_s3;
   write_val = req_val_s3 && !l15_csm_stall_s3;
end

always @ (posedge clk)
begin
   if (!rst_n)
   begin
      ghid_ticketed_cache_val[0] <= 0;
ghid_ticketed_cache[0] <= 0;
ghid_ticketed_cache_val[1] <= 0;
ghid_ticketed_cache[1] <= 0;
ghid_ticketed_cache_val[2] <= 0;
ghid_ticketed_cache[2] <= 0;
ghid_ticketed_cache_val[3] <= 0;
ghid_ticketed_cache[3] <= 0;
ghid_ticketed_cache_val[4] <= 0;
ghid_ticketed_cache[4] <= 0;
ghid_ticketed_cache_val[5] <= 0;
ghid_ticketed_cache[5] <= 0;
ghid_ticketed_cache_val[6] <= 0;
ghid_ticketed_cache[6] <= 0;
ghid_ticketed_cache_val[7] <= 0;
ghid_ticketed_cache[7] <= 0;

   end
   else
   begin
      if (!l15_csm_stall_s3)
      begin
         ghid_s3 <= ghid_s2;
         ticketid_s3 <= l15_csm_req_ticket_s2;
         req_val_s3 <= req_val_s3_next;
      end

      if (write_val)
      begin
         ghid_ticketed_cache[write_index] <= ghid_s3;
      end
      // logic for valid bit is a bit more complicated
      // although it should be impossible to both write and clear a slot at once
      
      if (write_val && write_index == 0)
         ghid_ticketed_cache_val[0] <= 1'b1;
      else if (l15_csm_clear_ticket_val && l15_csm_clear_ticket == 0)
         ghid_ticketed_cache_val[0] <= 1'b0;
      

      if (write_val && write_index == 1)
         ghid_ticketed_cache_val[1] <= 1'b1;
      else if (l15_csm_clear_ticket_val && l15_csm_clear_ticket == 1)
         ghid_ticketed_cache_val[1] <= 1'b0;
      

      if (write_val && write_index == 2)
         ghid_ticketed_cache_val[2] <= 1'b1;
      else if (l15_csm_clear_ticket_val && l15_csm_clear_ticket == 2)
         ghid_ticketed_cache_val[2] <= 1'b0;
      

      if (write_val && write_index == 3)
         ghid_ticketed_cache_val[3] <= 1'b1;
      else if (l15_csm_clear_ticket_val && l15_csm_clear_ticket == 3)
         ghid_ticketed_cache_val[3] <= 1'b0;
      

      if (write_val && write_index == 4)
         ghid_ticketed_cache_val[4] <= 1'b1;
      else if (l15_csm_clear_ticket_val && l15_csm_clear_ticket == 4)
         ghid_ticketed_cache_val[4] <= 1'b0;
      

      if (write_val && write_index == 5)
         ghid_ticketed_cache_val[5] <= 1'b1;
      else if (l15_csm_clear_ticket_val && l15_csm_clear_ticket == 5)
         ghid_ticketed_cache_val[5] <= 1'b0;
      

      if (write_val && write_index == 6)
         ghid_ticketed_cache_val[6] <= 1'b1;
      else if (l15_csm_clear_ticket_val && l15_csm_clear_ticket == 6)
         ghid_ticketed_cache_val[6] <= 1'b0;
      

      if (write_val && write_index == 7)
         ghid_ticketed_cache_val[7] <= 1'b1;
      else if (l15_csm_clear_ticket_val && l15_csm_clear_ticket == 7)
         ghid_ticketed_cache_val[7] <= 1'b0;
      

   end
end

// read port for ghid
always @ *
begin
   csm_l15_read_res_data = ghid_ticketed_cache[l15_csm_read_ticket];
   csm_l15_read_res_val = ghid_ticketed_cache_val[l15_csm_read_ticket];
end
*/

`else

wire [`NOC_X_WIDTH-1:0] lhid_s3_x;
wire [`NOC_Y_WIDTH-1:0] lhid_s3_y;

// read port for ghid
assign csm_l15_read_res_data = 0;
assign csm_l15_read_res_val = 0;


always @ *
begin
    csm_l15_res_val_s3 = l15_csm_req_val_s3;
end

assign csm_noc1encoder_req_val = 0;
assign csm_noc1encoder_req_type = 0;
assign csm_noc1encoder_req_mshrid =0;
assign csm_noc1encoder_req_address =0;
assign csm_noc1encoder_req_non_cacheable = 0;
assign csm_noc1encoder_req_size = 0;

always @ *
begin
    // csm_l15_res_data_s3 = {{`NOC_CHIPID_WIDTH{1'b0}},
    //                       {(`NOC_X_WIDTH-`HOME_ID_Y_POS_WIDTH){1'b0}}, lhid_s3[`HOME_ID_Y_POS],
    //                       {(`NOC_X_WIDTH-`HOME_ID_X_POS_WIDTH){1'b0}}, lhid_s3[`HOME_ID_X_POS]};
    csm_l15_res_data_s3 = 0;
    csm_l15_res_data_s3[`PACKET_HOME_ID_CHIP_MASK] = 1'b0; // non-csm mode only has 1 chip alone
    csm_l15_res_data_s3[`PACKET_HOME_ID_Y_MASK] = lhid_s3_y;
    csm_l15_res_data_s3[`PACKET_HOME_ID_X_MASK] = lhid_s3_x;
end

flat_id_to_xy lhid_to_xy (
    .flat_id(lhid_s3[`HOME_ID_Y_POS_WIDTH+`HOME_ID_X_POS_WIDTH-1:0]),
    .x_coord(lhid_s3_x),
    .y_coord(lhid_s3_y)
    );

`endif

endmodule
