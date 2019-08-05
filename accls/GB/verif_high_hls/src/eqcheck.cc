// ============================================================================
// Instruction-Level Abstraction of Gaussian Blur Accelerator
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// Gaussian Blur Accelerator developed by the Halide Team. Check "LICENSE"
// which comes with this distribution for more information.
// ============================================================================
//
// File Name: eqcheck.cc

// Generate the Verilog verification target for equivalence checking

#include <gb/eqcheck.h>

#include <string>
#include <vector>

#include <ilang/ilang++.h>
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
  config.ExpandMemoryArray = true;
  // config.CosaOtherSolverOptions = "--blackbox-array";
  // config.ForceInstCheckReset = true;

  return config;
}

void GenVerifTarget(Ila& model, const std::string& design_path,
                    const std::string& instr_map, const std::string& var_map,
                    const std::string& output_path) {

  std::vector<std::string> design_files = {
      "FIFO_hls_target_call_slice_stream_V_value_V.v",
      "FIFO_hls_target_linebuffer_1_in_stream_V_value_V.v",
      "FIFO_hls_target_p_p2_in_bounded_stencil_stream_s.v",
      "hls_target.v",
      "hls_target_Loop_1_proc.v",
      "hls_target_call.v",
      "hls_target_call_Loop_LB2D_buf_proc.v",
      "hls_target_call_Loop_LB2D_buf_proc_buffer_0_value_V.v",
      "hls_target_call_Loop_LB2D_shift_proc.v",
      "hls_target_linebuffer.v",
      "hls_target_linebuffer_1.v",
      "hls_target_linebuffer_Loop_1_proc.v",
      "hls_target_mux_8to1_sel3_8_1.v"};

  for (auto i = 0; i < design_files.size(); i++) {
    auto full_path = os_portable_append_dir(design_path, design_files.at(i));
    design_files[i] = full_path;
  }

  auto vlg_cfg = SetVlgGenConfig();
  auto vtg_cfg = SetVerifGenConfig();

  VerilogVerificationTargetGenerator vg(
      {},           // one include path
      design_files, // designs
      "hls_target", // top_module_name
      var_map,      // variable mapping
      instr_map,    // conditions of start/ready
      output_path,  // output path
      model.get(),  // model
      // VerilogVerificationTargetGenerator::backend_selector::COSA,
      VerilogVerificationTargetGenerator::backend_selector::JASPERGOLD,
      vtg_cfg, // target generator configuration
      vlg_cfg  // verilog generator configuration
  );

  vg.GenerateTargets();

  return;
}

}; // namespace ilang
