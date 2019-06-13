// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: tx_interface.cc

// LmacCore2 member function
//  - set up the signals on the TX FIFO/User logic interface

#include <lmac/core2/configs.h>
#include <lmac/core2/lmac_core_top.h>
#include <lmac/utils.h>

namespace ilang {

void LmacCore2::SetupTxInterface(Ila& m) {
  // Write enable.  1 = data is valid on the TX_DATA bus. 0 = no data is on the
  // data bus
  NewInput(m, TX_WE, TX_WE_BWID);

  // 64-bit data bus
  // The first qword sent by the user logic is the byte count of
  // the packet as follows: Qwd0[15:0] = packet byte count (total number of
  // bytes in the header and payload of the packet.  Example 16’h003C for
  // smallest 64 byte Ethernet packet.)
  //
  // Subsequent qwords after the packet byte count are the Ethernet packet
  // header and the payload.
  //
  // The format of the packet follows the Ethernet packet format which is
  // similar to what is shown by standard tools such as Wireshark.  The data on
  // this TX_DATA however must be in big-endian for the packet data.  (See
  // example of ARP packet below)
  NewInput(m, TX_DATA, TX_DATA_BWID);

  // (optional)
  // Byte enable of the 64-bit data bus. 1 bit for each byte lane of the data
  // bus. TX_BE[0] = if 1, means data is valid on TX_DATA[7:0] And TX_BE[7] = if
  // 1, means data is valid on TX_DATA[63:56]
  //
  // The internal logic of the LMAC generates its own byte enable.
  NewInput(m, TX_BE, TX_BE_BWID);

  // 1 = the internal TX FIFO is full and cannot accept any more packet.
  NewState(m, TXFIFO_FULL, TXFIFO_FULL_BWID);

  // Indicating the number of qwords the TX FIFO contained.  This value is
  // dynamic and can change from clock to clock.  (Current implementation only
  // uses bits [9:0] for FIFO size 1Kx64)
  NewState(m, TXFIFO_WUSED_QWD, TXFIFO_WUSED_QWD_BWID);

  return;
}

}; // namespace ilang

