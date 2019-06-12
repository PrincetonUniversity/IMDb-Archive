// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: eqcheck_reg.cc

// Generate Verilog verification target for equivalence checking reg instr
//  - GenVerifTargetReg

#include <lmac/core2/eqcheck.h>

#include <ilang/util/fs.h>

namespace ilang {

void GenVerifTargetReg(Ila& model, const std::string& design_path,
                       const std::string& instr_map, const std::string& var_map,
                       const std::string& output_path) {

  std::vector<std::string> design_files = {
      "LMAC_CORE_TOP.v",
      "asynch_fifo_reg.v", // XXX change accordingly
      "br_sfifo4x32.v",
      "bsh32_dn_88.v",
      "bsh8_dn_64.v",
      "byte_reordering.v",
      "crc32_d16s.v",
      "crc32_d24s.v",
      "crc32_d64.v",
      "crc32_d8s.v",
      "ctrl_2G_5G.v",
      "eth_crc32_gen.v",
      "fmac_fifo4Kx64.v",
      "fmac_fifo4Kx8.v",
      "fmac_fifo512x64_2clk.v",
      "fmac_register_if_LE2.v",
      //"fmac_saddr_filter.v",
      "g2x_ctrl.v",
      "gige_crc32x64.v",
      "gige_s2p.v",
      "gige_tx_encap.v",
      "gige_tx_gmii.v",
      "gigerx_bcnt_fifo256x16.v",
      "gigerx_fifo256x64_2clk.v",
      "gigerx_fifo256x8.v",
      "rx_5G.v",
      "rx_decap_LE2.v",
      "rx_xgmii_LE2.v",
      "tcore_fmac_core_LE2.v",
      "tx_10G_wrap.v",
      "tx_1G_wrap.v",
      "tx_encap.v",
      "tx_mac10g_crc32x64.v",
      "tx_xgmii_LE2.v",
      "txfifo_1024x64.v",
  };

  for (auto i = 0; i < design_files.size(); i++) {
    auto full_path = os_portable_append_dir(design_path, design_files.at(i));
    design_files[i] = full_path;
  }

  auto vlg_cfg = SetVlgGenConfig();
  auto vtg_cfg = SetVerifGenConfig();

  VerilogVerificationTargetGenerator vg(
      {},              // one include path
      design_files,    // designs
      "LMAC_CORE_TOP", // top_module_name
      var_map,         // variable mapping
      instr_map,       // conditions of start/ready
      output_path,     // output path
      model.get(),     // model
      // VerilogVerificationTargetGenerator::backend_selector::COSA,
      VerilogVerificationTargetGenerator::backend_selector::JASPERGOLD,
      vtg_cfg, // target generator configuration
      vlg_cfg  // verilog generator configuration
  );

  vg.GenerateTargets();

  return;
}

}; // namespace ilang
