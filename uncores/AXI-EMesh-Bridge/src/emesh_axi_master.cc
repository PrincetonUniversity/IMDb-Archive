/// \file the ila example of EMESH-AXI-Bridge
///  Hongce Zhang (hongcez@princeton.edu)
///

#include "emesh_axi_master.h"


EmeshAxiMasterBridge::EmeshAxiMasterBridge()
    : // construct the model
      wmodel("EmeshAxiMasterBridge_write"),
      rmodel("EmeshAxiMasterBridge_read"),

  wr_access(wmodel.NewBvInput("wr_access", 1) ),
  wr_packet(wmodel.NewBvInput("wr_packet",PW) ),
  wr_wait  (wmodel.NewBvState("wr_wait",   1) ),   // output

  rd_access(rmodel.NewBvInput("rd_access", 1) ),
  rd_packet(rmodel.NewBvInput("rd_packet",PW) ),
  rd_wait  (rmodel.NewBvState("rd_wait",   1) ),   // output

  rr_access(rmodel.NewBvState("rr_access",  1) ),  // output
  rr_packet(rmodel.NewBvState("rr_packet", PW) ), // output
  rr_wait  (rmodel.NewBvInput("rr_wait",    1) ),   

  // global reset
  m_axi_aresetn_r (rmodel.NewBvInput("m_axi_aresetn",1)),
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
  m_axi_bid   (rmodel.NewBvInput("m_axi_bid",    M_IDW)),     
  m_axi_bresp (rmodel.NewBvInput("m_axi_bresp",  2)),   
  m_axi_bvalid(rmodel.NewBvInput("m_axi_bvalid", 1)),  
  m_axi_bready(rmodel.NewBvState("m_axi_bready", 1)),  // output

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
  m_axi_rready(rmodel.NewBvState("m_axi_rready", 1)),  // output
      

  // -------------------- BELOW ARE NOT AXI SPEC ----------------- //
  // -------------------- BUT SPEC FOR THIS BRIDGE --------------- //
  // internal states -- may not have matches with the Verilog state
  tx_valid(wmodel.NewBvState("tx_valid", 1)),
  tx_burst(wmodel.NewBvState("tx_burst", 2)),
  tx_id   (wmodel.NewBvState("tx_id"   , M_IDW)),
  tx_addr (wmodel.NewBvState("tx_addr" , 32)),
  tx_data (wmodel.NewBvState("tx_data" , 64)),
  tx_len  (wmodel.NewBvState("tx_len"  , 8)),
  tx_size (wmodel.NewBvState("tx_size" , 3)),
  tx_count(wmodel.NewBvState("tx_count", 8))

