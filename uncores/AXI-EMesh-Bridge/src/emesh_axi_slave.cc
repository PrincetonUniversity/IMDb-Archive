/// \file the ila example of EMESH-AXI-Bridge
///  Huaixi Lu (huaixil@princeton.edu)
///

#include "emesh_axi_slave.h"


EmeshAxiSlaveBridge::EmeshAxiSlaveBridge()
    : // construct the model
  wmodel("EmeshAxiSlaveBridge_write"),
  // global reset
  s_axi_aresetn_w (wmodel.NewBvInput("s_axi_aresetn",1)),

  // AXI -- Write address
  s_axi_awid   (wmodel.NewBvInput("s_axi_awid",   S_IDW)),   
  s_axi_awaddr (wmodel.NewBvInput("s_axi_awaddr", 32)),
  s_axi_awlen  (wmodel.NewBvInput("s_axi_awlen",  8)),
  s_axi_awsize (wmodel.NewBvInput("s_axi_awsize", 3)),
  s_axi_awburst(wmodel.NewBvInput("s_axi_awburst",2)),
  s_axi_awlock (wmodel.NewBvInput("s_axi_awlock", 1)),
  s_axi_awcache(wmodel.NewBvInput("s_axi_awcache",4)),
  s_axi_awprot (wmodel.NewBvInput("s_axi_awprot", 3)),
  s_axi_awqos  (wmodel.NewBvInput("s_axi_awqos",  4)),
  s_axi_awvalid(wmodel.NewBvInput("s_axi_awvalid",1)),
  s_axi_awready(wmodel.NewBvState("s_axi_awready",1)), //output

  // AXI -- Write data
  s_axi_wid   (wmodel.NewBvInput("s_axi_wid",     S_IDW)),  
  s_axi_wdata (wmodel.NewBvInput("s_axi_wdata",   32)), 
  s_axi_wstrb (wmodel.NewBvInput("s_axi_wstrb",   4)), 
  s_axi_wlast (wmodel.NewBvInput("s_axi_wlast",   1)), 
  s_axi_wvalid(wmodel.NewBvInput("s_axi_wvalid",  1)),
  s_axi_wready(wmodel.NewBvState("s_axi_wready",  1)), //output

  // AXI -- Write response
  s_axi_bid   (wmodel.NewBvState("s_axi_bid",    S_IDW)), // output    
  s_axi_bresp (wmodel.NewBvState("s_axi_bresp",  2)),     // output 
  s_axi_bvalid(wmodel.NewBvState("s_axi_bvalid", 1)),     // output
  s_axi_bready(wmodel.NewBvInput("s_axi_bready", 1)), 

  rmodel("EmeshAxiSlaveBridge_read"),
  
  s_axi_aresetn_r (rmodel.NewBvInput("s_axi_aresetn",1)),
  // AXI -- Read address
  s_axi_arid   (rmodel.NewBvInput("s_axi_arid",    S_IDW)),
  s_axi_araddr (rmodel.NewBvInput("s_axi_araddr",  32)), 
  s_axi_arlen  (rmodel.NewBvInput("s_axi_arlen",   8)),
  s_axi_arsize (rmodel.NewBvInput("s_axi_arsize",  3)), 
  s_axi_arburst(rmodel.NewBvInput("s_axi_arburst", 2)), 
  s_axi_arlock (rmodel.NewBvInput("s_axi_arlock",  1)),  
  s_axi_arcache(rmodel.NewBvInput("s_axi_arcache", 4)), 
  s_axi_arprot (rmodel.NewBvInput("s_axi_arprot",  3)), 
  s_axi_arqos  (rmodel.NewBvInput("s_axi_arqos",   4)), 
  s_axi_arvalid(rmodel.NewBvInput("s_axi_arvalid", 1)),
  s_axi_arready(rmodel.NewBvState("s_axi_arready", 1)), //output

  // AXI -- Read data
  s_axi_rid   (rmodel.NewBvState("s_axi_rid",    S_IDW)), //output     
  s_axi_rdata (rmodel.NewBvState("s_axi_rdata",  32)),   //output
  s_axi_rresp (rmodel.NewBvState("s_axi_rresp",  2)),   //output
  s_axi_rlast (rmodel.NewBvState("s_axi_rlast",  1)),   //output
  s_axi_rvalid(rmodel.NewBvState("s_axi_rvalid", 1)),   //output
  s_axi_rready(rmodel.NewBvInput("s_axi_rready", 1)),
  
  // internal states -- may not have matches with the Verilog state
  // but necessary for modeling
  tx_wactive(wmodel.NewBvState("tx_wactive", 1)), // write_wactive
  tx_bwait(wmodel.NewBvState("tx_bwait", 1)), // b_wait
  tx_ractive(rmodel.NewBvState("tx_ractive", 1)), // read_wactive

  tx_len (rmodel.NewBvState("tx_len", 8)), // axi_arlen
  tx_arsize(rmodel.NewBvState("tx_arsize", 3))

  // ------------------------------------------------------------------
{

  // write data buffers

  // Write channel interface -- what corresponds to instruction
  wmodel.SetFetch( lConcat({s_axi_aresetn_w, s_axi_awvalid, s_axi_wvalid}) );
  // Valid instruction: what means to have valid command (valid = 1)
  wmodel.SetValid( /*always true*/ BoolConst(true) );

  { // reset instruction
    auto instr = wmodel.NewInstr("WReset");
    instr.SetDecode(s_axi_aresetn_w == 0 );
    
    // Write addr
    instr.SetUpdate(s_axi_awready, BvConst(1,1)); // default state recommends to be high
    instr.SetUpdate(tx_wactive, BvConst(0,1));
    instr.SetUpdate(s_axi_bid, BvConst(0,S_IDW));
    
    // Write data
    instr.SetUpdate(s_axi_wready, BvConst(0,1));
    instr.SetUpdate(s_axi_bvalid, BvConst(0,1));
    instr.SetUpdate(s_axi_bresp, BvConst(0,2));
    instr.SetUpdate(tx_bwait, BvConst(0,1));

  }


  { // AXIWriteAddrValid instruction
    auto instr = wmodel.NewInstr("AXIWriteAddrValid");

    instr.SetDecode( (s_axi_awvalid == 1) & (s_axi_aresetn_w == 1) ); // will get what's in its buffer

    instr.SetUpdate(s_axi_awready, Ite(~s_axi_awready & ~tx_wactive & ~tx_bwait, BvConst(1,1), BvConst(0,1)) );
    instr.SetUpdate(tx_wactive, Ite(s_axi_awready, BvConst(1,1), Ite(s_axi_wready & s_axi_wlast, BvConst(0,1), tx_wactive)) );
    
    instr.SetUpdate(s_axi_bid,  Ite(s_axi_awready, s_axi_awid, s_axi_bid) );
  }

  { // AXIWriteAddrNotValid instruction
    auto instr = wmodel.NewInstr("AXIWriteAddrNotValid"); 

    instr.SetDecode( ( s_axi_awvalid == 0 ) & ( s_axi_aresetn_w == 1 ) ); // should keep its old value
    
    instr.SetUpdate(s_axi_awready, Ite(~s_axi_awready & ~tx_wactive & ~tx_bwait, BvConst(1,1), unknownVal(1)));
    instr.SetUpdate(tx_wactive, Ite(s_axi_wready & s_axi_wvalid & s_axi_wlast, BvConst(0,1), unknownVal(1)) );
  }

  {
    // AXIWriteDataValid instruction
    auto instr = wmodel.NewInstr("AXIWriteDataValid"); 

    instr.SetDecode( ( s_axi_wvalid == 1 ) & ( s_axi_aresetn_w == 1 ) );

    instr.SetUpdate(s_axi_wready, Ite(s_axi_wready & s_axi_wlast, BvConst(0,1), Ite(tx_wactive, unknownVal(1), s_axi_wready)));
    instr.SetUpdate(s_axi_bvalid, Ite(s_axi_wready & s_axi_wlast, BvConst(1,1), Ite(s_axi_bready & s_axi_bvalid, BvConst(0,1), s_axi_wready)));
    instr.SetUpdate(s_axi_bresp, Ite(s_axi_wready & s_axi_wlast, BvConst(0,2), s_axi_bresp));
    instr.SetUpdate(tx_bwait, Ite(s_axi_wready & s_axi_wlast, ~s_axi_bready, Ite(s_axi_bready & s_axi_bvalid, BvConst(0,1), tx_bwait)));
  }


  {
    // AXIWriteDataNotValid instruction
    auto instr = wmodel.NewInstr("AXIWriteDataNotValid");

    instr.SetDecode( ( s_axi_wvalid == 0 ) & ( s_axi_aresetn_w == 1 ) );

    instr.SetUpdate(s_axi_wready, Ite(tx_wactive, unknownVal(1), s_axi_wready));
    instr.SetUpdate(s_axi_bvalid, Ite(s_axi_bready & s_axi_bvalid, BvConst(0,1), s_axi_wready));
    instr.SetUpdate(tx_bwait, Ite(s_axi_bready & s_axi_bvalid, BvConst(0,1), tx_bwait));
  }

  {
    auto instr = wmodel.NewInstr("AXIWriteAcknowlege");
    instr.SetDecode( ( s_axi_bvalid == 1 ) & ( s_axi_aresetn_w == 1 ) );
  }

  // ---------------------------------------------------------------------------- //
  // ---------------------------------------------------------------------------- //

  // Read channel interface -- what corresponds to instruction
  rmodel.SetFetch( lConcat({s_axi_aresetn_r, s_axi_arvalid, s_axi_rready }) );
  // Valid instruction: what means to have valid command (valid = 1)
  rmodel.SetValid( /*always true*/ BoolConst(true) );

  {// reset instruction
    auto instr = rmodel.NewInstr("RReset");
    instr.SetDecode( s_axi_aresetn_r == 0 );
    
    // AR
    instr.SetUpdate(s_axi_arready, BvConst(0,1));
    instr.SetUpdate(tx_ractive, BvConst(0,1));
    instr.SetUpdate(tx_len, BvConst(0,8));
    instr.SetUpdate(tx_arsize, BvConst(0,3));
    instr.SetUpdate(s_axi_rlast, BvConst(0,1));
    instr.SetUpdate(s_axi_rid, BvConst(0,S_IDW));

    // Read Resp
    instr.SetUpdate(s_axi_rvalid,  BvConst(0,1));
    instr.SetUpdate(s_axi_rdata, BvConst(0,32));
    instr.SetUpdate(s_axi_rresp, BvConst(0,2));
  }

  { // AR Valid  
    auto instr = rmodel.NewInstr("AXIReadAddrValid");
    instr.SetDecode( (s_axi_aresetn_r == 1) & (s_axi_arvalid == 1) );

    instr.SetUpdate(s_axi_arready, Ite(~s_axi_arready & ~tx_ractive, BvConst(1,1), BvConst(0,1)));
    instr.SetUpdate(tx_ractive, Ite(s_axi_arready, BvConst(1,1), Ite(s_axi_rvalid & s_axi_rlast & s_axi_rready, BvConst(0,1), tx_ractive)));
    instr.SetUpdate(tx_len, Ite(s_axi_arready, s_axi_arlen, Ite(s_axi_rvalid & s_axi_rready, tx_len - BvConst(1,8), tx_len)));
    instr.SetUpdate(tx_arsize, Ite(s_axi_arready, s_axi_arsize, tx_arsize));
    instr.SetUpdate(s_axi_rlast,  Ite(s_axi_arready, Ite(s_axi_arlen == 0, BvConst(1,1), BvConst(0,1)), 
                                  Ite(s_axi_rvalid & s_axi_rready, 
                                  Ite(tx_len == BvConst(1,8), BvConst(1,1), s_axi_rlast), s_axi_rlast)));
    instr.SetUpdate(s_axi_rid, Ite(s_axi_arready, s_axi_arid, s_axi_rid));
  }

  { // AR Not Valid
    auto instr = rmodel.NewInstr("AXIReadAddrNotValid");
    instr.SetDecode( (s_axi_aresetn_r == 1) & (s_axi_arvalid == 0) );
    // if arvalid is 1, it should hold its status
    instr.SetUpdate(s_axi_arready, Ite(~s_axi_arready & ~tx_ractive, BvConst(1,1), s_axi_arready));
    instr.SetUpdate(tx_ractive, Ite(s_axi_rvalid & s_axi_rlast & s_axi_rready, BvConst(0,1), tx_ractive));
    instr.SetUpdate(tx_len, Ite(s_axi_rvalid & s_axi_rready, tx_len - BvConst(1,8), tx_len));
    instr.SetUpdate(s_axi_rlast,  Ite(s_axi_rvalid & s_axi_rready, 
                                  Ite(tx_len == BvConst(1,8), BvConst(1,1), s_axi_rlast), s_axi_rlast));
  } 


  {
    auto instr = rmodel.NewInstr("AXIReadDataReady");
    instr.SetDecode( (s_axi_aresetn_r == 1) & (s_axi_rready == 1) );
    instr.SetUpdate(s_axi_rvalid,  Ite(unknownVal(1) == 1, BvConst(1,1), BvConst(0,1)));
    auto data = Ite(Extract(tx_arsize,1,0) == 0, Concat(Concat(unknownVal(8),unknownVal(8)), Concat(unknownVal(8),unknownVal(8))),
                Ite(Extract(tx_arsize,1,0) == 1, Concat(unknownVal(16),unknownVal(16)), unknownVal(32)));
    instr.SetUpdate(s_axi_rdata, Ite(unknownVal(1) == 1, data, s_axi_rdata) );
    instr.SetUpdate(s_axi_rresp, Ite(unknownVal(1) == 1, Ite(unknownVal(1) == 1, BvConst(2,2), BvConst(0,2) ), s_axi_rresp));
  }

  {
    auto instr = rmodel.NewInstr("AXIReadDataNotReady");
    instr.SetDecode( (s_axi_aresetn_r == 1) & (s_axi_rready == 0) );
    instr.SetUpdate(s_axi_rvalid,  Ite(unknownVal(1) == 1, BvConst(1,1), s_axi_rvalid));
  }

}
