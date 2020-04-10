#include "riscvIla.hpp"
#include <ilang/vtarget-out/vtarget_gen.h>

using namespace ilang;

/// the function to parse commandline arguments
VerilogVerificationTargetGenerator::vtg_config_t SetConfiguration();
VerilogVerificationTargetGenerator::vtg_config_t HandleArguments(int argc, char **argv);

void verifyNibblerInstCosa(
  Ila& model,
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg,
  const std::vector<std::string> & design_files,
  const std::string & varmap,
  const std::string & instcont
  ) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;
  // vlg_cfg.pass_node_name = true;
  vtg_cfg.CosaAddKeep = false;

  vtg_cfg.MemAbsReadAbstraction = true;
  //vtg_cfg.target_select = vtg_cfg.INST;
  //vtg_cfg.ForceInstCheckReset = true;


  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog-cpu/";
  //std::string IncludePath = VerilogPath + "include/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification-noreset/";

  std::vector<std::string> path_to_design_files; // update path
  for(auto && f : design_files)
    path_to_design_files.push_back( VerilogPath + f );

  VerilogVerificationTargetGenerator vg(
      {},                             // no include
      path_to_design_files,           // designs
      "mkCPU",             // top_module_name
      RefrelPath + varmap,            // variable mapping
      RefrelPath + instcont,          // conditions of start/ready
      OutputPath,                     // output path
      model.get(),                    // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();
}




int main(int argc, char **argv) {
  // TODO

  std::vector<std::string> design_files = {
    "FIFO20.v",
    "FIFO2.v",
    "mkCPU.v",
    "mkCSR_MIE.v",
    "mkCSR_MIP.v",
    "mkCSR_RegFile.v",
    "mkGPR_RegFile.v",
    "BRAM2.v",
    "mkMMU_Cache.v",
    "mkNear_Mem.v",
    "mkRISCV_MBox.v",
    "mkSoC_Map.v",
    "RegFile.v"
  };

  auto vtg_cfg = SetConfiguration();
  //auto vtg_cfg = HandleArguments(argc, argv);

  // build the model
  riscvILA_user piccolo;
  piccolo.addInstructions(); // 37 base integer instructions

  verifyNibblerInstCosa(piccolo.model, vtg_cfg, design_files, "varmap-piccolo.json", "instcond-piccolo-multi.json");

  // riscvILA_user riscvILA(0);
  return 0;
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

  /// other configurations
  ret.PortDeclStyle = VlgVerifTgtGenBase::vtg_config_t::AUTO;
  ret.CosaGenJgTesterScript = true;
  //ret.CosaOtherSolverOptions = "--blackbox-array";
  //ret.ForceInstCheckReset = true;

  return ret;
}

VerilogVerificationTargetGenerator::vtg_config_t SetConfiguration() {

  // set ilang option, operators like '<' will refer to unsigned arithmetics
  SetUnsignedComparison(true); 
  
  VerilogVerificationTargetGenerator::vtg_config_t ret;
  ret.CosaSolver = "btor";
  ret.CosaPyEnvironment = "~/cosaEnv/bin/activate";
  ret.CosaGenTraceVcd = true;

  /// other configurations
  ret.PortDeclStyle = VlgVerifTgtGenBase::vtg_config_t::AUTO;
  ret.CosaGenJgTesterScript = true;
  //ret.CosaOtherSolverOptions = "--blackbox-array";
  // ret.ForceInstCheckReset = true;

  return ret;
}

