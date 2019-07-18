// FileName: main.cc
// Generating the ILA portable from the ItSy abstraction.

#include <ilang/ilang++.h>

#include <string>

int main() {

  std::string archive_path = "../../archive";
  auto abst_file = archive_path + "/gb_high.abst";
  auto json_file = archive_path + "/gb_high.json";

  // get GB ILA from the legacy Python script (ItSy)
  auto gb = ilang::ImportSynthAbstraction(abst_file, "GB_HIGH");

  // export ILA portable
  ilang::ExportIlaPortable(gb, json_file);

  return 0;
}

