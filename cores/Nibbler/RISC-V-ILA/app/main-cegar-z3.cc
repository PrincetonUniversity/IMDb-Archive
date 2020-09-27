#include "riscvIla.hpp"
#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>

using namespace ilang;

/// the function to parse commandline arguments
VerilogVerificationTargetGenerator::vtg_config_t SetConfiguration();
VerilogVerificationTargetGenerator::vtg_config_t HandleArguments(int argc, char **argv);

/// To verify the exact AES128 ILA
void verifyNibbler(
  Ila& model, 
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg,
  const std::vector<std::string> & design_files,
  const std::string & varmap,
  const std::string & instcont
  ) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg; 
  vlg_cfg.pass_node_name = true;
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.InvariantSynthesisReachableCheckKeepOldInvariant = false;
  vtg_cfg.InvariantSynthesisKeepMemory = false;
  vtg_cfg.InvariantCheckKeepMemory = false;

  vtg_cfg.MemAbsReadAbstraction = true;
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.VerificationSettingAvoidIssueStage = true;
  vtg_cfg.YosysSmtFlattenDatatype = false;
  vtg_cfg.YosysSmtFlattenHierarchy = true;
  vtg_cfg.CosaPyEnvironment = "/home/hongce/cosaEnv/bin/activate";
  vtg_cfg.CosaPath = "/home/hongce/CoSA/";
  vtg_cfg.AbcPath = "/home/hongce/abc/";
  vtg_cfg.AbcUseGla = false; // gla will result in too coarse approximate 
  vtg_cfg.AbcUseAiger = true;
  vtg_cfg.AbcAssumptionStyle = vtg_cfg.AssumptionRegister; // you have to do it this way!
  vtg_cfg.CosaSolver = "btor";
  //vtg_cfg.ForceInstCheckReset = true;
  


  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string IncludePath = VerilogPath + "include/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification-invsyn/";

  std::vector<std::string> path_to_design_files; // update path
  for(auto && f : design_files)
    path_to_design_files.push_back( VerilogPath + f );

  InvariantSynthesizerCegar vg(
      {IncludePath},                             // no include
      path_to_design_files,           // designs
      "param_riscv_Core",             // top_module_name
      RefrelPath + varmap,            // variable mapping
      RefrelPath + instcont,          // conditions of start/ready
      OutputPath,                     // output path
      model.get(),                    // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      VerilogVerificationTargetGenerator::synthesis_backend_selector::Z3, // synthesis backend: Z3
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  std::vector<std::string> to_drop_states = {
  }; // 

  unsigned ncegar = 0;
  std::vector<std::string> insts({
    "ADD",
    "JALR",
    "BNE",
    "OR",
    "XOR",
    "SLT",
    "ADDI",
    "ANDI",
    "ORI",
    "XORI",
    "SLTI",
    "LUI",
    "AUIPC",
    "SUB",
    "AND"});

  for (auto && inst : insts) {
    std::cout << "------------------- " << inst << "------------------- " << std::endl;
    do{
      vg.GenerateVerificationTarget();
      if(vg.RunVerifAuto("/"+inst+"/")) {// the OPERATE 
        std::cerr << "No more Cex has been found! Cegar completes." << std::endl;
        break; // no more cex found
      }
      vg.ExtractVerificationResult();
      vg.CexGeneralizeRemoveStates(to_drop_states);
      vg.GenerateSynthesisTarget();
      if(vg.RunSynAuto()) {
        std::cerr << "Cex is reachable! Cegar failed" << std::endl;
        break; // cex is really reachable!!!
      }
      vg.ExtractSynthesisResult();
      vg.GenerateInvariantVerificationTarget();


      InvariantObject invs(vg.GetInvariants());
      invs.ExportToFile(OutputPath+"inv-syn.txt");
      ncegar ++;

    }while(not vg.in_bad_state());


     auto design_stat = vg.GetDesignStatistics();
     std::cout << "========== Design Info =========="  << std::endl;
     std::cout << "#bits=" << design_stat.NumOfDesignStateBits << std::endl;
     std::cout << "#vars=" << design_stat.NumOfDesignStateVars << std::endl;
     std::cout << "#extra_bits=" << design_stat.NumOfExtraStateBits << std::endl;
     std::cout << "#extra_vars=" << design_stat.NumOfExtraStateVars << std::endl;
     std::cout << "t(eq)= " << design_stat.TimeOfEqCheck << std::endl;
     std::cout << "t(syn)=" << design_stat.TimeOfInvSyn << std::endl;
     std::cout << "t(proof)= " << design_stat.TimeOfInvProof << std::endl;
     std::cout << "t(validate)=" << design_stat.TimeOfInvValidate << std::endl;
     std::cout << "#(cegar)=" << ncegar << std::endl;

    std::cout << "------------------- END of " << inst << "------------------- " << std::endl;
  }
}


int main(int argc, char **argv) {
  // TODO

  std::vector<std::string> design_files = {
    "param-Ctrl.v",
    "param-ShiftDemux.v",
    "param-CoreDpathRegfile.v",
    "param-CoreDpathAlu.v",
    "param-SIMDLaneDpath.v",
    "param-ClkEnBuf.v",
    "param-DeserializedReg.v",
    "param-PCComputation.v",
    "param-Dpath.v",
    "param-Core.v"
  };

  auto vtg_cfg = SetConfiguration();
  //auto vtg_cfg = HandleArguments(argc, argv);

  // build the model
  riscvILA_user nibbler;
  nibbler.addInstructions(); // 37 base integer instructions

  verifyNibbler(nibbler.model, vtg_cfg, design_files, "varmap-nibbler.json", "instcond-nibbler-noinv.json");

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
  ret.CosaPyEnvironment = "/ibuild/ilang-env/bin/activate";
  ret.CosaGenTraceVcd = true;

  /// other configurations
  ret.PortDeclStyle = VlgVerifTgtGenBase::vtg_config_t::NEW;
  ret.CosaGenJgTesterScript = true;
  //ret.CosaOtherSolverOptions = "--blackbox-array";
  //ret.ForceInstCheckReset = true;

  return ret;
}

