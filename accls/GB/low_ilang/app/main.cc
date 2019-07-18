// FileName: main.cc
// Generating the ILA portable from the ItSy abstraction.

#include <ilang/ilang++.h>

#include <string>

int main() {

  std::string archive_path = "../../archive";
  auto top_abst_file = archive_path + "/gb_low_top.abst";
  auto child_abst_file = archive_path + "/gb_low_child.abst";
  auto json_file = archive_path + "/gb_low.json";

  // get GB ILA from the legacy Python script (ItSy)
  auto gb = ilang::ImportSynthAbstraction(top_abst_file, "GB_LOW");
  ilang::ImportChildSynthAbstraction(child_abst_file, gb, "GB_LOW_U");

  // export ILA portable
  ilang::ExportIlaPortable(gb, json_file);

  return 0;
}

