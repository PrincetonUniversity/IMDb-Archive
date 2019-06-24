// File Name: main.cc
//
// Generate ILA portable for oc8051

#include <string>

#include <ilang/ilang++.h>

using namespace ilang;

int main() {

  auto itsy_abst_file = "../../archive/oc8051.abst";
  auto itsy_abst = ImportSynthAbstraction(itsy_abst_file, "oc8051");
  auto portable_file = "../../archive/oc8051.json";
  ExportIlaPortable(itsy_abst, portable_file);

  return 0;
}
