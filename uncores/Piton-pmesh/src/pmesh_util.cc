/// \file Utility function for constructing the AES model
///  Hongce Zhang (hongcez@princeton.edu)

#include "pmesh_l15_ila.h"
#include <cmath>
#include <string>

PMESH_L15_ILA::PMESH_L15_ILA(const std::string & model_name)
  : model(model_name) {
}

ExprRef PMESH_L15_ILA::unknown_range(unsigned low, unsigned high) {
  unsigned width = (unsigned)std::ceil(std::log2(high + 1));
  auto val = unknown(width)();
  model.AddInit((val >= low) & (val <= high));
  return val;
}

ExprRef PMESH_L15_ILA::unknown_choice(const ExprRef& a, const ExprRef& b) {
  return Ite(unknown(1)() == 1, a, b);
}

ExprRef PMESH_L15_ILA::lConcat(const std::vector<ExprRef> & l) {
  assert(l.size() >= 1);
  auto ret = l[0];

  for(auto beg = l.begin()+1; beg != l.end(); ++ beg )
    ret = Concat(ret, *beg);
  return ret;
}


/// build a map relation
ExprRef PMESH_L15_ILA::Map(const std::string & name, unsigned retLen, const ExprRef & val) {
  //return FuncRef(name, SortRef::BV(retLen), SortRef::BV(val.bit_width()) )( val );
  auto dummy_map_val = model.state(name+"_map_val");
  assert(dummy_map_val.bit_width() == retLen);
  return dummy_map_val;
}


/// build a map relation
ExprRef PMESH_L15_ILA::NewMap(const std::string & name, unsigned inLen, unsigned outLen) {
  // return ExprRef(NULL); //model.NewMemState(name+"_mapmem", inLen, outLen);
  // model the results from looking up the map
  return model.NewBvState(name+"_map_val", outLen); // so we don't care the `inLen`
}

void PMESH_L15_ILA::MapUpdate(InstrRef & instr, const std::string & name, const ExprRef & idx, 
  const ExprRef & cond, const ExprRef & val) {

  auto dummy_map_val = model.state(name+"_map_val");
  assert (dummy_map_val.bit_width() == val.bit_width());

  instr.SetUpdate( dummy_map_val, Ite(cond, val, unknown( dummy_map_val.bit_width() )() ) );
}


unsigned nondet_counter = 0;

FuncRef PMESH_L15_ILA::unknown(unsigned width) {
  return FuncRef("__unknown__" + std::to_string(nondet_counter++),
                 SortRef::BV(width));
}