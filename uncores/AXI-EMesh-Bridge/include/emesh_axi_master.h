/// \file the ila example of EMESH-AXI-Bridge
///  Hongce Zhang (hongcez@princeton.edu)
///

#ifndef EMESH_AXI_MASTER_H__
#define EMESH_AXI_MASTER_H__

#include <ilang/ilang++.h>
#include <vector>


using namespace ilang;

#define M_IDW 12
#define PW 104
#define AW 32
#define DW 32
#define BURST_FIXED BvConst(0,2)
#define BURST_INCR  BvConst(1,2)
#define BURST_WRAP  BvConst(2,2)

/// \brief the class of emesh-axi-master ila
class EmeshAxiMasterBridge {

public:
  // --------------- CONSTRUCTOR ------ //
  /// add state, add instructions, add child
  EmeshAxiMasterBridge();
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
  ExprRef m_axi_aresetn_r;
  ExprRef m_axi_aresetn_w;
  
  // I/O interface: this is where the commands come from.
  // ExprRef wr_access;
  // ExprRef wr_packet;
  // ExprRef wr_wait;   // output

  // ExprRef rd_access;
  // ExprRef rd_packet;
  // ExprRef rd_wait;   // output

  // ExprRef rr_access; // output
  // ExprRef rr_packet; // output
  // ExprRef rr_wait;   

  // AXI -- Write address
  ExprRef m_axi_awid;    // output
  ExprRef m_axi_awaddr;  // output
  ExprRef m_axi_awlen;   // output
  ExprRef m_axi_awsize;  // output
  ExprRef m_axi_awburst;  // output
  ExprRef m_axi_awlock;  // output
  ExprRef m_axi_awcache;  // output
  ExprRef m_axi_awprot;  // output
  ExprRef m_axi_awqos;   // output
  ExprRef m_axi_awvalid; // output
  ExprRef m_axi_awready; 

  // AXI -- Write data
  ExprRef m_axi_wid;     // output
  ExprRef m_axi_wdata;   // output
  ExprRef m_axi_wstrb;   // output
  ExprRef m_axi_wlast;   // output
  ExprRef m_axi_wvalid;  // output
  ExprRef m_axi_wready;  

  // AXI -- Write response
  ExprRef m_axi_bid;     
  ExprRef m_axi_bresp;   
  ExprRef m_axi_bvalid;  
  ExprRef m_axi_bready;  // output

  // AXI -- Read address
  ExprRef m_axi_arid;    // output
  ExprRef m_axi_araddr;  // output
  ExprRef m_axi_arlen;   // output
  ExprRef m_axi_arsize;  // output
  ExprRef m_axi_arburst; // output
  ExprRef m_axi_arlock;  // output
  ExprRef m_axi_arcache; // output
  ExprRef m_axi_arprot;  // output
  ExprRef m_axi_arqos;   // output
  ExprRef m_axi_arvalid; // output
  ExprRef m_axi_arready; 

  // AXI -- Read data
  ExprRef m_axi_rid;     
  ExprRef m_axi_rdata;   
  ExprRef m_axi_rresp;   
  ExprRef m_axi_rlast;   
  ExprRef m_axi_rvalid;  
  ExprRef m_axi_rready;  // output
  
  // internal state
  ExprRef tx_valid;
  ExprRef tx_burst;
  ExprRef tx_id;
  ExprRef tx_addr;
  ExprRef tx_data;
  ExprRef tx_len;
  ExprRef tx_size;
  ExprRef tx_count;
  ExprRef tx_addr_done;
  ExprRef tx_data_done;

}; // class EmeshAxiMasterBridge


#endif // EMESH_AXI_MASTER_H__


