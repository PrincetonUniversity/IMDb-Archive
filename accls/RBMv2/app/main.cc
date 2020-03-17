// main.cc
// Synopsis: Entry point for the executable

#include <rbm_ila.h>
#include <ilang/vtarget-out/vtarget_gen.h>


/// the function to parse commandline arguments
VerilogVerificationTargetGenerator::vtg_config_t HandleArguments(int argc, char **argv);
VerilogVerificationTargetGenerator::vtg_config_t LocalArguments();

/// To verify the exact RBM top-level ILA
void verifyRBMTopLevel(Ila& model, VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;
  
  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.VerificationSettingAvoidIssueStage = true;
  vtg_cfg.CosaGenJgTesterScript = true;
  vtg_cfg.ForceInstCheckReset = false;

  vlg_cfg.pass_node_name = true;

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/";

  VerilogVerificationTargetGenerator vg(
      {},                                                    // no include
      { VerilogPath + "mem_rbm_0_cmos32soi_rtl.v",
        VerilogPath + "rbm_0_cmos32soi_rtl.v",
        VerilogPath + "SRAM2S_1024X16.v",
        VerilogPath + "SRAM2S_1024X32.v",
        VerilogPath + "SRAM2S_128X16.v",
        VerilogPath + "SRAM2S_8192X16.v",
        VerilogPath + "top_rbm_0_cmos32soi_rtl.v"}, // designs
      "rbm_0_top_cmos32soi_rtl",                             // top_module_name
      RefrelPath + "var-map-top.json",            // variable mapping
      RefrelPath + "inst-cond-top.json",          // conditions of start/ready
      OutputPath,                                            // output path
      model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();
}


int main(int argc, char **argv) {

  auto vtg_cfg = LocalArguments();

  RBM rbm_ila;
  
  verifyRBMTopLevel(rbm_ila.model, vtg_cfg);

  return 0;
}



VerilogVerificationTargetGenerator::vtg_config_t LocalArguments() {
  
  VerilogVerificationTargetGenerator::vtg_config_t ret;
  ret.CosaSolver = "btor";
  ret.CosaPyEnvironment = "~/cosaEnv/bin/activate";
  ret.CosaPath = "~/CoSA/";
  return ret;
}


VerilogVerificationTargetGenerator::vtg_config_t HandleArguments(int argc, char **argv) {
  // the solver, the cosa environment
  // you can use a commandline parser if desired, but since it is not the main focus of
  // this demo, we skip it

  // set ilang option, operators like '<' will refer to unsigned arithmetics
  SetUnsignedComparison(true); 
  
  VerilogVerificationTargetGenerator::vtg_config_t ret;

  for(unsigned p = 1; p<argc; p++) {
    std::string arg = argv[p];
    auto split = arg.find("=");
    auto argName = arg.substr(0,split);
    auto param   = arg.substr(split+1);

    if(argName == "Solver")
      ret.CosaSolver = param;
    else if(argName == "Env")
      ret.CosaPyEnvironment = param;
    else if(argName == "Cosa")
      ret.CosaPath = param;
    // else unknown
    else {
      std::cerr<<"Unknown argument:" << argName << std::endl;
      std::cerr<<"Expecting Solver/Env/Cosa=???" << std::endl;
    }
  }

  ret.CosaGenTraceVcd = true;

  return ret;
}