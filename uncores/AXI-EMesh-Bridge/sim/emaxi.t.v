module emaxi_tb();

parameter M_IDW  = 12;
parameter PW     = 104;
parameter AW     = 32;
parameter DW     = 32;

reg clk;
reg rstn;

initial begin
  $dumpfile("sim.vcd");
  $dumpvars;

  clk = 0;
  rstn = 1;
  #5 rstn = 0; clk = 0;
  #5 rstn = 0; clk = 1;
  #5 rstn = 0; clk = 0;
  #5 rstn = 1; clk = 0;
  forever #5 clk = ~clk;
end

//Write request
reg               wr_access;
reg [PW-1:0]      wr_packet;   
wire              wr_wait;

//Read request
reg               rd_access;
reg [PW-1:0]      rd_packet;
wire              rd_wait;

//Read response
wire              rr_access;
wire [PW-1:0]     rr_packet;
reg               rr_wait;

wire [31:0]w_dstaddr_in   = wr_packet[39:8];    
wire [31:0]w_srcaddr_in   = wr_packet[103:72];  
wire [31:0]w_data_in      = wr_packet[71:40]; 
//########################
//AXI MASTER INTERFACE
//########################

//Write address channel
wire [M_IDW-1:0]  m_axi_awid;    // write address ID
wire [31 : 0]     m_axi_awaddr;  // master interface write address   
wire [7 : 0]      m_axi_awlen;   // burst length.
wire [2 : 0]      m_axi_awsize;  // burst size.
wire [1 : 0]      m_axi_awburst; // burst type.
wire              m_axi_awlock;  // lock type   
wire [3 : 0]      m_axi_awcache; // memory type.
wire [2 : 0]      m_axi_awprot;  // protection type.
wire [3 : 0]      m_axi_awqos;   // quality of service
wire              m_axi_awvalid; // write address valid
reg               m_axi_awready; // write address ready

//Write data channel
wire [M_IDW-1:0]  m_axi_wid;     
wire [63 : 0]     m_axi_wdata;   // master interface write data.
wire [7 : 0]      m_axi_wstrb;   // byte write strobes
wire              m_axi_wlast;   // last transfer in a write burst.
wire              m_axi_wvalid;  // indicates data is ready to go
reg               m_axi_wready;  // slave is ready for data

//Write response channel
reg [M_IDW-1:0]   m_axi_bid;
reg [1 : 0]       m_axi_bresp;   // status of the write transaction.
reg               m_axi_bvalid;  // channel is a valid write response
wire              m_axi_bready;  // master can accept write response.

//Read address channel
wire [M_IDW-1:0]  m_axi_arid;    // read address ID
wire [31 : 0]     m_axi_araddr;  // initial address of a read burst
wire [7 : 0]      m_axi_arlen;   // burst length
wire [2 : 0]      m_axi_arsize;  // burst size
wire [1 : 0]      m_axi_arburst; // burst type
wire              m_axi_arlock;  // lock type   
wire [3 : 0]      m_axi_arcache; // memory type
wire [2 : 0]      m_axi_arprot;  // protection type
wire [3 : 0]      m_axi_arqos;   // quality of service info
wire              m_axi_arvalid; // valid read address
reg               m_axi_arready; // slave is ready to accept an address

//Read data channel   
reg [M_IDW-1:0]   m_axi_rid;     // read data ID
reg [63 : 0]      m_axi_rdata;   // master read data
reg [1 : 0]       m_axi_rresp;   // status of the read transfer
reg               m_axi_rlast;   // last transfer in a read burst
reg               m_axi_rvalid;  // signaling the required read data
wire              m_axi_rready;  // master can accept the readback data

emaxi m (
  .wr_access(wr_access),
  .wr_packet(wr_packet),
  .wr_wait(wr_wait),
  .rd_access(rd_access),
  .rd_packet(rd_packet),
  .rd_wait(rd_wait),
  .rr_access(rr_access),
  .rr_packet(rr_packet),
  .rr_wait(rr_wait),
  .m_axi_aclk(clk),   
  .m_axi_aresetn(rstn),
  .m_axi_awid(m_axi_awid),   
  .m_axi_awaddr(m_axi_awaddr), 
  .m_axi_awlen(m_axi_awlen),  
  .m_axi_awsize(m_axi_awsize), 
  .m_axi_awburst(m_axi_awburst),
  .m_axi_awlock(m_axi_awlock), 
  .m_axi_awcache(m_axi_awcache),
  .m_axi_awprot(m_axi_awprot), 
  .m_axi_awqos(m_axi_awqos),  
  .m_axi_awvalid(m_axi_awvalid),
  .m_axi_awready(m_axi_awready),
  .m_axi_wid(m_axi_wid),   
  .m_axi_wdata(m_axi_wdata), 
  .m_axi_wstrb(m_axi_wstrb), 
  .m_axi_wlast(m_axi_wlast), 
  .m_axi_wvalid(m_axi_wvalid),
  .m_axi_wready(m_axi_wready),
  .m_axi_bid(m_axi_bid),
  .m_axi_bresp(m_axi_bresp), 
  .m_axi_bvalid(m_axi_bvalid),
  .m_axi_bready(m_axi_bready),
  .m_axi_arid(m_axi_arid),   
  .m_axi_araddr(m_axi_araddr), 
  .m_axi_arlen(m_axi_arlen),  
  .m_axi_arsize(m_axi_arsize), 
  .m_axi_arburst(m_axi_arburst),
  .m_axi_arlock(m_axi_arlock), 
  .m_axi_arcache(m_axi_arcache),
  .m_axi_arprot(m_axi_arprot), 
  .m_axi_arqos(m_axi_arqos),  
  .m_axi_arvalid(m_axi_arvalid),
  .m_axi_arready(m_axi_arready),
  .m_axi_rid(m_axi_rid),   
  .m_axi_rdata(m_axi_rdata), 
  .m_axi_rresp(m_axi_rresp), 
  .m_axi_rlast(m_axi_rlast), 
  .m_axi_rvalid(m_axi_rvalid),
  .m_axi_rready(m_axi_rready)
);


always @(negedge clk) begin
  if (~rstn) begin
    wr_access <= 0;
    wr_packet <= 0;
    rd_access <= 0;
    rd_packet <= 0;
    rr_wait <= 0;
    m_axi_awready <= 0;
    m_axi_wready <= 0;
    m_axi_bid <= 0;
    m_axi_bresp <= 0;
    m_axi_bvalid <= 0;
    m_axi_arready <= 0;
    m_axi_rid <= 0;
    m_axi_rdata <= 0;
    m_axi_rresp <= 0;
    m_axi_rlast <= 0;
    m_axi_rvalid <= 0;
  end else begin
    if(!wr_wait) begin
      wr_access <= $random;
      wr_packet <= $random;
    end
    
    rd_access <= $random;
    rd_packet <= $random;
    rr_wait <= $random;
    m_axi_awready <= $random;
    m_axi_wready <= $random;
    m_axi_bid <= $random;
    m_axi_bresp <= $random;
    m_axi_bvalid <= $random;
    m_axi_arready <= $random;
    m_axi_rid <= $random;
    m_axi_rdata <= $random;
    m_axi_rresp <= $random;
    m_axi_rlast <= $random;
    m_axi_rvalid <= $random;
  end
end

endmodule
