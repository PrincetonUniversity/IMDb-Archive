/// \file the ila example of AES block encryption
///  Hongce Zhang (hongcez@princeton.edu)
///

#ifndef PMESH_L15_NOC2_ILA_H__
#define PMESH_L15_NOC2_ILA_H__

#include <pmesh_l15_ila.h>

/// \brief the class of PMESH L1.5 NOC2 ila
class PMESH_L15_NOC2_ILA : public PMESH_L15_ILA {


public:
  // --------------- CONSTRUCTOR ------ //
  /// add state, add instructions, add child
  PMESH_L15_NOC2_ILA();


protected:
  // ------------ INPUTS ------------ //
  // I/O interface: this is where the commands come from.
  ExprRef icache_type    ;
  ExprRef mshrid         ;
  ExprRef threadid       ;
  ExprRef val            ;
  ExprRef reqtype        ;
  ExprRef mesi_ack_state ;
  ExprRef fwdack_vector  ;
  ExprRef address        ;
  ExprRef data_0         ;
  ExprRef data_1         ;
  ExprRef data_2         ;
  ExprRef data_3         ;

  // ------------ STATES ------------ //
  // output state l15->core
  ExprRef l15_transducer_val                 ;
  ExprRef l15_transducer_returntype          ;
  ExprRef l15_transducer_noncacheable        ;
  ExprRef l15_transducer_atomic              ;
  ExprRef l15_transducer_threadid            ;
  ExprRef l15_transducer_data_0              ;
  ExprRef l15_transducer_data_1              ;
  ExprRef l15_transducer_data_2              ;
  ExprRef l15_transducer_data_3              ;
  ExprRef l15_transducer_inval_address_15_4  ;
  ExprRef l15_transducer_inval_icache_all_way;
  ExprRef l15_transducer_inval_dcache_inval  ;
  // output state l15->noc1
  ExprRef noc1_val          ;
  ExprRef noc1_address      ;
  ExprRef noc1_noncacheable ;
  ExprRef noc1_size         ;
  ExprRef noc1_threadid     ;
  ExprRef noc1_mshrid       ;
  ExprRef noc1_type         ;
  ExprRef noc1_data_0       ;
  ExprRef noc1_data_1       ;
  // output state l15->noc3
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

  // internal state : map (mem)
  ExprRef mesi_state      ;
  ExprRef data_state      ;
  ExprRef mshr_val        ;
  ExprRef mshr_ld_address ;
  ExprRef mshr_st_address ;
  ExprRef mshr_st_state   ;
  ExprRef mshr_st_way     ;
  ExprRef mshr_data       ;
  ExprRef mshr_ctrl       ;
  ExprRef fetch_state     ;

}; // class PMESH_L15_NOC2_ILA


#endif // PMESH_L15_NOC2_ILA_H__


