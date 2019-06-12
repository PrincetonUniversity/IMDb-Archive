// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: eqcheck.h

// Generate Verilog verification target for equivalence checking
//  - SetVerifConfigs
//  - GenVerifTarget

#ifndef LMAC_CORE2_EQCHECK_H__
#define LMAC_CORE2_EQCHECK_H__

#include <ilang/ilang++.h>
#include <ilang/vtarget-out/vtarget_gen.h>

namespace ilang {

// verilog generator configs
VerilogGeneratorBase::VlgGenConfig SetVlgGenConfig();

// verification target generator configs
VerilogVerificationTargetGenerator::vtg_config_t SetVerifGenConfig();

// generate Verilog verification target for reg instr
void GenVerifTargetReg(Ila& model, const std::string& design_path,
                       const std::string& instr_map, const std::string& var_map,
                       const std::string& output_path);

}; // namespace ilang

#endif // LMAC_CORE2_EQCHECK_H__
