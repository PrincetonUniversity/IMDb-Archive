/// \file the ila example of RBM
///  Hongce Zhang (hongcez@princeton.edu)
///

#ifndef RBM_ILA_H__
#define RBM_ILA_H__

#include <cmath>

#include <ilang/ilang++.h>
#include <rbm_fixpoint.h>

using namespace ilang;

/// The fixpoint formats, defined in RBM ila.h
extern const FixpointFormat FP01_D;
extern const FixpointFormat FP05_D;
extern const FixpointFormat FPsum3;
extern const FixpointFormat FPpow;
extern const FixpointFormat FPsum;
extern const FixpointFormat FPedge;
extern const FixpointFormat FPconst;
extern const FixpointFormat FPu16;
/// ila constants
extern ExprRef b0; // = BvConst(0 , 1);
extern ExprRef b1; // = BvConst(1 , 1);
extern ExprRef h0_8  ; // = BvConst(0 , 8);
extern ExprRef h1_8  ; // = BvConst(1 , 8);
extern ExprRef h0_4  ; // = BvConst(0 , 4);
extern ExprRef h1_4  ; // = BvConst(1 , 4);
extern ExprRef h2_4  ; // = BvConst(2 , 4);
extern ExprRef h3_4  ; // = BvConst(3 , 4);
extern ExprRef h4_4  ; // = BvConst(4 , 4);
extern ExprRef h0_16 ; // = BvConst(0 , 16);
extern ExprRef h1_16 ; // = BvConst(1 , 16);
extern ExprRef h0_32 ; // = BvConst(0 , 32);
extern ExprRef h0_64 ; // = BvConst(0 , 64);
// Design parameters
constexpr unsigned K = 5;
constexpr unsigned NUM_MOVIE_MAX = 100;
constexpr unsigned NUM_HIDDEN_MAX = 100;
constexpr unsigned NUM_VISIBLE_MAX = NUM_MOVIE_MAX * K;
constexpr unsigned DATAMEM_ADDR_WIDTH = unsigned(log2(NUM_VISIBLE_MAX+1)) + 1 ;  // 9 // it is definitely not dividable, but need to check
constexpr unsigned HIDDEN_UNIT_WIDTH  = unsigned(log2(NUM_HIDDEN_MAX+1)) + 1 ;  // 7 // it is definitely not dividable, but need to check
constexpr unsigned VISIBLE_UNIT_WIDTH = unsigned(log2(NUM_VISIBLE_MAX+1)) + 1 ;  // 9
constexpr unsigned EDGEMEM_ADDR_WIDTH = unsigned(log2( (NUM_VISIBLE_MAX+1)*(NUM_HIDDEN_MAX+1) )) + 1 ;// 16
constexpr unsigned POS_ADDR_WIDTH = EDGEMEM_ADDR_WIDTH;
constexpr unsigned NEG_ADDR_WIDTH = EDGEMEM_ADDR_WIDTH;
constexpr unsigned PREDICT_RESULT_WIDTH = unsigned(log2(NUM_MOVIE_MAX))+1;  // 7
constexpr unsigned KWIDTH = unsigned( log2(K) ) + 1; // 3


/// \brief the class of RBM ila
class RBM {

public:
  // --------------- CONSTRUCTOR ------ //
  /// add state, add instructions, add child
  RBM();
  // --------------- MEMBERS ----------- //
  /// the ila mode
  Ila model;


private:
  /// Called by the constructor to create the child-ILA
  /// for 
  Ila AddChildComputeUabs(InstrRef& inst);
  // Ila AddChildLoadUabs   (InstrRef& inst);
  // Ila AddChildStoreUabs  (InstrRef& inst);
  // Ila AddChildTrain      (Ila& m);
  // Ila AddChildPredict    (Ila& m);
protected:
  // ------------ STATE ------------ //
  // I/O interface: this is where the commands come from.
  ExprRef conf_done         ;
  ExprRef conf_num_hidden   ;
  ExprRef conf_num_visible  ;
  ExprRef conf_num_users    ;
  ExprRef conf_num_loops    ;
  ExprRef conf_num_testusers;
  ExprRef conf_num_movies   ;
  /// other I/Os
  ExprRef reset       ; // I
  /// DMA read port
  /*
  ExprRef rd_grant  ; // I 1
  ExprRef rd_request; // O 1
  ExprRef rd_index  ; // O 32
  ExprRef rd_length ; // O 32
  ExprRef data_in   ; // I 32
  /// DMA write port
  ExprRef wr_grant   ; // I 1
  ExprRef wr_request ; // O 1
  ExprRef wr_index   ; // O 32
  ExprRef wr_length  ; // O 32
  ExprRef data_out   ; // O 32
  ExprRef rd_trans;
  ExprRef wr_trans;
  ExprRef rd_complete;
  ExprRef wr_complete;
  */

  // Top-level memory state
  ExprRef data          ;
  ExprRef predict_result;
  ExprRef mem           ;

  // internal arch state.
  ExprRef init_done    ;
  ExprRef done         ;
  ExprRef num_hidden   ;
  ExprRef num_visible  ;
  ExprRef num_users    ;
  ExprRef num_loops    ;
  ExprRef num_testusers;
  ExprRef num_movies   ;
  // internal arch state for transmissions


}; // class RBM

#endif // RBM_ILA_H__
