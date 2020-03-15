/// \file the ila example of RBM 3nd-level
/// ila rbm level 3 -- train
/// Hongce Zhang (hongcez@princeton.edu)
///

#include <rbm_ila.h>

Ila RBM::AddChildTrain(Ila& m) {
  auto uabs = m.NewChild("train");
  // TODO: add the exact computation here
  return uabs;
}