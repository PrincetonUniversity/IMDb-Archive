/// \file the ila example of RBM 2nd-level
/// ila rbm level 2 -- store
/// Hongce Zhang (hongcez@princeton.edu)
///
#include <rbm_ila.h>

Ila RBM::AddChildStoreUabs(InstrRef& inst) {
  auto uabs = model.NewChild("DMAStore");
  
  auto i = uabs.NewBvState("i", 16);
  

  // store

  return uabs;
} // AddChildStoreUabs