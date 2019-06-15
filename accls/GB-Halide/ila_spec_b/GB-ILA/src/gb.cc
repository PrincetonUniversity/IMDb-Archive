// gb.cc
// Synopsis: implementation of the Gaussian Blur library

#include <gb/gb.h>
#include <ilang/target-itsy/interface.h>
#include <ilang/target-json/interface.h>
#include <ilang/util/log.h>

namespace ilang {

InstrLvlAbsPtr GetGbIlaFromSynthEngine() {
  SetToStdErr(1);
  // EnableDebug("SynthImport");
  // EnableDebug("Portable");

  std::string archive_dir = "../../archive";

  // parent
  auto parent_file = archive_dir + "/gb_top.ila";
  auto parent = ImportSynthAbsFromFile(parent_file, "gb_top");
  ILA_NOT_NULL(parent);

  // parent --> child
  auto child_file = archive_dir + "/gb_child.ila";
  auto child = ImportSynthAbsFromFileHier(child_file, parent, "gb_child");
  ILA_NOT_NULL(child);

  // export
  IlaSerDesMngr::SerToFile(parent, "gb_low.json");

  auto test = IlaSerDesMngr::DesFromFile("gb_low.json");

  return parent;
}

}; // namespace ilang

