#include <aes_128.h>
#include <aes_ila.h>
#include <ilang/aqed-out/aqed_out.h>


/// Build the model
int main(int argc, char **argv) {

  SetUnsignedComparison(true); 
  // build the aes model
  AES aes_ila_model;


  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/aqed/";

  auto aqed_out = AQedInfoGenerator::Create(
      {}, // no include
      {VerilogPath + "aes_top.v",   VerilogPath + "reg2byte.v",
       VerilogPath + "reg16byte.v", VerilogPath + "reg32byte.v",
       VerilogPath + "reg256byte.v",
       VerilogPath + "aes_128_abs.v"},                // designs
      "aes_top",                                      // top_module_name
      RefrelPath + "ref-rel-var-map.json",            // variable mapping
      RefrelPath + "ref-rel-inst-cond.json",          // conditions of start/ready
      aes_ila_model.model.get());

  aqed_out->ExportVerilogTopLevelIOInfo(OutputPath + "io.txt");
  aqed_out->ExportInstructionAndDecode (OutputPath + "decoder.v");
  aqed_out->ExportExtraSignalReferenced(OutputPath + "var_ref.txt");

  return 0;
}


