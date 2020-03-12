/// \file the ila example of RBM top-level (utilities)
///  Hongce Zhang (hongcez@princeton.edu)
///

#include <rbm_ila.h>

/// The fixpoint formats, defined in RBM ila.h
const FixpointFormat FP01_D(64,1);
const FixpointFormat FP05_D(32,3);
const FixpointFormat FPsum3(64,64);
const FixpointFormat FPpow(32,32);
const FixpointFormat FPsum(16,15,true);
const FixpointFormat FPedge(8,7,true);
const FixpointFormat FPconst(16,4,true);
const FixpointFormat FPu16(16,16);

ExprRef b0    = BvConst(0 , 1);
ExprRef b1    = BvConst(1 , 1);
ExprRef h0_8  = BvConst(0 , 8);
ExprRef h1_8  = BvConst(1 , 8);
ExprRef h0_4  = BvConst(0 , 4);
ExprRef h1_4  = BvConst(1 , 4);
ExprRef h2_4  = BvConst(2 , 4);
ExprRef h3_4  = BvConst(3 , 4);
ExprRef h4_4  = BvConst(4 , 4);
ExprRef h0_16 = BvConst(0 , 16);
ExprRef h1_16 = BvConst(1 , 16);
ExprRef h0_32 = BvConst(0 , 32);
ExprRef h0_64 = BvConst(0 , 64);
