#include <emesh_axi_master.h>
#include <ilang/vtarget-out/vtarget_gen.h>

/// the function to generate configuration
VerilogVerificationTargetGenerator::vtg_config_t SetConfiguration();

void verifyAxiMaster(
  Ila& model, 
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg,
  const std::vector<std::string> & design_files
   ) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;
  vlg_cfg.pass_node_name = true;

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
      RefrelPath + "varmap-emaxi.json",                      // variable mapping
      RefrelPath + "instcond-emaxi.json",                    // conditions of start/ready
      OutputPath,                                            // output path
      model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();
}


/// Build the model
int main() {
  // extract the configurations
  std::vector<std::string> design_files = {
    "emaxi_trans.v",
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

  auto vtg_cfg = SetConfiguration();

  // build the model
  EmeshAxiMasterBridge emaxi;

  // verifyAxiMaster(emaxi.model, vtg_cfg, design_files);

  return 0;
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