// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: lmac_core_top.cc

// LmacCore2 top level entry points
//  - TX FIFO
//  - RX FIFO
//  - Register interface and configuration
//  - PHY

#include <lmac/core2/lmac_core_top.h>

#include <ilang/util/log.h>
#include <lmac/core2/configs.h>

namespace ilang {

LmacCore2::LmacCore2() {}

LmacCore2::~LmacCore2() {}

Ila LmacCore2::New(const std::string& name) {
  ILA_DLOG("LMAC") << "Create ILA with name " << name;
  auto m = Ila(name);

  // state vars
  SetArchStateVar(m);
  SetImplStateVar(m);

  // model hierarchy
  SetChild(m);

  // instruction decode & updates
  SetInstr(m);

  // initial condition
  SetInit(m);

  { // valid
    // TX FIFO
    auto tx_valid = (m.input(TX_WE) == TX_WE_V_VALID) &
                    (m.state(TXFIFO_FULL) != TXFIFO_FULL_V_FULL);
    // RX FIFO
    auto rx_valid = true;

    // Reg interface
    auto reg_itf_valid = (m.input(REG_RD_START) == REG_RD_START_V_BUSY) &
                         (m.input(HOST_ADDR) >= FMAC_TX_PKT_CNT_ADDR) &
                         (m.input(HOST_ADDR) <= FMAC_RX_PKT_CNT1518_HI_ADDR);

    // PHY
    auto phy_valid = true;

    auto valid = tx_valid | rx_valid | reg_itf_valid | phy_valid;
    m.SetValid(valid);
  }

  { // fetch
    // TX FIFO
    auto tx_fetch = (m.input(TX_WE));

    // RX FIFO
    auto rx_fetch = BoolConst(true);

    // Reg interface
    auto reg_fetch = BoolConst(true);

    // PHY
    auto phy_fetch = BoolConst(true);

    auto fetch =
        Concat(tx_fetch, Concat(rx_fetch, Concat(reg_fetch, phy_fetch)));
    m.SetFetch(fetch);
  }

  ILA_DLOG("LMAC") << "Done";
  return m;
}

void LmacCore2::SetArchStateVar(Ila& m) {
  ILA_DLOG("LMAC") << "Setup state variables from spec.";

  // TX interface signals
  SetupTxInterface(m);

  // RX interface signals
  SetupRxInterface(m);

  // Register interface and configuration
  SetupRegInterface(m);
  SetupMmioRegs(m);

  // PHY interface signals

  return;
}

void LmacCore2::SetImplStateVar(Ila& m) {
  ILA_DLOG("LMAC") << "Setup state variables (impl. specific)";

  // TX FIFO internal states
#if 0 // TODO
  SetupTxInternal(m);
#endif

  // RX FIFO internal states

  // Register and configuration

  // PHY internal states

  return;
}

void LmacCore2::SetChild(Ila& m) {
  ILA_DLOG("LMAC") << "Setup child ILAs";

  return;
}

void LmacCore2::SetInstr(Ila& m) {
  ILA_DLOG("LMAC") << "Setup instructions";

  // TX FIFO instructions
  SetupTxInstr(m);

  // RX FIFO instructions

  // Register and configuration instructions
  SetupRegInstr(m);

  // PHY instructions

  return;
}

void LmacCore2::SetInit(Ila& m) {
  ILA_DLOG("LMAC") << "Setup initial condition";

  return;
}

}; // namespace ilang
