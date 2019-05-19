// rbm.cc
// Synopsis: implementation of the RBM library

#include <ilang/portable/interface.h>
#include <ilang/synth-interface/synth_engine_interface.h>
#include <ilang/util/log.h>
#include <rbm/rbm.h>

namespace ilang {

Ila GetRbmIla() {
  auto m = Ila("RBM");

  return m;
}

InstrLvlAbsPtr GetRbmIlaFromSynthEngine() {
  SetToStdErr(1);
  // EnableDebug("SynthImport");
  // EnableDebug("Portable");

  std::string archive_dir = "../../archive";

  // parent
  auto parent_ila_file = archive_dir + "/rbm.ila";
  auto parent_ila = ImportSynthAbsFromFile(parent_ila_file, "rbm");
  ILA_NOT_NULL(parent_ila);

  // parent --> compute
  auto child_compute_file = archive_dir + "/compute.ila";
  auto child_compute =
      ImportSynthAbsFromFileHier(child_compute_file, parent_ila, "compute");
  ILA_NOT_NULL(child_compute);

  // parent --> compute --> predict
  auto child_predict_file = archive_dir + "/predict.ila";
  auto child_predict =
      ImportSynthAbsFromFileHier(child_predict_file, child_compute, "predict");
  ILA_NOT_NULL(child_predict);

  // parent --> compute --> train
  auto child_train_file = archive_dir + "/train.ila";
  auto child_train =
      ImportSynthAbsFromFileHier(child_train_file, child_compute, "train");
  ILA_NOT_NULL(child_train);

  // parent --> DMAload
  auto child_dmaload_file = archive_dir + "/DMAload.ila";
  auto child_dmaload =
      ImportSynthAbsFromFileHier(child_dmaload_file, parent_ila, "DMAload");
  ILA_NOT_NULL(child_dmaload);

  // parent --> store
  auto child_store_file = archive_dir + "/store.ila";
  auto child_store =
      ImportSynthAbsFromFileHier(child_store_file, parent_ila, "store");
  ILA_NOT_NULL(child_store);

  // export
  IlaSerDesMngr::SerToFile(parent_ila, "rbm.json");

  auto test = IlaSerDesMngr::DesFromFile("rbm.json");

  return parent_ila;
}

}; // namespace ilang

