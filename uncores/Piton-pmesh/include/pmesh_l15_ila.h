/// \file the ila example of OpenPiton L1.5 PCX ILA
///  Hongce Zhang (hongcez@princeton.edu)
///


#ifndef PMESH_L15_ILA_H__
#define PMESH_L15_ILA_H__

#include <ilang/ilang++.h>
#include <vector>

using namespace ilang;

#define NOC2_MSG_WIDTH 8
#define WAY_WIDTH  2
#define MESI_WIDTH 2
#define ADDR_WIDTH  40
#define BOOL_WIDTH  1
#define THREADID_WIDTH  1
#define DATA_WIDTH  64
#define MSHR_ID_WIDTH  2
#define AMO_OP_WIDTH 4
#define FETCH_STATE_WIDTH 3

#define PCX_REQTYPE_WIDTH 5
#define NOC1_REQ_SIZE_WIDTH  3
#define NOC1_REQ_TYPE_WIDTH  5
#define NOC2_REQTYPE_WIDTH 8
#define NOC3_REQ_TYPE_WIDTH  3
#define FWD_SUBCACHELINE_VECTOR 4
#define CPX_RETURNTYPE_WIDTH 4

#define b0 BvConst(0,1)
#define b1 BvConst(1,1)
#define zero_data BvConst(0,DATA_WIDTH)

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
  /// Set update function to a map  
  void MapUpdate(InstrRef & instr, const std::string & name, const ExprRef & idx, const ExprRef & val) ;
  /// Set update function to a map
  void MapUpdate(InstrRef & instr, const std::string & name, const ExprRef & idx, 
    const ExprRef & cond, const ExprRef & val);
}; // class PMESH_L15_ILA

#endif // PMESH_L15_ILA_H__