{

  // write data buffers

  // Write channel interface -- what corresponds to instruction
  wmodel.SetFetch( lConcat({m_axi_aresetn_w, m_axi_awready, m_axi_wready}) );
  // Valid instruction: what means to have valid command (valid = 1)
  wmodel.SetValid( /*always true*/ BoolConst(true) );

  { // reset instruction
    auto inst = wmodel.NewInstr("Reset");
    inst.SetDecode( m_axi_aresetn_w == 0 );
    inst.SetUpdate(m_axi_awvalid, BvConst(0,1));
    inst.SetUpdate(m_axi_wvalid,  BvConst(0,1));

    inst.SetUpdate(tx_valid, BvConst(0,1));
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
    inst.SetUpdate(m_axi_awvalid, Ite(finish_transaction, unknown(1),  unknown(1) ));
    inst.SetUpdate(m_axi_awaddr,  Ite(finish_transaction, unknown(32), unknown(32)));
    inst.SetUpdate(m_axi_awlen,   Ite(finish_transaction, unknown(8),  unknown(8) ));
    inst.SetUpdate(m_axi_awsize,  Ite(finish_transaction, unknown(3),  unknown(3) ));
    */

    
    // a general spec
    inst.SetUpdate(m_axi_awvalid, Ite(m_axi_awvalid, tx_valid, unknown(1)) );
    inst.SetUpdate(m_axi_awid,    Ite(m_axi_awvalid & tx_valid, tx_id,   unknown(M_IDW)));
    inst.SetUpdate(m_axi_awaddr,  Ite(m_axi_awvalid & tx_valid, tx_addr, unknown(32)));
    inst.SetUpdate(m_axi_awlen,   Ite(m_axi_awvalid & tx_valid, tx_len,  unknown(8)));
    inst.SetUpdate(m_axi_awsize,  Ite(m_axi_awvalid & tx_valid, tx_size, unknown(3)));
    inst.SetUpdate(m_axi_awburst, Ite(m_axi_awvalid & tx_valid, tx_burst,unknown(2)));
  }

  { // AXIWriteAddrNotReady instruction
    auto instr = wmodel.NewInstr("AXIWriteAddrNotReady"); // then it should keep the old value

    instr.SetDecode( ( m_axi_awready == 0 ) & ( m_axi_aresetn_w == 1 ) ); // should keep its old value
    // if it is valid, you should keep it
    // if it is not valid, there must be a chance that m_axi_awvalid can be 1 (does not wait on awready)
    inst.SetUpdate(m_axi_awvalid,Ite(m_axi_awvalid, m_axi_awvalid, Ite(unknown(1) == 1, BvConst(1,1) , unknown(1) ) ));
    inst.SetUpdate(m_axi_awid,   Ite(m_axi_awvalid, m_axi_awid,    unknown(M_IDW)) );
    inst.SetUpdate(m_axi_awaddr, Ite(m_axi_awvalid, m_axi_awaddr,  unknown(32) ));
    inst.SetUpdate(m_axi_awlen,  Ite(m_axi_awvalid, m_axi_awlen,   unknown(8) ));
    inst.SetUpdate(m_axi_awsize, Ite(m_axi_awvalid, m_axi_awsize,  unknown(3) ));
  }

  {
    // AXIWriteAddrNotReady instruction
    auto instr = wmodel.NewInstr("AXIWriteDataReady"); // then it should keep the old value

    instr.SetDecode( ( m_axi_wready == 1 ) & ( m_axi_aresetn_w == 1 ) ); 
    instr.SetUpdate( ( m_axi_wvalid == ) );

  }


  {
    // AXIWriteAddrNotReady instruction
    auto instr = wmodel.NewInstr("AXIWriteDataNotReady"); // then it should keep the old value

    instr.SetDecode( ( m_axi_wready == 0 ) & ( m_axi_aresetn_w == 1 ) ); // should keep its old value
    // if it is valid, you should keep it
    // if it is not valid, there must be a chance that m_axi_awvalid can be 1 (does not wait on awready)

    instr.SetUpdate( m_axi_wid,    Ite(m_axi_wvalid, m_axi_wid,   unknown(M_IDW)));
    instr.SetUpdate( m_axi_wdata,  Ite(m_axi_wvalid, m_axi_wdata, unknown(64)));
    instr.SetUpdate( m_axi_wstrb,  Ite(m_axi_wvalid, m_axi_wstrb, unknown(8)));
    instr.SetUpdate( m_axi_wlast,  Ite(m_axi_wvalid, m_axi_wlast, unknown(1)));
    instr.SetUpdate( m_axi_wvalid, Ite(m_axi_wvalid, m_axi_wvalid,unknown(1)));
  }

  {
    auto instr = wmodel.NewInst("AXIWriteAcknowlege");

    instr.SetDecode( ( m_axi_bvalid == 1 ) & ( m_axi_aresetn_w == 1 ) );
    // if b_ready
  }





  // Write channel interface -- what corresponds to instruction
  rmodel.SetFetch( lConcat({m_axi_aresetn_r, m_axi_arready, m_axi_rvalid }) );
  // Valid instruction: what means to have valid command (valid = 1)
  rmodel.SetValid( /*always true*/ BoolConst(true) );

  {// reset instruction
    auto inst = rmodel.NewInstr("Reset");
    inst.SetDecode( m_axi_aresetn_r == 0 );
    inst.SetUpdate(m_axi_arvalid, BvConst(0,1));
    inst.SetUpdate(m_axi_rvalid,  BvConst(0,1));
  }

  { //  
    auto inst = rmodel.NewInstr("AXIReadAddrReady");
    inst.SetDecode( (m_axi_aresetn_r == 1) & (m_axi_arready == 1) );
    // not much spec required here

  }

  { //  
    auto inst = rmodel.NewInstr("AXIReadAddrNotReady");
    inst.SetDecode( (m_axi_aresetn_r == 1) & (m_axi_arready == 0) );

    // if arvalid is 1, it should hold its status
  }

  // AXI spec has no requirement for rready actually

}


/*

    auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map
    auto DATA_cache = Map( "address_to_data_map", 64, address ); // Use the map

    auto hit = MESI_state != MESI_INVALID;

    // on miss : send out noc1 request eventually

    instr.SetUpdate(l15_noc1buffer_req_address,      Ite(! hit, address,       unknown(40)() ) );
    instr.SetUpdate(l15_noc1buffer_req_noncacheable, Ite(! hit, BvConst(0,1) , unknown(1)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_size,         Ite(! hit, size ,         unknown(3)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_type,         Ite(! hit, BvConst(2,5) , unknown(5)()  ) );

    // on the hit side : return the data on cpx

    instr.SetUpdate( l15_transducer_val,             Ite( hit , BvConst(1,1), unknown(1)() ) );
    instr.SetUpdate( l15_transducer_returntype,      Ite( hit , BvConst(0,4) , unknown(4)() ) );
    instr.SetUpdate( l15_transducer_data_0,          Ite( hit , DATA_cache , unknown(64)()  ) );
*/