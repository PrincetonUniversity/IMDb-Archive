// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: main.cc

// entry point of the application
//  - create a new ila of LMAC CORE 2
//  - perform sanity check (optional)
//  - export ila portable

#include <fstream>

#include <ilang/util/log.h>

#include <argparse/argparse.hpp>
#include <lmac/core2/eqcheck.h>
#include <lmac/core2/lmac_core_top.h>

using namespace ilang;

int main(int argc, const char** argv) {
  // configure debug log
  LogToErr(true);
  // EnableDebug("LMAC");

  // create LMac Core 2 ILA
  auto core2 = LmacCore2::New();

  // export json target (ILA Portable)
  ExportIlaPortable(core2, "LmacCore2.json");

  // export verilog target (w/o child program)
  std::string verilog_file_name = "LmacCore2.v";
  std::ofstream fw_verilog(verilog_file_name);
  core2.ExportToVerilog(fw_verilog);

  // define the command line parser
  ArgumentParser parser;
  parser.addArgument("-d", "--design_path", 1);
  parser.addArgument("-o", "--output_path", 1);
  parser.addArgument("-i", "--instr_map", 1);
  parser.addArgument("-v", "--var_map", 1);
  parser.parse(argc, argv);

  auto design_path = parser.retrieve<std::string>("design_path");
  auto output_path = parser.retrieve<std::string>("output_path");
  auto instr_map = parser.retrieve<std::string>("instr_map");
  auto var_map = parser.retrieve<std::string>("var_map");

  design_path = (design_path == "") ? "../design/core2" : design_path;
  output_path = (output_path == "") ? "../verification/core2" : output_path;

  // generate verification target
  GenVerifTargetReg(core2, design_path, instr_map, var_map,
                    output_path + "/reg");

  // reset debug config.
  DisableDebug("LMAC");

  return 0;
}
