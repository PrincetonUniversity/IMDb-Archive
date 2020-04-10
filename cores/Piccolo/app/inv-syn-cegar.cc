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
  const std::string & instcont,
  bool restore_inv
  ) {
  VerilogGeneratorBase::VlgGenConfig vlg_cfg; 
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
  std::string VerilogPath = RootPath + "/verilog-cpu/";
  //std::string IncludePath = VerilogPath + "include/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification-invsyn/";
  std::string SmtFile    = OutputPath + "smt/__design_smt.smt2";

  std::vector<std::string> path_to_design_files; // update path
  for(auto && f : design_files)
    path_to_design_files.push_back( VerilogPath + f );

  InvariantSynthesizerCegar vg(
      {},                             // no include
      path_to_design_files,           // designs
      "mkCPU",             // top_module_name
      RefrelPath + varmap,            // variable mapping
      RefrelPath + instcont,          // conditions of start/ready
      OutputPath,                     // output path
      model.get(),                    // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      VerilogVerificationTargetGenerator::synthesis_backend_selector::Z3, // synthesis backend: Z3
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  std::vector<std::string> to_drop_states = {
    "m1.csr_regfile.rg_mcycle[63:0]",
    "m1.csr_regfile.rg_mepc[31:0]",
    "m1.csr_regfile.rg_minstret[63:0]",
    "m1.csr_regfile.rg_mscratch[31:0]",
    "m1.csr_regfile.rg_mtval[31:0]",
    "m1.csr_regfile.rg_mtvec[30:0]",
    "m1.near_mem.icache.f_fabric_write_reqs.data0_reg[98:0]",
    "m1.near_mem.icache.f_fabric_write_reqs.data1_reg[98:0]",
    "m1.near_mem.icache.master_xactor_f_rd_addr.data0_reg[96:0]",
    "m1.near_mem.icache.master_xactor_f_rd_addr.data1_reg[96:0]",
    "m1.near_mem.icache.master_xactor_f_rd_data.data0_reg[70:0]",
    "m1.near_mem.icache.master_xactor_f_rd_data.data1_reg[70:0]",
    "m1.near_mem.icache.master_xactor_f_wr_addr.data0_reg[96:0]",
    "m1.near_mem.icache.master_xactor_f_wr_addr.data1_reg[96:0]",
    "m1.near_mem.icache.master_xactor_f_wr_data.data0_reg[72:0]",
    "m1.near_mem.icache.master_xactor_f_wr_data.data1_reg[72:0]",
    "m1.near_mem.icache.ram_state_and_ctag_cset.DOA_R[22:0]",
    "m1.near_mem.icache.ram_state_and_ctag_cset.DOB_R[22:0]",
    "m1.near_mem.icache.ram_word64_set.DOA_R[63:0]",
    "m1.near_mem.icache.ram_word64_set.DOB_R[63:0]",
    "m1.near_mem.icache.rg_ld_val[63:0]",
    "m1.stage2_mbox.intDiv_rg_denom2[31:0]",
    "m1.stage2_mbox.intDiv_rg_n[31:0]",
    "m1.stage2_mbox.intDiv_rg_quo[31:0]",
    "m1.stage2_mbox.rg_result[31:0] ",
    "m1.stage2_mbox.rg_v1[31:0]",
    "m1.stage2_mbox.rg_v2[31:0]",

    "m1.near_mem.dcache.master_xactor_f_rd_addr.data0_reg[96:0]",
    "m1.near_mem.dcache.master_xactor_f_rd_addr.data1_reg[96:0]",
    "m1.near_mem.dcache.master_xactor_f_rd_data.data0_reg[70:0]",
    "m1.near_mem.dcache.master_xactor_f_rd_data.data1_reg[70:0]",
    "m1.near_mem.dcache.master_xactor_f_wr_addr.data0_reg[96:0]",
    "m1.near_mem.dcache.master_xactor_f_wr_addr.data1_reg[96:0]",
    "m1.near_mem.dcache.master_xactor_f_wr_data.data0_reg[72:0]",
    "m1.near_mem.dcache.master_xactor_f_wr_data.data1_reg[72:0]",
    "m1.near_mem.dcache.ram_state_and_ctag_cset.DOB_R[22:0]",
    "m1.near_mem.dcache.ram_word64_set.DOB_R[63:0]",

  }; // 



  vg.LoadDesignSmtInfo(SmtFile);

  if (restore_inv)
    vg.LoadInvariantsFromFile(OutputPath + "inv-syn-stage1.txt");

  unsigned ncegar = 0;
  std::vector<std::string> insts({
    "ADD"});

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
    break; // just run for one instruction
  }
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

  bool restore_inv = false;
  for (int argi = 1; argi<argc; ++ argi) {
    if (std::string(argv[argi]) == "restore") {
      std::cout << "Will restore inv from file." << std::endl;
      restore_inv = true;
    }
  }

  auto vtg_cfg = SetConfiguration();
  //auto vtg_cfg = HandleArguments(argc, argv);

  // build the model
  riscvILA_user piccolo;
  piccolo.addInstructions(); // 37 base integer instructions

  verifyNibbler(piccolo.model, vtg_cfg, design_files, "varmap-piccolo.json", "instcond-piccolo.json", restore_inv);


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

