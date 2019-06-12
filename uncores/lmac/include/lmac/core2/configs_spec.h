// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: configs_spec.h

// specification
//  - hardware parameters documented in the specification/documentation
//  - behavior defined in the specification/documentation

#ifndef LMAC_CORE2_CONFIGS_SPEC_H__
#define LMAC_CORE2_CONFIGS_SPEC_H__

namespace ilang {

// -------------------------------------------------------------------------- //
// TX FIFO Interface
// -------------------------------------------------------------------------- //

// Write enable.  1 = data is valid on the TX_DATA bus. 0 = no data is on the
// data bus
#define TX_WE "TX_WE"
#define TX_WE_BWID 1
#define TX_WE_V_VALID 1

// 64-bit data bus
// The first qword sent by the user logic is the byte count of
// the packet as follows: Qwd0[15:0] = packet byte count (total number of bytes
// in the header and payload of the packet.  Example 16’h003C for smallest 64
// byte Ethernet packet.)
//
// Subsequent qwords after the packet byte count are the Ethernet packet header
// and the payload.
//
// The format of the packet follows the Ethernet packet format which is similar
// to what is shown by standard tools such as Wireshark.  The data on this
// TX_DATA however must be in big-endian for the packet data.  (See example of
// ARP packet below)
#define TX_DATA "TX_DATA"
#define TX_DATA_BWID 64
#define TX_DATA_PKT_BYTE_CNT_BEG 0
#define TX_DATA_PKT_BYTE_CNT_END 15

// (optional)
// Byte enable of the 64-bit data bus. 1 bit for each byte lane of the data bus.
// TX_BE[0] = if 1, means data is valid on TX_DATA[7:0] And TX_BE[7] = if 1,
// means data is valid on TX_DATA[63:56]
//
// The internal logic of the LMAC generates its own byte enable.
#define TX_BE "TX_BE"
#define TX_BE_BWID 8
#define TX_BE_V_ENABLE 1

// 1 = the internal TX FIFO is full and cannot accept any more packet.
#define TXFIFO_FULL "TXFIFO_FULL"
#define TXFIFO_FULL_BWID 1
#define TXFIFO_FULL_V_FULL 1

// Indicating the number of qwords the TX FIFO contained.  This value is dynamic
// and can change from clock to clock.  (Current implementation only uses bits
// [9:0] for FIFO size 1Kx64)
#define TXFIFO_WUSED_QWD "TXFIFO_WUSED_QWD"
#define TXFIFO_WUSED_QWD_BWID 13

// -------------------------------------------------------------------------- //
// RX FIFO Interface
// -------------------------------------------------------------------------- //
// 1. Receiving Bus - package byte count/size interface (IPCS)
// 2. Receiving Bus - package data interface. (after reading the byte count the
//    user can read out the packet's data - 1 entire packet at a time)

//
// 1. Receiving Bus - package byte count/size interface (IPCS)
//

// Byte count FIFO’s empty signal.  0 = a byte count is available for reading. 1
// = no byte count is available
#define RX_IPCS_EMPTY "RX_IPCS_EMPTY"
#define RX_IPCS_EMPTY_BWID 1
#define RX_IPCS_EMPTY_V_READY 0
#define RX_IPCS_EMPTY_V_NONE 1

// Read enable for the byte count FIFO.
// User asserts 1 pulse to read 1 qword from the byte count FIFO for the size of
// 1 packet.
#define RX_IPCS_RDEN "RX_IPCS_RDEN"
#define RX_IPCS_RDEN_BWID 1
#define RX_IPCS_RDEN_V_READ 1

// 64-bit IPCS data bus
// The IPCS bus is only intended for reading 1 qword at a time.
// Currently:
// [63:48] = packet’s byte count
// [47:0] = reserve
#define RX_IPCS_DATA "RX_IPCS_DATA"
#define RX_IPCS_DATA_BWID 64
#define RX_IPCS_PKT_BYTE_CNT_BEG 48
#define RX_IPCS_PKT_BYTE_CNT_END 63

//
// 2. Receiving Bus - package data interface. (after reading the byte count the
//    user can read out the packet's data - 1 entire packet at a time)
//

// Packet data FIFO’s empty signal.  0 = a packet is available for reading. 1 =
// no packet is available
#define RX_PKT_EMPTY "RX_PKT_EMPTY"
#define RX_PKT_EMPTY_BWID 1
#define RX_PKT_EMPTY_V_READY 0
#define RX_PKT_EMPTY_V_NONE 1

// Read enable for the packet data FIFO. User asserts 1 pulse to read 1 qword
// from the packet data FIFO.
#define RX_PKT_RDEN "RX_PKT_RDEN"
#define RX_PKT_RDEN_BWID 1
#define RX_PKT_RDEN_V_READ 1

// Indicates a user packet read cycle is in progress.  This signal is used to
// indicate to the LMAC that a user packet read cycle is in progress which can
// be lengthy depending on the size of the packet.  Asserts high to start the
// cycle and only negates when the user’s cycle is fully complete.
#define RX_PKT_RD_CYCLE "RX_PKT_RD_CYCLE"
#define RX_PKT_RD_CYCLE_BWID 1
#define RX_PKT_RD_CYCLE_BUSY 1
#define RX_PKT_RD_CYCLE_DONE 0

// Packet’s data bus.
// For each RDEN a qword is sent out to the user from the packet data FIFO.  The
// full qword is always provided.  Based on the packet’s byte count the user can
// determines how many bytes is available or remaining to be read from the
// packet. Similar to FPGA FIFO, it takes 1 clock for data to be available from
// the read enable.
#define RX_PKT_DATA "RX_PKT_DATA"
#define RX_PKT_DATA_BWID 64

// -------------------------------------------------------------------------- //
// LMAC Register Interface, Configuration, Clock and Reset
// -------------------------------------------------------------------------- //
// 1. clocks and reset signals
// 2. configuration signals
// 3. register interface

//
// 1. clocks and reset signals
//

//
// 2. configuration signals
//

// Static signal from the LMAC.  If 1, indicating the Ethernet link is up.
#define LINKUP "Linkup"
#define LINKUP_BWID 1
#define LINKUP_V_UP 1

// 32 bit control signals (See MAC_CTRL below)
#define MAC_CTRL "MAC_CTRL"
#define MAC_CTRL_BWID 32

// Second set of 32-bit control signals (See MAC_CTRL1 below)
#define MAC_CTRL1 "MAC_CTRL1"
#define MAC_CTRL1_BWID 32

// LMAC receive path enable.  This is a static signal.  It allows the user logic
// to control the receiving function of the LMAC.  It’s useful in debug support.
// 1 = RX enable
#define MAC_RXD_EN "MAC_RXD_EN"
#define MAC_RXD_EN_BWID 1
#define MAC_RXD_EN_V_ENABLE 1

//
// MAC_CTRL Configuration Detail
//

// Enable the TX Path to transmit PAUSE frame for stopping other sources from
// sending any more packet(s) to this port (XOFF event) or transmitting XON
// frame.
// 0 = disable PAUSE frame transmission
// 1 = enable PAUSE frame transmission
#define TX_XO_EN_BEG 0
#define TX_XO_EN_END 0
#define TX_XO_EN_V_DEFAULT 0
#define TX_XO_EN_V_DISABLE 0
#define TX_XO_EN_V_ENABLE 1

// Enable the RX Path to receive PAUSE frame.
// 0 = disable PAUSE frame receipt
// 1 = enable PAUSE frame receipt
#define RX_XO_EN_BEG 1
#define RX_XO_EN_END 1
#define RX_XO_EN_V_DEFAULT 0
#define RX_XO_EN_V_DISABLE 0
#define RX_XO_EN_V_ENABLE 1

// Reserved
#define MAC_CTRL_RSV_2_BEG 2
#define MAC_CTRL_RSV_2_END 2
#define MAC_CTRL_RSV_2_V_DEFAULT 0

// Enable CRC checking
// 0 = disable
// 1 = enable
// If enabled, incoming CRC will be checked.  Bad CRC packet will be dropped.
// If disabled, checking of bad CRC is disabled and bad CRC packets will be
// forwarded as if they are good CRC packets.  This is mainly for diagnostic
// purposes
#define RX_CRC_EN_BEG 3
#define RX_CRC_EN_END 3
#define RX_CRC_EN_V_DEFAULT 0
#define RX_CRC_EN_V_DISABLE 0
#define RX_CRC_EN_V_ENABLE 1

// Enable Promiscuous mode (see definition)
// 0 = disable
// 1 = enable
#define PROMIS_MODE_EN_BEG 4
#define PROMIS_MODE_EN_END 4
#define PROMIS_MODE_EN_V_DEFAULT 1
#define PROMIS_MODE_EN_V_DISABLE 0
#define PROMIS_MODE_EN_V_ENABLE 1

// Reserved
#define MAC_CTRL_RSV_5_BEG 5
#define MAC_CTRL_RSV_5_END 5
#define MAC_CTRL_RSV_5_V_DEFAULT 0

// If set to 1, enable the auto clearing of TX statistic registers upon a read
// to the register is detected.  Only a few registers has this capability.
// 0 = disable
// 1 = enable
#define TX_CNT_AUTO_CLR_EN_BEG 6
#define TX_CNT_AUTO_CLR_EN_END 6
#define TX_CNT_AUTO_CLR_EN_V_DEFAULT 0
#define TX_CNT_AUTO_CLR_EN_V_DISABLE 0
#define TX_CNT_AUTO_CLR_EN_V_ENABLE 1

// If set to 1, enable the auto clearing of RX statistic registers upon a read
// to the register is detected.  Only a few registers has this capability.
// 0 = disable
// 1 = enable
#define RX_CNT_AUTO_CLR_EN_BEG 7
#define RX_CNT_AUTO_CLR_EN_END 7
#define RX_CNT_AUTO_CLR_EN_V_DEFAULT 0
#define RX_CNT_AUTO_CLR_EN_V_DISABLE 0
#define RX_CNT_AUTO_CLR_EN_V_ENABLE 1

// Reserved
#define MAC_CTRL_RSV_8_BEG 8
#define MAC_CTRL_RSV_8_END 9
#define MAC_CTRL_RSV_8_V_DEFAULT 0

// Enable Receiving Multicast packets.
// 1’b0 = disable
// 1’b1 = enable
#define MCAST_EN_BEG 10
#define MCAST_EN_END 10
#define MCAST_EN_V_DEFAULT 1
#define MCAST_EN_V_DISABLE 0
#define MCAST_EN_V_ENABLE 1

// If enabled, LMAC will accept received broadcast packets.
// 1’b0 = disable
// 1’b1 = enable
#define ACCEPT_BRDCST_EN_BEG 11
#define ACCEPT_BRDCST_EN_END 11
#define ACCEPT_BRDCST_EN_V_DEFAULT 1
#define ACCEPT_BRDCST_EN_V_DISABLE 0
#define ACCEPT_BRDCST_EN_V_ENABLE 1

// Reserved
#define MAC_CTRL_RSV_12_BEG 12
#define MAC_CTRL_RSV_12_END 12
#define MAC_CTRL_RSV_12_V_DEFAULT 0

// Reserved
#define MAC_CTRL_RSV_13_BEG 13
#define MAC_CTRL_RSV_13_END 13
#define MAC_CTRL_RSV_13_V_DEFAULT 0

// Reserved
#define MAC_CTRL_RSV_14_BEG 14
#define MAC_CTRL_RSV_14_END 14
#define MAC_CTRL_RSV_14_V_DEFAULT 0

// Reserved
#define MAC_CTRL_RSV_15_BEG 15
#define MAC_CTRL_RSV_15_END 15
#define MAC_CTRL_RSV_15_V_DEFAULT 0

// Reserved
#define MAC_CTRL_RSV_16_BEG 16
#define MAC_CTRL_RSV_16_END 19
#define MAC_CTRL_RSV_16_V_DEFAULT 0

// Reserved
#define MAC_CTRL_RSV_20_BEG 20
#define MAC_CTRL_RSV_20_END 23
#define MAC_CTRL_RSV_20_V_DEFAULT 0

// Reserved
#define MAC_CTRL_RSV_24_BEG 24
#define MAC_CTRL_RSV_24_END 31
#define MAC_CTRL_RSV_24_V_DEFAULT 0

//
// MAC_CTRL1
//

// Reserved
#define MAC_CTRL1_RSV_28_BEG 28
#define MAC_CTRL1_RSV_28_END 31
#define MAC_CTRL1_RSV_28_V_DEFAULT 0

// Reserved
#define MAC_CTRL1_RSV_27_BEG 27
#define MAC_CTRL1_RSV_27_END 27

// Reserved
#define MAC_CTRL1_RSV_24_BEG 24
#define MAC_CTRL1_RSV_24_END 26
#define MAC_CTRL1_RSV_24_V_DEFAULT 0

// Reserved
#define MAC_CTRL1_RSV_23_BEG 23
#define MAC_CTRL1_RSV_23_END 23

// Reserved
#define MAC_CTRL1_RSV_20_BEG 20
#define MAC_CTRL1_RSV_20_END 22
#define MAC_CTRL1_RSV_20_V_DEFAULT 0

// Reserved
#define MAC_CTRL1_RSV_19_BEG 19
#define MAC_CTRL1_RSV_19_END 19
#define MAC_CTRL1_RSV_19_V_DEFAULT 0

// Reserved
#define MAC_CTRL1_RSV_18_BEG 18
#define MAC_CTRL1_RSV_18_END 18
#define MAC_CTRL1_RSV_18_V_DEFAULT 0

// Reserved
#define MAC_CTRL1_RSV_16_BEG 16
#define MAC_CTRL1_RSV_16_END 17
#define MAC_CTRL1_RSV_16_V_DEFAULT 0

// Reserved
#define MAC_CTRL1_RSV_15_BEG 15
#define MAC_CTRL1_RSV_15_END 15
#define MAC_CTRL1_RSV_15_V_DEFAULT 0

// Reserved
#define MAC_CTRL1_RSV_14_BEG 14
#define MAC_CTRL1_RSV_14_END 14
#define MAC_CTRL1_RSV_14_V_DEFAULT 0

// This field configures the max packet size in byte for the Ethernet port.
// Typically, Ethernet packets are between 64 and 1518 (5EE hex) bytes. This
// value is applicable for most applications.
#define MAX_PKT_SIZE_BEG 0
#define MAX_PKT_SIZE_END 13
#define MAC_PKT_SIZE_V_DEFAULT 0x5EE

//
// 3. register interface
//

//
// Host Address Bus
//

// 16 bit host byte address bus for selecting a memory mapped register. Valid 1
// clk before register read start signal. Address must be 64-bit aligned for
// most cases.
#define HOST_ADDR "host_addr"
#define HOST_ADDR_BWID 16

// Pulse. Register read start.
// 1 = Start the read for a register in the LMAC core 0 = idle.
#define REG_RD_START "reg_rd_start"
#define REG_RD_START_BWID 1
#define REG_RD_START_V_BUSY 1
#define REG_RD_START_V_IDLE 0

// Pulse. Indicating the register read is done and its data is available on the
// MAC_REGDOUT bus.
// 1 = indicating the data is available for the read to register
// 0 = data not available.
// This signal is the 5-clock delay of the register read start signal.
#define REG_RD_DONE_OUT "reg_rd_done_out"
#define REG_RD_DONE_OUT_BWID 1
#define REG_RD_DONE_OUT_V_READY 1
#define REG_RD_DONE_OUT_V_NONE 0

//
// Read Completion Data Bus
//

// Data returned from LMAC register. Read is 32 bit at a time.
#define MAC_REGDOUT "mac_regdout"
#define MAC_REGDOUT_BWID 32

//
// Available Registers
//

#include <lmac/core2/configs_spec_mmio_regs.h>

}; // namespace ilang

#endif // LMAC_CORE2_CONFIGS_SPEC_H__

