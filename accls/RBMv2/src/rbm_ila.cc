/// \file the ila example of RBM top-level
///  Hongce Zhang (hongcez@princeton.edu)
///

#include <rbm_ila.h>

RBM::RBM()
    : // construct the model
      model("RBM"),
      // I/O interface: this is where the commands come from.
      conf_done         (model.NewBvInput("conf_done"          , 1))  ,
      conf_num_hidden   (model.NewBvInput("conf_num_hidden"    , 32)) ,
      conf_num_visible  (model.NewBvInput("conf_num_visible"   , 32)) ,
      conf_num_users    (model.NewBvInput("conf_num_users"     , 32)) ,
      conf_num_loops    (model.NewBvInput("conf_num_loops"     , 32)) ,
      conf_num_testusers(model.NewBvInput("conf_num_testusers" , 32)) ,
      conf_num_movies   (model.NewBvInput("conf_num_movies"    , 32)) ,
      // other I/Os
      reset             (model.NewBvInput("rst"              , 1))  ,
      /// DMA read port
      mem               (model.NewMemState("mem", 32, 8)),
      /*
      rd_grant   (model.NewBvInput("rd_grant"   , 1))  ,
      rd_request (model.NewBvState("rd_request" , 1))  ,
      rd_index   (model.NewBvState("rd_index"   , 32)) ,
      rd_length  (model.NewBvState("rd_length"  , 32)) ,
      rd_complete(model.NewBvState("rd_complete", 1))  ,
      data_in    (model.NewBvInput("data_in"    , 32)) ,
      /// DMA write port
      wr_grant   (model.NewBvInput("wr_grant"   , 1))  ,
      wr_request (model.NewBvState("wr_request" , 1))  ,
      wr_index   (model.NewBvState("wr_index"   , 32)) ,
      wr_length  (model.NewBvState("wr_length"  , 32)) ,
      wr_complete(model.NewBvState("wr_complete", 1))  ,
      data_out   (model.NewBvState("data_out"   , 32)) , */
      // Top-level memory state
      data           ( model.NewMemState ( "data"           , DATAMEM_ADDR_WIDTH   , 8)) ,
      predict_result ( model.NewMemState ( "predict_result" , PREDICT_RESULT_WIDTH , 8)) ,
      // internal arch state.
      init_done    (model.NewBvState("init_done"     , 1))  ,
      done         (model.NewBvState("done"          , 1))  ,
      num_hidden   (model.NewBvState("num_hidden"    , 16)) ,
      num_visible  (model.NewBvState("num_visible"   , 16)) ,
      num_users    (model.NewBvState("num_users"     , 16)) ,
      num_loops    (model.NewBvState("num_loops"     , 16)) ,
      num_testusers(model.NewBvState("num_testusers" , 16)) ,
      num_movies   (model.NewBvState("num_movies"    , 16))
      // internal arch state for transmissions
      // rd_trans     (model.NewBvState("rd_trans", 1)),
      // wr_trans     (model.NewBvState("wr_trans", 1))
  {

    // Question: How should these be treated
    model.AddInit(init_done     == b0);
    model.AddInit(done          == b0);
    model.AddInit(num_hidden    == h0_16);
    model.AddInit(num_visible   == h0_16);
    model.AddInit(num_users     == h0_16);
    model.AddInit(num_loops     == h0_16);
    model.AddInit(num_testusers == h0_16);
    model.AddInit(num_movies    == h0_16);
    // model.AddInit(rd_request    == b0);
    // model.AddInit(wr_request    == b0);
    // model.AddInit(rd_complete   == b0);
    // model.AddInit(wr_complete   == b0);

    model.SetValid((reset == 0) | (conf_done == 1));


    { // RESET
      auto instr = model.NewInstr("Reset");

      instr.SetDecode( reset == 0 ); // neg reset
      instr.SetUpdate(init_done    , b0 );
      instr.SetUpdate(done         , b0 );
      instr.SetUpdate(num_hidden   , h0_16);
      instr.SetUpdate(num_visible  , h0_16);
      instr.SetUpdate(num_users    , h0_16);
      instr.SetUpdate(num_loops    , h0_16);
      instr.SetUpdate(num_testusers, h0_16);
      instr.SetUpdate(num_movies   , h0_16);
    } // RESET

    { // Configure
      auto instr = model.NewInstr("Configure");

      instr.SetDecode( (reset == 1) & (conf_done == 1) );
      // if init_done, will not update
      instr.SetUpdate ( init_done    , Ite ( init_done == 0, b1, init_done ) );
      instr.SetUpdate ( num_hidden   , Ite ( init_done == 0, conf_num_hidden    ( 15,0 ) , num_hidden    ) ) ;
      instr.SetUpdate ( num_visible  , Ite ( init_done == 0, conf_num_visible   ( 15,0 ) , num_visible   ) ) ;
      instr.SetUpdate ( num_users    , Ite ( init_done == 0, conf_num_users     ( 15,0 ) , num_users     ) ) ;
      instr.SetUpdate ( num_loops    , Ite ( init_done == 0, conf_num_loops     ( 15,0 ) , num_loops     ) ) ;
      instr.SetUpdate ( num_testusers, Ite ( init_done == 0, conf_num_testusers ( 15,0 ) , num_testusers ) ) ;
      instr.SetUpdate ( num_movies   , Ite ( init_done == 0, conf_num_movies    ( 15,0 ) , num_movies    ) ) ;
      // will start the sub-ila from this instruction
      auto child = AddChildComputeUabs(instr);
      instr.SetProgram(child);
    } // Configure


    // in the future we should
    // 1. wrap this API
    // 2. find a way to auto deduct such
    model.AddSeqTran( InstrRef(NULL)           , model.instr("Reset")     , BoolConst(true) );
    model.AddSeqTran( model.instr("Reset")     , model.instr("Configure") , BoolConst(true) );
    model.AddSeqTran( model.instr("Reset")     , model.instr("Reset")     , BoolConst(true) );
    model.AddSeqTran( InstrRef(NULL)           , model.instr("Configure") , BoolConst(true) );
    model.AddSeqTran( model.instr("Configure") , model.instr("Reset")     , BoolConst(true) );
    // no configure -> configure edge


/*
    { // ReadGranted
      auto instr = model.NewInstr("ReadGranted");

      instr.SetDecode( (reset == 0) & (rd_grant == 1) );
      instr.SetUpdate( rd_trans, Ite(rd_request == 1, b1, rd_trans) );
      // the first load
      instr.SetUpdate( data, Ite(rd_request == 1, Store(data, BvConst(0, DATAMEM_ADDR_WIDTH), data_in(7,0) ), data ) );
      // this will trigger the loadUabs
      auto child = AddChildLoadUabs(instr);
      instr.SetProgram(child);
    } // ReadGranted

    { // WriteGranted
      auto instr = model.NewInstr("WriteGranted");

      instr.SetDecode( (reset == 0) & (wr_grant == 1) );
      instr.SetUpdate( wr_trans, Ite(wr_request == 1, b1, wr_trans) );
      // this will trigger the storeUabs
      auto child = AddChildStoreUabs(instr);
      instr.SetProgram(child);
    } // WriteGranted
*/
} // RBM - the constructor



