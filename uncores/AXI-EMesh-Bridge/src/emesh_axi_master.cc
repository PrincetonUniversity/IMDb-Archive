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
  m_axi_rready(rmodel.NewBvState("m_axi_rready", 1))  // output
      
    {

  // Write channel interface -- what corresponds to instruction
  wmodel.SetFetch( lConcat({wr_access, wr_packet, m_axi_awready , m_axi_wready})   );
  // Valid instruction: what means to have valid command (valid = 1)
  wmodel.SetValid( ( wr_access == 1 ) | ( m_axi_awready == 1 ) | ( m_axi_wready == 1 ) );

  { // add instruction
    auto instr = wmodel.NewInstr("WRITE_buf");

    instr.SetDecode( ( wr_access == 1 ) & ( wr_wait == 0 ) );

    // will go into its buffer and set the m_axi_awvalid & m_axi_wvalid

    // will go into the output if the buffer is empty now?
    // wr_wait can be arbitrary -- no requirement ? unless we model the exact size of the buffer
  }

  { // add instruction
    auto instr = wmodel.NewInstr("WriteAddrReady");

    instr.SetDecode( ( m_axi_awready == 1 ) & ( m_axi_awvalid == 1 ) ); // will get what's in its buffer

  }

  { // add instruction
    auto instr = wmodel.NewInstr("WriteDataReady");

    instr.SetDecode( ( m_axi_wready == 1 ) & ( m_axi_wvalid == 1 ) ); // will get what's in its buffer
    // also true if the buffer is empty

  }


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