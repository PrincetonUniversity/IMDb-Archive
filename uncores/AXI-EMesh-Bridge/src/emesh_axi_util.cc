/// \file Utility function for constructing the EMESH-AXI-Bridge
///  Hongce Zhang (hongcez@princeton.edu)

#include "emesh_axi_master.h"
#include "emesh_axi_slave.h"
#include <cmath>
#include <string>


ExprRef EmeshAxiMasterBridge::unknown_choice(const ExprRef& a, const ExprRef& b) {
  return Ite(unknown(1)() == 1, a, b);
}

ExprRef EmeshAxiMasterBridge::lConcat(const std::vector<ExprRef> & l) {
  assert(l.size() >= 1);
  auto ret = l[0];

  for(auto beg = l.begin()+1; beg != l.end(); ++ beg )
    ret = Concat(ret, *beg);
  return ret;
}

ExprRef EmeshAxiSlaveBridge::unknown_choice(const ExprRef& a, const ExprRef& b) {
  return Ite(unknown(1)() == 1, a, b);
}

ExprRef EmeshAxiSlaveBridge::lConcat(const std::vector<ExprRef> & l) {
  assert(l.size() >= 1);
  auto ret = l[0];

  for(auto beg = l.begin()+1; beg != l.end(); ++ beg )
    ret = Concat(ret, *beg);
  return ret;
}



unsigned nondet_counter = 0;

FuncRef EmeshAxiMasterBridge::unknown(unsigned width) {
  return FuncRef("unknown" + std::to_string(nondet_counter++),
                 SortRef::BV(width));
}

ExprRef EmeshAxiMasterBridge::unknownVal(unsigned width) {
  return unknown(width)();
}

unsigned nondet_counter_slave = 0;
FuncRef EmeshAxiSlaveBridge::unknown(unsigned width) {
  return FuncRef("unknown" + std::to_string(nondet_counter_slave++),
                 SortRef::BV(width));
}

ExprRef EmeshAxiSlaveBridge::unknownVal(unsigned width) {
  return unknown(width)();
}