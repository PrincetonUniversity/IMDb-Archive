// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: utils.h

// commonly used helper functions/macros

#ifndef LMAC_UTILS_H__
#define LMAC_UTILS_H__

#include <ilang/ilang++.h>

namespace ilang {

// calculate the bid-width of a range
#define GET_BWID(beg, end) (end - beg + 1)

// Create new input variable in the ILA
ExprRef NewInput(Ila& m, const std::string& name, const int& wid = 1);

// Create new state variable in the ILA
ExprRef NewState(Ila& m, const std::string& name, const int& wid = 1);

}; // namespace ilang

#endif // LMAC_UTILS_H__

