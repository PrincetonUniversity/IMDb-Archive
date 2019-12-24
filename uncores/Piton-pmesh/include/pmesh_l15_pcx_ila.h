/// \file the ila example of OpenPiton L1.5 PCX ILA
///  Hongce Zhang (hongcez@princeton.edu)
///

#ifndef PMESH_L15_PCX_ILA_H__
#define PMESH_L15_PCX_ILA_H__

#include <pmesh_l15_ila.h>

/// \brief the class of PMESH L1.5 ila
class PMESH_L15_PCX_ILA : public PMESH_L15_ILA {

public:
  // --------------- CONSTRUCTOR ------ //
  /// add state, add instructions, add child
  PMESH_L15_PCX_ILA();

private:
  /// Called by the constructor to create the child-ILA
  /// for block encryption
  void AddChild(InstrRef& inst);

protected:
  // ------------ STATE ------------ //
  // I/O interface: this is where the commands come from.
  ExprRef address;
  ExprRef data   ;
  ExprRef nc     ;
  ExprRef rqtype ;
  ExprRef size   ;
  ExprRef val    ;

  // arch state.
  ExprRef l15_noc1buffer_req_address;
  ExprRef l15_noc1buffer_req_noncacheable;
  ExprRef l15_noc1buffer_req_size        ;
  ExprRef l15_noc1buffer_req_type        ;

  // output state l15->core
  ExprRef l15_transducer_val       ;
  ExprRef l15_transducer_returntype;
  ExprRef l15_transducer_data_0    ;

  // internal state : map (mem)
  ExprRef mesi_state;
  ExprRef data_state;


}; // class PMESH_L15_PCX_ILA


#endif // PMESH_L15_PCX_ILA_H__


