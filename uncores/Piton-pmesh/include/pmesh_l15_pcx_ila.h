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

  ExprRef address   ;
  ExprRef data      ;
  ExprRef data_next ;
  ExprRef nc        ;
  ExprRef rqtype    ;
  ExprRef size      ;
  ExprRef threadid  ;
  ExprRef val       ;
  ExprRef amo_op    ;
  ExprRef invalidate;

  // arch state.
  ExprRef noc1_val          ;
  ExprRef noc1_address      ;
  ExprRef noc1_noncacheable ;
  ExprRef noc1_size         ;
  ExprRef noc1_threadid     ;
  ExprRef noc1_mshrid       ;
  ExprRef noc1_type         ;
  ExprRef noc1_data_0       ;
  ExprRef noc1_data_1       ;

  ExprRef noc3_val          ;
  ExprRef noc3_type         ;
  ExprRef noc3_data_0       ;
  ExprRef noc3_data_1       ;
  ExprRef noc3_mshrid       ;
  ExprRef noc3_threadid     ;
  ExprRef noc3_address      ;
  ExprRef noc3_invalidate   ;
  ExprRef noc3_with_data    ;
  ExprRef noc3_fwdack_vector;

  ExprRef l15_transducer_val                  ;
  ExprRef l15_transducer_returntype           ;
  ExprRef l15_transducer_noncacheable         ;
  ExprRef l15_transducer_atomic               ;
  ExprRef l15_transducer_data_0               ;
  ExprRef l15_transducer_data_1               ;
  ExprRef l15_transducer_data_2               ;
  ExprRef l15_transducer_data_3               ;
  ExprRef l15_transducer_inval_address_15_4   ;
  ExprRef l15_transducer_inval_icache_all_way ;
  ExprRef l15_transducer_inval_dcache_inval   ;

  // internal state : map (mem)
  ExprRef mesi_state    ;
  ExprRef data_state    ;
  ExprRef missed_on_this;
  ExprRef mshr_data     ;
  ExprRef fetch_state   ;

}; // class PMESH_L15_PCX_ILA


#endif // PMESH_L15_PCX_ILA_H__


