/// \file the ila example of EMESH-AXI-Bridge
///  Huaixi Lu (huaixil@princeton.edu)
///

#ifndef EMESH_AXI_SLAVE_H__
#define EMESH_AXI_SLAVE_H__

#include <ilang/ilang++.h>
#include <vector>


using namespace ilang;

#define S_IDW 12
#define PW 104
#define AW 32
#define DW 32
#define RETURN_ADDR BvConst(0,AW)

#ifdef TARGET_SIM
#define TW 16   //timeout counter width
#else
#define TW 16  //timeout counter width
#endif

#define BURST_FIXED BvConst(0,2)
#define BURST_INCR  BvConst(1,2)
#define BURST_WRAP  BvConst(2,2)

/// \brief the class of emesh-axi-slave ila
class EmeshAxiSlaveBridge {

public:
  // --------------- CONSTRUCTOR ------ //
  /// add state, add instructions, add child
  EmeshAxiSlaveBridge();
  // --------------- MEMBERS ----------- //
  /// the ila model for write channels
  Ila wmodel;
  /// the ila model for read channels
  Ila rmodel;

protected:
  // --------------- HELPERS -------- //
  /// a nondeterministic choice of a or b
  static ExprRef unknown_choice(const ExprRef& a, const ExprRef& b);
  /// a nondeterminstic bitvector const of width
  static FuncRef unknown(unsigned width);
  static ExprRef unknownVal(unsigned width);
  /// a helper function to concat a vector of express
  static ExprRef lConcat(const std::vector<ExprRef> & l);


protected:
  // ------------ STATE ------------ //
  // reset
  ExprRef s_axi_aresetn_r;
  ExprRef s_axi_aresetn_w;
  
  // I/O interface: this is where the commands come from.
  // ExprRef wr_access; // output
  // ExprRef wr_packet; // output
  // ExprRef wr_wait;   

  // ExprRef rd_access; // output
  // ExprRef rd_packet; // output
  // ExprRef rd_wait;

  // ExprRef rr_access;
  // ExprRef rr_packet;
  // ExprRef rr_wait;   // output

  // AXI -- Read address
  ExprRef s_axi_arid;    //read address ID
  ExprRef s_axi_araddr;
  ExprRef s_axi_arburst;
  ExprRef s_axi_arcache;
  ExprRef s_axi_arlock;
  ExprRef s_axi_arlen;
  ExprRef s_axi_arprot;
  ExprRef s_axi_arqos;
  ExprRef s_axi_arready; //output
  ExprRef s_axi_arsize;
  ExprRef s_axi_arvalid;

  // AXI -- Write address
  ExprRef s_axi_awid;
  ExprRef s_axi_awaddr;
  ExprRef s_axi_awburst;
  ExprRef s_axi_awcache;
  ExprRef s_axi_awlock;
  ExprRef s_axi_awlen;
  ExprRef s_axi_awprot;
  ExprRef s_axi_awqos;   
  ExprRef s_axi_awsize;
  ExprRef s_axi_awvalid;
  ExprRef s_axi_awready; //output

  // AXI -- Read data
  ExprRef s_axi_rid;    //output
  ExprRef s_axi_rdata;  //output
  ExprRef s_axi_rlast;  //output
  ExprRef s_axi_rresp;  //output
  ExprRef s_axi_rvalid; //output
  ExprRef s_axi_rready;
  
  // AXI -- Write data 
  ExprRef s_axi_wid;
  ExprRef s_axi_wdata;
  ExprRef s_axi_wlast;   
  ExprRef s_axi_wstrb;
  ExprRef s_axi_wvalid;
  ExprRef s_axi_wready; //output

  // AXI -- Buffered Write response
  ExprRef s_axi_bid;    //output
  ExprRef s_axi_bresp;  //output
  ExprRef s_axi_bvalid; //output
  ExprRef s_axi_bready;

  // internal state
  ExprRef tx_wactive;
  ExprRef tx_bwait;
  ExprRef tx_ractive;
  
  ExprRef tx_arlen;
  ExprRef tx_arsize;

}; // class EmeshAxiSlaveBridge


#endif // EMESH_AXI_SLAVE_H__


