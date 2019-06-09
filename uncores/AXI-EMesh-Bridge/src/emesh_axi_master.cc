/// \file the ila example of EMESH-AXI-Bridge
///  Hongce Zhang (hongcez@princeton.edu)
///

#include "emesh_axi_master.h"


EmeshAxiMasterBridge::EmeshAxiMasterBridge()
    : // construct the model
  wmodel("EmeshAxiMasterBridge_write"),
  // global reset
  m_axi_aresetn_w (wmodel.NewBvInput("m_axi_aresetn",1)),

  // AXI -- Write address
  m_axi_awid   (wmodel.NewBvState("m_axi_awid",   M_IDW)),    // output
  m_axi_awaddr (wmodel.NewBvState("m_axi_awaddr", 32)),  // output
  m_axi_awlen  (wmodel.NewBvState("m_axi_awlen",  8)),   // output
  m_axi_awsize (wmodel.NewBvState("m_axi_awsize", 3)),  // output
  m_axi_awburst(wmodel.NewBvState("m_axi_awburst",2)), // output
  m_axi_awlock (wmodel.NewBvState("m_axi_awlock", 1)),  // output
  m_axi_awcache(wmodel.NewBvState("m_axi_awcache",4)), // output
  m_axi_awprot (wmodel.NewBvState("m_axi_awprot", 3)),  // output
  m_axi_awqos  (wmodel.NewBvState("m_axi_awqos",  4)),   // output
  m_axi_awvalid(wmodel.NewBvState("m_axi_awvalid",1)), // output
  m_axi_awready(wmodel.NewBvInput("m_axi_awready",1)),

  // AXI -- Write data
  m_axi_wid   (wmodel.NewBvState("m_axi_wid",     M_IDW)),     // output
  m_axi_wdata (wmodel.NewBvState("m_axi_wdata",   64)),   // output
  m_axi_wstrb (wmodel.NewBvState("m_axi_wstrb",   8)),   // output
  m_axi_wlast (wmodel.NewBvState("m_axi_wlast",   1)),   // output
  m_axi_wvalid(wmodel.NewBvState("m_axi_wvalid",  1)),  // output
  m_axi_wready(wmodel.NewBvInput("m_axi_wready",  1)),  

  // AXI -- Write response
  m_axi_bid   (wmodel.NewBvInput("m_axi_bid",    M_IDW)),     
  m_axi_bresp (wmodel.NewBvInput("m_axi_bresp",  2)),   
  m_axi_bvalid(wmodel.NewBvInput("m_axi_bvalid", 1)),  
  m_axi_bready(wmodel.NewBvState("m_axi_bready", 1)),  // output

  m_axi_aresetn_r (rmodel.NewBvInput("m_axi_aresetn",1)),

  // internal states -- may not have matches with the Verilog state
  // but necessary for modeling
  tx_valid(wmodel.NewBvState("tx_valid", 1)),
  tx_addr_done(wmodel.NewBvState("tx_addr_done", 1)),
  tx_data_done(wmodel.NewBvState("tx_data_done", 1)),
  tx_burst(wmodel.NewBvState("tx_burst", 2)),
  tx_id   (wmodel.NewBvState("tx_id"   , M_IDW)),
  tx_addr (wmodel.NewBvState("tx_addr" , 32)),
  tx_data (wmodel.NewBvState("tx_data" , 64)),
  tx_len  (wmodel.NewBvState("tx_len"  , 8)),
  tx_size (wmodel.NewBvState("tx_size" , 3)),
  tx_count(wmodel.NewBvState("tx_count", 8)),

  // ------------------------------------------------------------------
  
  rmodel("EmeshAxiMasterBridge_read"),
  // AXI -- Read address
  m_axi_arid   (rmodel.NewBvState("m_axi_arid",    M_IDW)),    // output
  m_axi_araddr (rmodel.NewBvState("m_axi_araddr",  32)),  // output
  m_axi_arlen  (rmodel.NewBvState("m_axi_arlen",   8)),   // output
  m_axi_arsize (rmodel.NewBvState("m_axi_arsize",  3)),  // output
  m_axi_arburst(rmodel.NewBvState("m_axi_arburst", 2)), // output
  m_axi_arlock (rmodel.NewBvState("m_axi_arlock",  1)),  // output
  m_axi_arcache(rmodel.NewBvState("m_axi_arcache", 4)), // output
  m_axi_arprot (rmodel.NewBvState("m_axi_arprot",  3)),  // output
  m_axi_arqos  (rmodel.NewBvState("m_axi_arqos",   4)),   // output
  m_axi_arvalid(rmodel.NewBvState("m_axi_arvalid", 1)), // output
  m_axi_arready(rmodel.NewBvInput("m_axi_arready", 1)),

  // AXI -- Read data
  m_axi_rid   (rmodel.NewBvInput("m_axi_rid",    M_IDW)),     
  m_axi_rdata (rmodel.NewBvInput("m_axi_rdata",  64)),   
  m_axi_rresp (rmodel.NewBvInput("m_axi_rresp",  2)),   
  m_axi_rlast (rmodel.NewBvInput("m_axi_rlast",  1)),   
  m_axi_rvalid(rmodel.NewBvInput("m_axi_rvalid", 1)),  
  m_axi_rready(rmodel.NewBvState("m_axi_rready", 1))  // output
{

  // write data buffers

  // Write channel interface -- what corresponds to instruction
  wmodel.SetFetch( lConcat({m_axi_aresetn_w, m_axi_awready, m_axi_wready}) );
  // Valid instruction: what means to have valid command (valid = 1)
  wmodel.SetValid( /*always true*/ BoolConst(true) );

  { // reset instruction
    auto instr = wmodel.NewInstr("WReset");
    instr.SetDecode( m_axi_aresetn_w == 0 );
    instr.SetUpdate(m_axi_awvalid, BvConst(0,1));
    instr.SetUpdate(m_axi_wvalid,  BvConst(0,1));

    // instr.SetUpdate(tx_valid, BvConst(0,1)); this is not guaranteed
    // ready signals not specified and thus won't be checked
  }


  { // AXIWriteAddrReady instruction
    auto instr = wmodel.NewInstr("AXIWriteAddrReady");

    instr.SetDecode( ( m_axi_awready == 1 ) & ( m_axi_aresetn_w == 1 ) ); // will get what's in its buffer
    // if ( m_axi_awvalid == 1 ), the a transaction is completed, no value will protentially loaded
    // you can check it by providing where the next transaction comes
    // ow. we don't make any guarantee
    // how much you regard as the spec, how much we check
    // you need to differentiate the spec of AXI from the spec of this Emesh-AXI-bridge

    /* This is one way of treating the spec:
    auto finish_transaction = ( m_axi_awready == 1 ) & (m_axi_awvalid == 1); 
    instr.SetUpdate(m_axi_awvalid, Ite(finish_transaction, unknownVal(1),  unknownVal(1) ));
    instr.SetUpdate(m_axi_awaddr,  Ite(finish_transaction, unknownVal(32), unknownVal(32)));
    instr.SetUpdate(m_axi_awlen,   Ite(finish_transaction, unknownVal(8),  unknownVal(8) ));
    instr.SetUpdate(m_axi_awsize,  Ite(finish_transaction, unknownVal(3),  unknownVal(3) ));
    */

    
    // a general spec
    instr.SetUpdate(m_axi_awvalid, Ite(m_axi_awvalid == 1, tx_valid & ~tx_addr_done, unknownVal(1)) );
    instr.SetUpdate(m_axi_awid,    Ite(m_axi_awvalid & tx_valid & ~tx_addr_done == 1, tx_id,   unknownVal(M_IDW)));
    instr.SetUpdate(m_axi_awaddr,  Ite(m_axi_awvalid & tx_valid & ~tx_addr_done == 1, tx_addr, unknownVal(32)));
    instr.SetUpdate(m_axi_awlen,   Ite(m_axi_awvalid & tx_valid & ~tx_addr_done == 1, tx_len,  unknownVal(8)));
    instr.SetUpdate(m_axi_awsize,  Ite(m_axi_awvalid & tx_valid & ~tx_addr_done == 1, tx_size, unknownVal(3)));
    instr.SetUpdate(m_axi_awburst, Ite(m_axi_awvalid & tx_valid & ~tx_addr_done == 1, tx_burst,unknownVal(2)));
  }

  { // AXIWriteAddrNotReady instruction
    auto instr = wmodel.NewInstr("AXIWriteAddrNotReady"); // then it should keep the old value

    instr.SetDecode( ( m_axi_awready == 0 ) & ( m_axi_aresetn_w == 1 ) ); // should keep its old value
    // if it is valid, you should keep it
    // if it is not valid, there must be a chance that m_axi_awvalid can be 1 (does not wait on awready)
    instr.SetUpdate(m_axi_awvalid,Ite(m_axi_awvalid == 1, m_axi_awvalid, Ite(unknownVal(1) == 1, BvConst(1,1) , unknownVal(1) ) ));
    instr.SetUpdate(m_axi_awid,   Ite(m_axi_awvalid == 1, m_axi_awid,    unknownVal(M_IDW)) );
    instr.SetUpdate(m_axi_awaddr, Ite(m_axi_awvalid == 1, m_axi_awaddr,  unknownVal(32) ));
    instr.SetUpdate(m_axi_awlen,  Ite(m_axi_awvalid == 1, m_axi_awlen,   unknownVal(8) ));
    instr.SetUpdate(m_axi_awsize, Ite(m_axi_awvalid == 1, m_axi_awsize,  unknownVal(3) ));
  }

  {
    // AXIWriteAddrNotReady instruction
    auto instr = wmodel.NewInstr("AXIWriteDataReady"); // then it should keep the old value

    instr.SetDecode( ( m_axi_wready == 1 ) & ( m_axi_aresetn_w == 1 ) ); 

    auto tx_size_nbyte = BvConst(1,32) << ZExt( tx_size - 1 , 32);
    // the start address tx_addr is aligned to the size of transfer
    // the length of the burst is 2,4,8, or 16
    auto wrap_shr = Ite(tx_len == 1, BvConst(1,32),
                    Ite(tx_len == 3, BvConst(2,32),
                    Ite(tx_len == 7, BvConst(3,32),
                    Ite(tx_len == 15, BvConst(4,32), unknownVal(32) ))));
    auto total_bytes = BvConst(1,32) << ( wrap_shr + ZExt( tx_size - 1 , 32) );
    auto aligned_burst_address = ( tx_addr >> (ZExt(tx_size,32) + wrap_shr) ) << (ZExt(tx_size,32) + wrap_shr);
    auto current_addr = Ite(tx_burst == BURST_FIXED, tx_addr,
                        Ite(tx_burst == BURST_INCR,  tx_addr + tx_size_nbyte * ZExt(tx_count,32) ,
                        Ite(tx_burst == BURST_WRAP,  
                          Ite( tx_addr + tx_size_nbyte * ZExt(tx_count,32) >= aligned_burst_address + total_bytes, 
                               tx_addr + tx_size_nbyte * ZExt(tx_count,32) - total_bytes, 
                               tx_addr + tx_size_nbyte * ZExt(tx_count,32) ),
                          unknownVal(32))));

    auto strb = Ite( tx_size == 0, BvConst(0x01,8) << ZExt(current_addr(2,0),8), // 1byte
                Ite( tx_size == 1, BvConst(0x03,8) << ZExt(Concat(current_addr(2,1), BvConst(0,1)),8), // 2bytes
                Ite( tx_size == 2, BvConst(0x0f,8) << ZExt(Concat(current_addr(2,2), BvConst(0,2)),8), // 4bytes
                Ite( tx_size == 3, BvConst(0xff,8) , unknownVal(8)  // 8bytes
                 ))));

    instr.SetUpdate( m_axi_wvalid, Ite( m_axi_wvalid == 1, tx_valid & ~tx_data_done, unknownVal(1) ) );
    instr.SetUpdate( m_axi_wid,    Ite( m_axi_wvalid & tx_valid & ~tx_data_done == 1, tx_id, unknownVal(M_IDW)) );
    instr.SetUpdate( m_axi_wdata,  Ite( m_axi_wvalid & tx_valid & ~tx_data_done == 1, tx_data, unknownVal(64) ) );
    instr.SetUpdate( m_axi_wstrb,  Ite( m_axi_wvalid & tx_valid & ~tx_data_done == 1, strb, unknownVal(8) ) );
    instr.SetUpdate( m_axi_wlast,  Ite( m_axi_wvalid & tx_valid & ~tx_data_done == 1, Ite( tx_count == tx_len, BvConst(1,1), BvConst(0,1)), unknownVal(1) ) );

    instr.SetUpdate( tx_count,     Ite( m_axi_wvalid & tx_valid & ~tx_data_done == 1, tx_count + 1, BvConst(0, 8) ));
  }


  {
    // AXIWriteAddrNotReady instruction
    auto instr = wmodel.NewInstr("AXIWriteDataNotReady"); // then it should keep the old value

    instr.SetDecode( ( m_axi_wready == 0 ) & ( m_axi_aresetn_w == 1 ) ); // should keep its old value
    // if it is valid, you should keep it
    // if it is not valid, there must be a chance that m_axi_awvalid can be 1 (does not wait on awready)

    instr.SetUpdate( m_axi_wid,    Ite(m_axi_wvalid == 1, m_axi_wid,   unknownVal(M_IDW)));
    instr.SetUpdate( m_axi_wdata,  Ite(m_axi_wvalid == 1, m_axi_wdata, unknownVal(64)));
    instr.SetUpdate( m_axi_wstrb,  Ite(m_axi_wvalid == 1, m_axi_wstrb, unknownVal(8)));
    instr.SetUpdate( m_axi_wlast,  Ite(m_axi_wvalid == 1, m_axi_wlast, unknownVal(1)));
    instr.SetUpdate( m_axi_wvalid, Ite(m_axi_wvalid == 1, m_axi_wvalid, Ite( unknownVal(1) == 1, BvConst(1,1), unknownVal(1))));
  }

  {
    auto instr = wmodel.NewInstr("AXIWriteAcknowlege");

    instr.SetDecode( ( m_axi_bvalid == 1 ) & ( m_axi_aresetn_w == 1 ) );
    // if b_ready
  }

  // ----------------------------------------------------------------------------

  // Write channel interface -- what corresponds to instruction
  rmodel.SetFetch( lConcat({m_axi_aresetn_r, m_axi_arready, m_axi_rvalid }) );
  // Valid instruction: what means to have valid command (valid = 1)
  rmodel.SetValid( /*always true*/ BoolConst(true) );

  {// reset instruction
    auto instr = rmodel.NewInstr("RReset");
    instr.SetDecode( m_axi_aresetn_r == 0 );
    instr.SetUpdate(m_axi_arvalid, BvConst(0,1));
    instr.SetUpdate(m_axi_rvalid,  BvConst(0,1));
  }

  { //  
    auto instr = rmodel.NewInstr("AXIReadAddrReady");
    instr.SetDecode( (m_axi_aresetn_r == 1) & (m_axi_arready == 1) );
    // the bridge spec should specify the relationship
  }

  { //  
    auto instr = rmodel.NewInstr("AXIReadAddrNotReady");
    instr.SetDecode( (m_axi_aresetn_r == 1) & (m_axi_arready == 0) );
    // if arvalid is 1, it should hold its status
    instr.SetUpdate(m_axi_arid   , Ite(m_axi_arvalid == 1, m_axi_arid   ,unknownVal(M_IDW)));
    instr.SetUpdate(m_axi_araddr , Ite(m_axi_arvalid == 1, m_axi_araddr ,unknownVal(32)));
    instr.SetUpdate(m_axi_arlen  , Ite(m_axi_arvalid == 1, m_axi_arlen  ,unknownVal(8)));
    instr.SetUpdate(m_axi_arsize , Ite(m_axi_arvalid == 1, m_axi_arsize ,unknownVal(3)));
    instr.SetUpdate(m_axi_arburst, Ite(m_axi_arvalid == 1, m_axi_arburst,unknownVal(2)));
    instr.SetUpdate(m_axi_arvalid, Ite(m_axi_arvalid == 1, m_axi_arvalid,unknownVal(1)));
  } 

  // AXI spec has no requirement for rready actually
  {
    auto instr = rmodel.NewInstr("AXIReadDataNotValid");
    instr.SetDecode( (m_axi_aresetn_r == 1) & (m_axi_rvalid == 0) );
  }

  {
    auto instr = rmodel.NewInstr("AXIReadDataValid");
    instr.SetDecode( (m_axi_aresetn_r == 1) & (m_axi_rvalid == 1) );
  }

}

/*

  wr_access(wmodel.NewBvInput("wr_access", 1) ),
  wr_packet(wmodel.NewBvInput("wr_packet",PW) ),
  wr_wait  (wmodel.NewBvState("wr_wait",   1) ),   // output

  rd_access(rmodel.NewBvInput("rd_access", 1) ),
  rd_packet(rmodel.NewBvInput("rd_packet",PW) ),
  rd_wait  (rmodel.NewBvState("rd_wait",   1) ),   // output

  rr_access(rmodel.NewBvState("rr_access",  1) ),  // output
  rr_packet(rmodel.NewBvState("rr_packet", PW) ), // output
  rr_wait  (rmodel.NewBvInput("rr_wait",    1) ),   

*/