// rbm.h
// Synopsis: the header of the rbm library

#ifndef RBM_ILA_H__
#define RBM_ILA_H__

#include <ilang/ila/instr_lvl_abs.h>
#include <ilang/ilang++.h>

namespace ilang {

Ila GetRbmIla();

InstrLvlAbsPtr GetRbmIlaFromSynthEngine();

}; // namespace ilang

#endif // RBM_ILA_H__

