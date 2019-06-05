/// \file this is the concept of transaction level verification

module emaxi_trans(input clk, input rstn, 
  input wr_access, 
  input [104-1:0] wr_packet, 

  input m_axi_awready,
  input m_axi_wready,

  output              wr_wait,
  output [104-1:0]    m_axi_awid,    // write address ID
  output [31 : 0]     m_axi_awaddr,  // master interface write address   
  output [7 : 0]      m_axi_awlen,   // burst length.
  output [2 : 0]      m_axi_awsize,  // burst size.
  output [1 : 0]      m_axi_awburst, // burst type.
  output              m_axi_awlock,  // lock type   
  output [3 : 0]      m_axi_awcache, // memory type.
  output [2 : 0]      m_axi_awprot,  // protection type.
  output [3 : 0]      m_axi_awqos,   // quality of service
  output              m_axi_awvalid, // write address valid

  output [104-1:0]    m_axi_wid,     
  output [63 : 0]     m_axi_wdata,   // master interface write data.
  output [7 : 0]      m_axi_wstrb,   // byte write strobes
  output              m_axi_wlast,   // last transfer in a write burst.
  output              m_axi_wvalid   // indicates data is ready to go

  );


// set to 5 when initially wr_access and ~wr_wait (avoid repetition)
// when ever sees a m_axi_awready & m_axi_awvalid , --
// the match is (exists counter > 0) --> eq?


// when wr_access is true , if ~wr_wait, the output should be the same as desired within a certain boundary


endmodule // emaxi_trans
