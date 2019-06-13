// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: tx_internal.cc

// TX FIFO internal state variables
//  - non-interface
//  - implementation specific

#include <lmac/core2/lmac_core_top.h>

#include <lmac/core2/configs.h>
#include <lmac/utils.h>

namespace ilang {

void LmacCore2::SetupTxInternal(Ila& m) {
  // TX FIFO internal buffer
  auto tx_fifo_buff =
      m.NewMemState(TXFIFO_BUFF, TXFIFO_BUFF_ADDR_BWID, TXFIFO_BUFF_DATA_BWID);
  tx_fifo_buff.SetEntryNum(TXFIFO_BUFF_DEPTH);

  // read pointer
  NewState(m, TXFIFO_BUFF_RD_PTR, TXFIFO_BUFF_RD_PTR_BWID);

  // write pointer
  NewState(m, TXFIFO_BUFF_WR_PTR, TXFIFO_BUFF_WR_PTR_BWID);

  return;
}

}; // namespace ilang

