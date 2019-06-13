// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: mmio_regs.cc

// LmacCore2 member function
//  - set up the mmio addressable registers

#include <lmac/core2/configs.h>
#include <lmac/core2/lmac_core_top.h>
#include <lmac/utils.h>

namespace ilang {

void LmacCore2::SetupMmioRegs(Ila& m) {
  // Number of transmitted packets. (Each register is 32 bit)
  NewState(m, FMAC_TX_PKT_CNT, FMAC_TX_PKT_CNT_BWID);

  // Number of received packets (bit 31:0 of 64 bit counter)
  NewState(m, FMAC_RX_PKT_CNT_LO, FMAC_RX_PKT_CNT_LO_BWID);

  // Number of received packets (bit 63:32 of 64 bit counter)
  NewState(m, FMAC_RX_PKT_CNT_HI, FMAC_RX_PKT_CNT_HI_BWID);

  // Number of bytes transmitted
  NewState(m, FMAC_TX_BYTE_CNT, FMAC_TX_BYTE_CNT_BWID);

  // Number of bytes received (low part of 64 bit count)
  NewState(m, FMAC_RX_BYTE_CNT_LO, FMAC_RX_BYTE_CNT_LO_BWID);

  // Number of bytes received (high part of 64 bit count)
  NewState(m, FMAC_RX_BYTE_CNT_HI, FMAC_RX_BYTE_CNT_HI_BWID);

  // Number of undersize packets received (<64 byte packet)
  NewState(m, FMAC_RX_UNDERSIZE_PKT_CNT, FMAC_RX_UNDERSIZE_PKT_CNT_BWID);

  // Number of CRC error packets encountered
  NewState(m, FMAC_RX_CRC32_ERR_CNT, FMAC_RX_CRC32_ERR_CNT_BWID);

  // Number of packets overrun the RxFIFO and dropped
  NewState(m, FMAC_RX_DCNT_OVERRUN, FMAC_RX_DCNT_OVERRUN_BWID);

  // Number of packets received encountered link error
  NewState(m, FMAC_RX_DCNT_LINK_ERR, FMAC_RX_DCNT_LINK_ERR_BWID);

  // Number of packets received but over the MAX packet size
  NewState(m, FMAC_RX_PKT_CNT_OVERSIZE, FMAC_RX_PKT_CNT_OVERSIZE_BWID);

  // Internal PHY/Ethernet Link status and information
  NewState(m, FMAC_PHY_STAT, FMAC_PHY_STAT_BWID);

  // Number of jabber packets
  NewState(m, FMAC_RX_PKT_CNT_JABBER, FMAC_RX_PKT_CNT_JABBER_BWID);

  // Number of fragmented packets
  NewState(m, FMAC_RX_PKT_CNT_FRAGMENT, FMAC_RX_PKT_CNT_FRAGMENT_BWID);

  // Number of raw Ethernet frames received
  NewState(m, FMAC_RX_RAW_FRAME_CNT, FMAC_RX_RAW_FRAME_CNT_BWID);

  // Number of bad Ethernet frames received
  NewState(m, FMAC_RX_BAD_FRAME_CNT, FMAC_RX_BAD_FRAME_CNT_BWID);

  // Number of packets with size < 64 bytes (low 32 bit count)
  NewState(m, FMAC_RX_PKT_CNT64_LO, FMAC_RX_PKT_CNT64_LO_BWID);

  // Number of packets with size < 64 bytes (high 32 bit count)
  NewState(m, FMAC_RX_PKT_CNT64_HI, FMAC_RX_PKT_CNT64_HI_BWID);

  // Number of packets with size < 127 bytes (low 32 bit count)
  NewState(m, FMAC_RX_PKT_CNT127_LO, FMAC_RX_PKT_CNT127_LO_BWID);

  // Number of packets with size < 127 bytes (high 32 bit count)
  NewState(m, FMAC_RX_PKT_CNT127_HI, FMAC_RX_PKT_CNT127_HI_BWID);

  // .......
  NewState(m, FMAC_RX_PKT_CNT255_LO, FMAC_RX_PKT_CNT255_LO_BWID);

  // .......
  NewState(m, FMAC_RX_PKT_CNT255_HI, FMAC_RX_PKT_CNT255_HI_BWID);

  // .......
  NewState(m, FMAC_RX_PKT_CNT511_LO, FMAC_RX_PKT_CNT511_LO_BWID);

  // .......
  NewState(m, FMAC_RX_PKT_CNT511_HI, FMAC_RX_PKT_CNT511_HI_BWID);

  // .......
  NewState(m, FMAC_RX_PKT_CNT1023_LO, FMAC_RX_PKT_CNT1023_LO_BWID);

  // .......
  NewState(m, FMAC_RX_PKT_CNT1023_HI, FMAC_RX_PKT_CNT1023_HI_BWID);

  // Number of packets with size < 1518 bytes (low 32 bit count)
  NewState(m, FMAC_RX_PKT_CNT1518_LO, FMAC_RX_PKT_CNT1518_LO_BWID);

  // Number of packets with size < 1518 bytes (high 32 bit count)
  NewState(m, FMAC_RX_PKT_CNT1518_HI, FMAC_RX_PKT_CNT1518_HI_BWID);

  return;
}

}; // namespace ilang

