// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: utils.cc

// commonly used helper functions/macros

#include <lmac/share.h>
#include <lmac/utils.h>

namespace ilang {

ExprRef NewInput(Ila& m, const std::string& name, const int& wid) {
#ifdef ICFG_BOOL_AS_BV
  return m.NewBvInput(name, wid);

#else // ICFG_BOOL_AS_BV
  if (wid == 1) {
    return m.NewBoolInput(name);
  } else {
    return m.NewBvInput(name, wid);
  }

#endif // ICFG_BOOL_AS_BV
}

ExprRef NewState(Ila& m, const std::string& name, const int& wid) {
#ifdef ICFG_BOOL_AS_BV
  return m.NewBvState(name, wid);

#else // ICFG_BOOL_AS_BV
  if (wid == 1) {
    return m.NewBoolState(name);
  } else {
    return m.NewBvState(name, wid);
  }

#endif // ICFG_BOOL_AS_BV
}

}; // namespace ilang

