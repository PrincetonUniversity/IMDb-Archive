// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: reg_instr.cc

// Register interface and Configuration
//  - read register
//  - write register

#include <lmac/core2/configs.h>
#include <lmac/core2/lmac_core_top.h>

#include <ilang/util/log.h>

namespace ilang {

void RegRdInstr(Ila& m);
void RegWrInstr(Ila& m);

void LmacCore2::SetupRegInstr(Ila& m) {
  // read register
  RegRdInstr(m);

  // write configuration
  RegWrInstr(m);

  return;
}

void RegRdInstr(Ila& m) {

  { // read register FMAC_TX_PKT_CNT
    auto instr = m.NewInstr("REG_RD_FMAC_TX_PKT_CNT");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_TX_PKT_CNT_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_TX_PKT_CNT);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT_LO
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT_LO");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT_LO_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT_LO);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT_HI
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT_HI");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT_HI_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT_HI);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_TX_BYTE_CNT
    auto instr = m.NewInstr("REG_RD_FMAC_TX_BYTE_CNT");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_TX_BYTE_CNT_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_TX_BYTE_CNT);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_BYTE_CNT_LO
    auto instr = m.NewInstr("REG_RD_FMAC_RX_BYTE_CNT_LO");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_BYTE_CNT_LO_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_BYTE_CNT_LO);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_BYTE_CNT_HI
    auto instr = m.NewInstr("REG_RD_FMAC_RX_BYTE_CNT_HI");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_BYTE_CNT_HI_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_BYTE_CNT_HI);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_UNDERSIZE_PKT_CNT
    auto instr = m.NewInstr("REG_RD_FMAC_RX_UNDERSIZE_PKT_CNT");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_UNDERSIZE_PKT_CNT_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_UNDERSIZE_PKT_CNT);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_CRC32_ERR_CNT
    auto instr = m.NewInstr("REG_RD_FMAC_RX_CRC32_ERR_CNT");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_CRC32_ERR_CNT_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_CRC32_ERR_CNT);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_DCNT_OVERRUN
    auto instr = m.NewInstr("REG_RD_FMAC_RX_DCNT_OVERRUN");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_DCNT_OVERRUN_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_DCNT_OVERRUN);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_DCNT_LINK_ERR
    auto instr = m.NewInstr("REG_RD_FMAC_RX_DCNT_LINK_ERR");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_DCNT_LINK_ERR_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_DCNT_LINK_ERR);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT_OVERSIZE
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT_OVERSIZE");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT_OVERSIZE_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT_OVERSIZE);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_PHY_STAT
    auto instr = m.NewInstr("REG_RD_FMAC_PHY_STAT");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_PHY_STAT_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_PHY_STAT);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT_JABBER
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT_JABBER");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT_JABBER_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT_JABBER);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT_FRAGMENT
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT_FRAGMENT");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT_FRAGMENT_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT_FRAGMENT);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_RAW_FRAME_CNT
    auto instr = m.NewInstr("REG_RD_FMAC_RX_RAW_FRAME_CNT");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_RAW_FRAME_CNT_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_RAW_FRAME_CNT);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_BAD_FRAME_CNT
    auto instr = m.NewInstr("REG_RD_FMAC_RX_BAD_FRAME_CNT");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_BAD_FRAME_CNT_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_BAD_FRAME_CNT);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT64_LO
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT64_LO");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT64_LO_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT64_LO);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT64_HI
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT64_HI");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT64_HI_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT64_HI);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT127_LO
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT127_LO");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT127_LO_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT127_LO);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT127_HI
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT127_HI");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT127_HI_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT127_HI);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT255_LO
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT255_LO");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT255_LO_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT255_LO);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT255_HI
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT255_HI");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT255_HI_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT255_HI);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT511_LO
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT511_LO");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT511_LO_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT511_LO);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT511_HI
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT511_HI");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT511_HI_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT511_HI);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT1023_LO
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT1023_LO");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT1023_LO_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT1023_LO);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT1023_HI
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT1023_HI");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT1023_HI_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT1023_HI);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT1518_LO
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT1518_LO");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT1518_LO_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT1518_LO);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  { // read register FMAC_RX_PKT_CNT1518_HI
    auto instr = m.NewInstr("REG_RD_FMAC_RX_PKT_CNT1518_HI");

    // decode
    auto addr_match = (m.input(HOST_ADDR) == FMAC_RX_PKT_CNT1518_HI_ADDR);
    auto is_reg_read = (m.input(REG_RD_START) == REG_RD_START_V_BUSY);
    instr.SetDecode(addr_match & is_reg_read);

    // update
    auto data_out_var = m.state(MAC_REGDOUT);
    auto data_out_nxt = m.state(FMAC_RX_PKT_CNT1518_HI);
    instr.SetUpdate(data_out_var, data_out_nxt);

    auto done_signal_var = m.state(REG_RD_DONE_OUT);
    auto done_signal_nxt = BvConst(1, REG_RD_DONE_OUT_BWID);
    instr.SetUpdate(done_signal_var, done_signal_nxt);
  }

  return;
}

void RegWrInstr(Ila& m) {
  //

  return;
}

}; // namespace ilang
