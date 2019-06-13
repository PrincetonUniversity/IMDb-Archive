// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: configs_spec_mmio_regs.h

// mmio addressable registers
//  - documented/specified
//  - reserved registers not included

#ifndef LMAC_CORE2_CONFIGS_SPEC_MMIO_REGS_H__
#define LMAC_CORE2_CONFIGS_SPEC_MMIO_REGS_H__

namespace ilang {

// Number of transmitted packets. (Each register is 32 bit)
#define FMAC_TX_PKT_CNT "FMAC_TX_PKT_CNT"
#define FMAC_TX_PKT_CNT_BWID 32
#define FMAC_TX_PKT_CNT_ADDR 0x1020

// Number of received packets (bit 31:0 of 64 bit counter)
#define FMAC_RX_PKT_CNT_LO "FMAC_RX_PKT_CNT_LO"
#define FMAC_RX_PKT_CNT_LO_BWID 32
#define FMAC_RX_PKT_CNT_LO_ADDR 0x1028

// Number of received packets (bit 63:32 of 64 bit counter)
#define FMAC_RX_PKT_CNT_HI "FMAC_RX_PKT_CNT_HI"
#define FMAC_RX_PKT_CNT_HI_BWID 32
#define FMAC_RX_PKT_CNT_HI_ADDR 0x102C

// Number of bytes transmitted
#define FMAC_TX_BYTE_CNT "FMAC_TX_BYTE_CNT"
#define FMAC_TX_BYTE_CNT_BWID 32
#define FMAC_TX_BYTE_CNT_ADDR 0x1030

// Number of bytes received (low part of 64 bit count)
#define FMAC_RX_BYTE_CNT_LO "FMAC_RX_BYTE_CNT_LO"
#define FMAC_RX_BYTE_CNT_LO_BWID 32
#define FMAC_RX_BYTE_CNT_LO_ADDR 0x1038

// Number of bytes received (high part of 64 bit count)
#define FMAC_RX_BYTE_CNT_HI "FMAC_RX_BYTE_CNT_HI"
#define FMAC_RX_BYTE_CNT_HI_BWID 32
#define FMAC_RX_BYTE_CNT_HI_ADDR 0x103C

// Number of undersize packets received (<64 byte packet)
#define FMAC_RX_UNDERSIZE_PKT_CNT "FMAC_RX_UNDERSIZE_PKT_CNT"
#define FMAC_RX_UNDERSIZE_PKT_CNT_BWID 32
#define FMAC_RX_UNDERSIZE_PKT_CNT_ADDR 0x1040

// Number of CRC error packets encountered
#define FMAC_RX_CRC32_ERR_CNT "FMAC_RX_CRC32_ERR_CNT"
#define FMAC_RX_CRC32_ERR_CNT_BWID 32
#define FMAC_RX_CRC32_ERR_CNT_ADDR 0x1048

// Number of packets overrun the RxFIFO and dropped
#define FMAC_RX_DCNT_OVERRUN "FMAC_RX_DCNT_OVERRUN"
#define FMAC_RX_DCNT_OVERRUN_BWID 32
#define FMAC_RX_DCNT_OVERRUN_ADDR 0x1050

// Number of packets received encountered link error
#define FMAC_RX_DCNT_LINK_ERR "FMAC_RX_DCNT_LINK_ERR"
#define FMAC_RX_DCNT_LINK_ERR_BWID 32
#define FMAC_RX_DCNT_LINK_ERR_ADDR 0x1058

// Number of packets received but over the MAX packet size
#define FMAC_RX_PKT_CNT_OVERSIZE "FMAC_RX_PKT_CNT_OVERSIZE"
#define FMAC_RX_PKT_CNT_OVERSIZE_BWID 32
#define FMAC_RX_PKT_CNT_OVERSIZE_ADDR 0x1060

// Internal PHY/Ethernet Link status and information
#define FMAC_PHY_STAT "FMAC_PHY_STAT"
#define FMAC_PHY_STAT_BWID 32
#define FMAC_PHY_STAT_ADDR 0x1068

// Number of jabber packets
#define FMAC_RX_PKT_CNT_JABBER "FMAC_RX_PKT_CNT_JABBER"
#define FMAC_RX_PKT_CNT_JABBER_BWID 32
#define FMAC_RX_PKT_CNT_JABBER_ADDR 0x1078

// Number of fragmented packets
#define FMAC_RX_PKT_CNT_FRAGMENT "FMAC_RX_PKT_CNT_FRAGMENT"
#define FMAC_RX_PKT_CNT_FRAGMENT_BWID 32
#define FMAC_RX_PKT_CNT_FRAGMENT_ADDR 0x1080

// Number of raw Ethernet frames received
#define FMAC_RX_RAW_FRAME_CNT "FMAC_RX_RAW_FRAME_CNT"
#define FMAC_RX_RAW_FRAME_CNT_BWID 32
#define FMAC_RX_RAW_FRAME_CNT_ADDR 0x1088

// Number of bad Ethernet frames received
#define FMAC_RX_BAD_FRAME_CNT "FMAC_RX_BAD_FRAME_CNT"
#define FMAC_RX_BAD_FRAME_CNT_BWID 32
#define FMAC_RX_BAD_FRAME_CNT_ADDR 0x1090

// Number of packets with size < 64 bytes (low 32 bit count)
#define FMAC_RX_PKT_CNT64_LO "FMAC_RX_PKT_CNT64_LO"
#define FMAC_RX_PKT_CNT64_LO_BWID 32
#define FMAC_RX_PKT_CNT64_LO_ADDR 0x1800

// Number of packets with size < 64 bytes (high 32 bit count)
#define FMAC_RX_PKT_CNT64_HI "FMAC_RX_PKT_CNT64_HI"
#define FMAC_RX_PKT_CNT64_HI_BWID 32
#define FMAC_RX_PKT_CNT64_HI_ADDR 0x1804

// Number of packets with size < 127 bytes (low 32 bit count)
#define FMAC_RX_PKT_CNT127_LO "FMAC_RX_PKT_CNT127_LO"
#define FMAC_RX_PKT_CNT127_LO_BWID 32
#define FMAC_RX_PKT_CNT127_LO_ADDR 0x1808

// Number of packets with size < 127 bytes (high 32 bit count)
#define FMAC_RX_PKT_CNT127_HI "FMAC_RX_PKT_CNT127_HI"
#define FMAC_RX_PKT_CNT127_HI_BWID 32
#define FMAC_RX_PKT_CNT127_HI_ADDR 0x180C

// .......
#define FMAC_RX_PKT_CNT255_LO "FMAC_RX_PKT_CNT255_LO"
#define FMAC_RX_PKT_CNT255_LO_BWID 32
#define FMAC_RX_PKT_CNT255_LO_ADDR 0x1810

// .......
#define FMAC_RX_PKT_CNT255_HI "FMAC_RX_PKT_CNT255_HI"
#define FMAC_RX_PKT_CNT255_HI_BWID 32
#define FMAC_RX_PKT_CNT255_HI_ADDR 0x1814

// .......
#define FMAC_RX_PKT_CNT511_LO "FMAC_RX_PKT_CNT511_LO"
#define FMAC_RX_PKT_CNT511_LO_BWID 32
#define FMAC_RX_PKT_CNT511_LO_ADDR 0x1818

// .......
#define FMAC_RX_PKT_CNT511_HI "FMAC_RX_PKT_CNT511_HI"
#define FMAC_RX_PKT_CNT511_HI_BWID 32
#define FMAC_RX_PKT_CNT511_HI_ADDR 0x181C

// .......
#define FMAC_RX_PKT_CNT1023_LO "FMAC_RX_PKT_CNT1023_LO"
#define FMAC_RX_PKT_CNT1023_LO_BWID 32
#define FMAC_RX_PKT_CNT1023_LO_ADDR 0x1820

// .......
#define FMAC_RX_PKT_CNT1023_HI "FMAC_RX_PKT_CNT1023_HI"
#define FMAC_RX_PKT_CNT1023_HI_BWID 32
#define FMAC_RX_PKT_CNT1023_HI_ADDR 0x1824

// Number of packets with size < 1518 bytes (low 32 bit count)
#define FMAC_RX_PKT_CNT1518_LO "FMAC_RX_PKT_CNT1518_LO"
#define FMAC_RX_PKT_CNT1518_LO_BWID 32
#define FMAC_RX_PKT_CNT1518_LO_ADDR 0x1828

// Number of packets with size < 1518 bytes (high 32 bit count)
#define FMAC_RX_PKT_CNT1518_HI "FMAC_RX_PKT_CNT1518_HI"
#define FMAC_RX_PKT_CNT1518_HI_BWID 32
#define FMAC_RX_PKT_CNT1518_HI_ADDR 0x182C

}; // namespace ilang

#endif // LMAC_CORE2_CONFIGS_SPEC_MMIO_REGS_H__

