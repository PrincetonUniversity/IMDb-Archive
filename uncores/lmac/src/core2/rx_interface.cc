// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: rx_interface.cc

// LmacCore2 member function
//  - set up the signals on the RX FIFO to user logic interface

#include <lmac/core2/configs.h>
#include <lmac/core2/lmac_core_top.h>
#include <lmac/utils.h>

namespace ilang {

void LmacCore2::SetupRxInterface(Ila& m) {
  //
  // 1. Receiving Bus - package byte count/size interface (IPCS)
  //

  // Byte count FIFO’s empty signal.  0 = a byte count is available for reading.
  // 1 = no byte count is available
  NewState(m, RX_IPCS_EMPTY, RX_IPCS_EMPTY_BWID);

  // Read enable for the byte count FIFO.
  // User asserts 1 pulse to read 1 qword from the byte count FIFO for the size
  // of 1 packet.
  NewInput(m, RX_IPCS_RDEN, RX_IPCS_RDEN_BWID);

  // 64-bit IPCS data bus
  // The IPCS bus is only intended for reading 1 qword at a time.
  // Currently:
  // [63:48] = packet’s byte count
  // [47:0] = reserve
  NewState(m, RX_IPCS_DATA, RX_IPCS_DATA_BWID);

  //
  // 2. Receiving Bus - package data interface. (after reading the byte count
  // the user can read out the packet's data - 1 entire packet at a time)
  //

  // Packet data FIFO’s empty signal.  0 = a packet is available for reading. 1
  // = no packet is available
  NewState(m, RX_PKT_EMPTY, RX_PKT_EMPTY_BWID);

  // Read enable for the packet data FIFO. User asserts 1 pulse to read 1 qword
  // from the packet data FIFO.
  NewInput(m, RX_PKT_RDEN, RX_PKT_RDEN_BWID);

  // Indicates a user packet read cycle is in progress.  This signal is used to
  // indicate to the LMAC that a user packet read cycle is in progress which can
  // be lengthy depending on the size of the packet.  Asserts high to start the
  // cycle and only negates when the user’s cycle is fully complete.
  NewInput(m, RX_PKT_RD_CYCLE, RX_PKT_RD_CYCLE_BWID);

  // Packet’s data bus.
  // For each RDEN a qword is sent out to the user from the packet data FIFO.
  // The full qword is always provided.  Based on the packet’s byte count the
  // user can determines how many bytes is available or remaining to be read
  // from the packet. Similar to FPGA FIFO, it takes 1 clock for data to be
  // available from the read enable.
  NewState(m, RX_PKT_DATA, RX_PKT_DATA_BWID);

  return;
}

}; // namespace ilang
