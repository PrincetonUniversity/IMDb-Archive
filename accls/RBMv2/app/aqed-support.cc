#include <rbm_ila.h>
#include <ilang/aqed-out/aqed_out.h>


/// Build the model
int main(int argc, char **argv) {

  SetUnsignedComparison(true); 
  // build the aes model
  RBM rbm_ila;


  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/aqed/";

  auto aqed_out = AQedInfoGenerator::Create(
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
      rbm_ila.model.get());

  aqed_out->ExportVerilogTopLevelIOInfo(OutputPath + "io.txt");
  aqed_out->ExportInstructionAndDecode (OutputPath + "decoder.v");
  aqed_out->ExportExtraSignalReferenced(OutputPath + "var_ref.txt");

  return 0;
}


