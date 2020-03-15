/// \file the ila example of RBM 3nd-level
/// ila rbm level 3 -- predict
/// Hongce Zhang (hongcez@princeton.edu)
///

#include <rbm_ila.h>

Ila RBM::AddChildPredict(Ila& m) {
  auto uabs = m.NewChild("predict");
  // TODO: add the exact computation here
  return uabs;
}