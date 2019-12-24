/// \file the ila example of OpenPiton L1.5 PCX ILA
///  Hongce Zhang (hongcez@princeton.edu)
///


#ifndef PMESH_L15_ILA_H__
#define PMESH_L15_ILA_H__

#include <ilang/ilang++.h>
#include <vector>

using namespace ilang;

#define MESI_INVALID 0
#define MESI_SHARED 1
#define MESI_EXCLUSIVE 2
#define MESI_MODIFIED 3


/// \brief the class of PMESH L1.5 ila
class PMESH_L15_ILA {

public:
  // --------------- MEMBERS ----------- //
  /// the ila mode
  Ila model;
  
  // --------------- CONSTRUCTOR ----------- //
  PMESH_L15_ILA(const std::string & model_name);
  
protected:
  // --------------- HELPERS -------- //
  /// specify a nondeterministic value within range [low,high]
  ExprRef unknown_range(unsigned low, unsigned high);
  /// a nondeterministic choice of a or b
  static ExprRef unknown_choice(const ExprRef& a, const ExprRef& b);
  /// a nondeterminstic bitvector const of width
  static FuncRef unknown(unsigned width);
  /// a helper function to concat a vector of express
  static ExprRef lConcat(const std::vector<ExprRef> & l);
  /// use a relation
  ExprRef Map(const std::string & name, unsigned retLen, const ExprRef & val);
  /// build a map relation
  ExprRef NewMap(const std::string & name, unsigned inLen, unsigned outLen);
    

}; // class PMESH_L15_ILA

#endif // PMESH_L15_ILA_H__

