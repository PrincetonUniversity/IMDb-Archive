// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: eqcheck.cc

// Generate Verilog verification target for equivalence checking
//  - SetVlgGenConfig
//  - SetVerifGenConfig

#include <lmac/core2/eqcheck.h>

#include <ilang/util/fs.h>

namespace ilang {

VerilogGeneratorBase::VlgGenConfig SetVlgGenConfig() {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;

  vlg_cfg.pass_node_name = true;

  return vlg_cfg;
}

VerilogVerificationTargetGenerator::vtg_config_t SetVerifGenConfig() {
  auto config = VerilogVerificationTargetGenerator::vtg_config_t();

  // ilang
  SetUnsignedComparison(true);

  // CoSA
  config.CosaSolver = "z3";
  config.CosaPyEnvironment = "$HOME/cosa_ve/bin/activate";
  config.CosaGenTraceVcd = true;
  // config.CosaPath = "$HOME/CoSA";

  // other
  config.PortDeclStyle = VlgVerifTgtGenBase::vtg_config_t::NEW;
  config.CosaGenJgTesterScript = true;
  // config.CosaOtherSolverOptions = "--blackbox-array";
  // config.ForceInstCheckReset = true;

  return config;
}

}; // namespace ilang
