// ============================================================================
// Instruction-Level Abstraction of Gaussian Blur Accelerator
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// Gaussian Blur Accelerator developed by the Halide Team. Check "LICENSE"
// which comes with this distribution for more information.
// ============================================================================
//
// File Name: eqcheck.h

#ifndef GB_EQCHECK_H__
#define GB_EQCHECK_H__

#include <ilang/ilang++.h>
#include <ilang/vtarget-out/vtarget_gen.h>

namespace ilang {

// verilog generator configs
VerilogGeneratorBase::VlgGenConfig SetVlgGenConfig();

// verification target generator configs
VerilogVerificationTargetGenerator::vtg_config_t SetVerifGenConfig();

// generate Verilog verification target for reg instr
void GenVerifTarget(Ila& model, const std::string& design_path,
                    const std::string& instr_map, const std::string& var_map,
                    const std::string& output_path);

}; // namespace ilang

#endif // GB_EQCHECK_H__
