#include <emesh_axi_master.h>
#include <emesh_axi_slave.h>
#include <ilang/vtarget-out/vtarget_gen.h>

/// the function to generate configuration
VerilogVerificationTargetGenerator::vtg_config_t SetConfiguration();
VerilogVerificationTargetGenerator::vtg_config_t HandleArguments(int argc, char **argv);

void verifyAxiMasterRW(
  Ila& model, 
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg,
  const std::vector<std::string> & design_files,
  const std::string & varmap,
  const std::string & instcont
   ) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;
  vlg_cfg.pass_node_name = true;
  vtg_cfg.CosaAddKeep = false;

  std::string RootPath    = "..";
  std::string VerilogPath = RootPath    + "/verilog/";
  std::string IncludePath = VerilogPath + "include/";
  std::string RefrelPath  = RootPath    + "/refinement/";
  std::string OutputPath  = RootPath    + "/verification/";

  std::vector<std::string> path_to_design_files; // update path
  for(auto && f : design_files)
    path_to_design_files.push_back( VerilogPath + f );
  

  VerilogVerificationTargetGenerator vg(
      {},                                                    // one include path
      path_to_design_files,                                  // designs
      "emaxi",                                               // top_module_name
      RefrelPath + varmap,                      // variable mapping
      RefrelPath + instcont,                    // conditions of start/ready
      OutputPath,                                            // output path
      model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();
}

void verifyAxiSlaveRW(
  Ila& model, 
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg,
  const std::vector<std::string> & design_files,
  const std::string & varmap,
  const std::string & instcont
   ) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;
  vlg_cfg.pass_node_name = true;
  vtg_cfg.CosaAddKeep = false;

  std::string RootPath    = "..";
  std::string VerilogPath = RootPath    + "/verilog/";
  std::string IncludePath = VerilogPath + "include/";
  std::string RefrelPath  = RootPath    + "/refinement/";
  std::string OutputPath  = RootPath    + "/slave_verification/";

  std::vector<std::string> path_to_design_files; // update path
  for(auto && f : design_files)
    path_to_design_files.push_back( VerilogPath + f );
  

  VerilogVerificationTargetGenerator vg(
      {},                                                    // one include path
      path_to_design_files,                                  // designs
      "esaxi",                                               // top_module_name
      RefrelPath + varmap,                      // variable mapping
      RefrelPath + instcont,                    // conditions of start/ready
      OutputPath,                                            // output path
      model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();
}


/// Build the model
int main(int argc, char **argv) {
  // extract the configurations
  std::vector<std::string> design_files = {
    "emaxi.v",
    "emesh2packet.v",
    "em_se.v",
    "esaxi.v",
    "oh_dsync.v",
    "oh_fifo_sync.v",
    "oh_memory_dp.v",
    "oh_memory_ram.v",
    "packet2emesh.v"
  };

  //auto vtg_cfg = SetConfiguration();
  auto vtg_cfg = HandleArguments(argc, argv);

  // build the model
  EmeshAxiMasterBridge emaxi;
  EmeshAxiSlaveBridge esaxi;

  verifyAxiMasterRW(emaxi.wmodel, vtg_cfg, design_files, "varmap-emaxi-write.json", "instcond-emaxi-write.json");
  verifyAxiMasterRW(emaxi.rmodel, vtg_cfg, design_files, "varmap-emaxi-read.json",  "instcond-emaxi-read.json");

  verifyAxiSlaveRW(esaxi.wmodel, vtg_cfg, design_files, "varmap-esaxi-write.json", "instcond-esaxi-write.json");
  verifyAxiSlaveRW(esaxi.rmodel, vtg_cfg, design_files, "varmap-esaxi-read.json",  "instcond-esaxi-read.json");

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
  ret.PortDeclStyle = VlgVerifTgtGenBase::vtg_config_t::NEW;
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
  ret.CosaPath = "~/CoSA";
  ret.CosaGenTraceVcd = true;

  /// other configurations
  ret.PortDeclStyle = VlgVerifTgtGenBase::vtg_config_t::NEW;
  ret.CosaGenJgTesterScript = true;
  //ret.CosaOtherSolverOptions = "--blackbox-array";
  //ret.ForceInstCheckReset = true;

  return ret;
}