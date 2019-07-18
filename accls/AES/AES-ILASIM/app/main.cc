// main.cc
// Synopsis: Entry point for the executable

#include <simple/lib.h>
#include "aes_ila.h"
#include <iostream>
#include <ilang/ila/instr_lvl_abs.h>
#include <ilang/target-sc/ila_sim.h>

int main(int argc, char *argv[]) {
  AES aes_model;
  auto model = aes_model.model.get();
  IlaSim simulator_generator;
  string sim_gen_dir = argv[1];
  string systemc_path = argv[2];
  bool cpp_gen = false;
  simulator_generator.set_instr_lvl_abs(model);
  simulator_generator.set_systemc_path(systemc_path);
  simulator_generator.sim_gen(sim_gen_dir, false, true, cpp_gen);

  return 0;
}
