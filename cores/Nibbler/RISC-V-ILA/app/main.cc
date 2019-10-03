#include "riscvIla.hpp"

using namespace ilang;

/// the function to parse commandline arguments
VerilogVerificationTargetGenerator::vtg_config_t HandleArguments(int argc, char **argv);

/// To verify the exact AES128 ILA
void verifyNibbler(Ila& model, VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;

  vlg_cfg.pass_node_name = true;

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/";

  VerilogVerificationTargetGenerator vg(
      {},                                                    // no include
      {VerilogPath + "aes_abs.v", VerilogPath + "one_round.v",
       VerilogPath + "final_round.v", VerilogPath + "expand_key_128.v",
       VerilogPath + "S.v", VerilogPath + "S4.v", VerilogPath + "xS.v",
       VerilogPath + "table_lookup.v", VerilogPath + "T.v"}, // designs
      "nibbler",                                             // top_module_name
      RefrelPath + "ref-rel-var-map-aes128.json",            // variable mapping
      RefrelPath + "ref-rel-inst-cond-aes128.json",          // conditions of start/ready
      OutputPath,                                            // output path
      model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();
}


int main() {
  // TODO

  riscvILA_user riscvILA(0);
  riscvILA.addInstructions(); // 37 base integer instructions

  return 0;
}

