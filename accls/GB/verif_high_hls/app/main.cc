// ============================================================================
// Instruction-Level Abstraction of Gaussian Blur Accelerator
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// Gaussian Blur Accelerator developed by the Halide Team. Check "LICENSE"
// which comes with this distribution for more information.
// ============================================================================
//
// File Name: main.cc

// entry point of verifying the equivalence between GB-High and HLS Verlog

#include <argparse/argparse.hpp>
#include <gb/eqcheck.h>

#include <fstream>
#include <string>

#include <ilang/ilang++.h>

int main(int argc, const char** argv) {

  std::string json_file = "../../archive/gb_high.json";
  std::string vlog_path = "../../vlog_hls";
  std::string out_path = "../verification";

  auto gb = ilang::ImportIlaPortable(json_file);

  ArgumentParser parser;
  parser.addArgument("-i", "--instr_map", 1);
  parser.addArgument("-v", "--var_map", 1);
  parser.parse(argc, argv);

  auto instr_map = parser.retrieve<std::string>("instr_map");
  auto var_map = parser.retrieve<std::string>("var_map");

  instr_map = (instr_map == "") ? "../refinement/instr.json" : instr_map;
  var_map = (var_map == "") ? "../refinement/var.json" : var_map;

  GenVerifTarget(gb, vlog_path, instr_map, var_map, out_path);

  return 0;
}
