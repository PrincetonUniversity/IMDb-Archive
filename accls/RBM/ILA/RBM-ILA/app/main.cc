// main.cc
// Synopsis: Entry point for the executable

#include <rbm/rbm.h>

int main() {

  // XXX RBM ILA has not been rewritten
  // auto m = GetRbmIla();

  // get RBM ILA from legacy Python scripts
  auto rbm_legacy = ilang::GetRbmIlaFromSynthEngine();

  return 0;
}

