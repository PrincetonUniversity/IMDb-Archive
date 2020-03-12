/// \file the ila example of RBM 2nd-level
/// ila rbm level 2 -- load
/// Hongce Zhang (hongcez@princeton.edu)
///


#include <rbm_ila.h>

Ila RBM::AddChildLoadUabs(InstrRef& inst) {
  auto uabs = model.NewChild("DMAload");
  
  auto i = uabs.NewBvState("i", 16);
  

  // load

  return uabs;
} // AddChildLoadUabs