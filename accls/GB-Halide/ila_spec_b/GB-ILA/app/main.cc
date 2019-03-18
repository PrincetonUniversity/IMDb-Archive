// main.cc
// Synopsis: Entry point for the executable

#include <gb/gb.h>

int main() {

  // get GB ILA from legacy Python scripts
  auto gb_legacy = ilang::GetGbIlaFromSynthEngine();

  return 0;
}

