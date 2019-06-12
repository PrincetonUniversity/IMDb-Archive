// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: reg_interface.cc

// LmacCore2 member function
//  - set up the signals on the register interface

#include <lmac/core2/configs.h>
#include <lmac/core2/lmac_core_top.h>
#include <lmac/utils.h>

namespace ilang {

void LmacCore2::SetupRegInterface(Ila& m) {
  //
  // 1. clocks and reset signals
  //

  //
  // 2. configuration signals
  //

  // Static signal from the LMAC.  If 1, indicating the Ethernet link is up.
  NewState(m, LINKUP, LINKUP_BWID);

  // 32 bit control signals (See MAC_CTRL below)
  NewInput(m, MAC_CTRL, MAC_CTRL_BWID);

  // Second set of 32-bit control signals (See MAC_CTRL1 below)
  NewInput(m, MAC_CTRL1, MAC_CTRL1_BWID);

  // LMAC receive path enable.  This is a static signal.  It allows the user
  // logic to control the receiving function of the LMAC.  It’s useful in debug
  // support. 1 = RX enable
  NewInput(m, MAC_RXD_EN, MAC_RXD_EN_BWID);

  //
  // 3. register interface
  //

  //
  // Host Address Bus
  //

  // 16 bit host byte address bus for selecting a memory mapped register. Valid
  // 1 clk before register read start signal. Address must be 64-bit aligned for
  // most cases.
  NewInput(m, HOST_ADDR, HOST_ADDR_BWID);

  // Pulse. Register read start.
  // 1 = Start the read for a register in the LMAC core 0 = idle.
  NewInput(m, REG_RD_START, REG_RD_START_BWID);

  // Pulse. Indicating the register read is done and its data is available on
  // the MAC_REGDOUT bus. 1 = indicating the data is available for the read to
  // register 0 = data not available. This signal is the 5-clock delay of the
  // register read start signal.
  NewState(m, REG_RD_DONE_OUT, REG_RD_DONE_OUT_BWID);

  //
  // Read Completion Data Bus
  //

  // Data returned from LMAC register. Read is 32 bit at a time.
  NewState(m, MAC_REGDOUT, MAC_REGDOUT_BWID);

  return;
}

}; // namespace ilang

