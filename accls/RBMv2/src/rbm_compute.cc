/// \file the ila example of RBM 2nd-level
/// ila rbm level 2 -- Compute
/// Hongce Zhang (hongcez@princeton.edu)
///


#include <rbm_ila.h>

Ila RBM::AddChildComputeUabs(InstrRef& inst) {
  auto uabs = model.NewChild("compute");

  uabs.SetValid( (init_done == 1) & (done == 0) );
  // states
  auto index      = uabs.NewBvState("index"      , 16);
  auto loop_count = uabs.NewBvState("loop_count" , 16);
  auto upc        = uabs.NewBvState("upc"        , 4);
  auto i          = uabs.NewBvState("i"          , 16);

  auto data               = uabs.state("data");
  auto data               = uabs.state("mem");

  auto edges_mem          = uabs.NewMemState("edges", EDGEMEM_ADDR_WIDTH ,8 ) ;
  auto nlp                = uabs.state("num_loops"                          ) ;
  auto nm                 = ZExt(uabs.state("num_movies"             ) , 32 ) ;
  auto nu                 = uabs.state("num_users"                          ) ;
  auto ntu                = uabs.state("num_testusers"                      ) ;
  auto out_rd_request     = uabs.state("rd_request"                         ) ;
  // auto out_rd_complete    = uabs.state("rd_complete"                        ) ;
  // auto out_rd_length      = uabs.state("rd_length"                          ) ;
  // auto out_rd_index       = uabs.state("rd_index"                           ) ;
  auto train_input_done   = uabs.state("train_input_done",   1              ) ;
  auto predict_input_done = uabs.state("predict_input_done", 1              ) ;

  // init statement
  uabs.AddInit(upc                == 0);
  uabs.AddInit(index              == 0);
  uabs.AddInit(loop_count         == 0);
  uabs.AddInit(train_input_done   == 0);
  uabs.AddInit(predict_input_done == 0);
  uabs.AddInit(i == 0);
  // uabs.AddInit(out_rd_complete    == 0);

  // pc states
  auto StartReadState        = BvConst(0,4);
  auto StartTrainOrPredict   = BvConst(1,4);
  auto WaitForTrainStartDeactivateState   = BvConst(2,4);
  auto WaitForPredictStartDeactivateState  = BvConst(3,4);


  auto trainUabs   = AddChildTrain(uabs); // the reason to have it here is to have the
  auto predictUabs = AddChildPredict(uabs);

  { // StartRead
      auto instr = model.NewInstr("StartRead");

      instr.SetDecode( (upc == StartReadState) & (i < nv) );

      instr.SetUpdate( data , Store(data, i, Load(mem, ZExt(index, 32) + i)));
      instr.SetUpdate( i, i+1);
      instr.SetUpdate( upc , StartReadState );

  } // StartRead
  { // FinishRead
      auto instr = model.NewInstr("FinishRead");

      instr.SetDecode( (upc == StartReadState) & (i == nv) );

      instr.SetUpdate ( data , nv, BvConst(1,32));
      instr.SetUpdate ( train_input_done   , Ite ( loop_count <  nlp, b1 , b0 ) ) ;
      instr.SetUpdate ( predict_input_done , Ite ( loop_count == nlp, b1 , b0 ) ) ;

      instr.SetUpdate( upc , StartTrainOrPredict );
  } // FinishRead
  { // WaitForStartTrain
      auto instr = model.NewInstr("WaitForStartTrain");

      instr.SetDecode( (upc == StartTrainOrPredict) & (
         ( (train_input_done == 1) & (train_start == 1) ) 
          ) );
      // predict_start <- 1
      // predict_input_done <- 0

      instr.SetUpdate ( train_input_done   , b0 ) ;
      instr.SetUpdate ( upc , WaitForTrainStartDeactivateState );
      instr.SetProgram ( trainUabs );
  } // WaitForStartTrain

  { // WaitForStartPredict
      auto instr = model.NewInstr("WaitForStartPredict");

      instr.SetDecode( (upc == StartTrainOrPredict) & (
         ( (predict_input_done == 1) & (predict_start == 1) )
          ) );
      // predict_start <- 1
      // predict_input_done <- 0

      instr.SetUpdate ( predict_input_done , b0 ) ;

      instr.SetUpdate ( upc ,  WaitForPredictStartDeactivateState );
      instr.SetProgram ( predictUabs );
  } // WaitForStartPredict

  { // WaitForStartDoNothing
      auto instr = model.NewInstr("WaitForStartDoNothing");

      instr.SetDecode( (upc == StartTrainOrPredict) & ! (
         ( (train_input_done == 1) & (train_start == 1) ) |
         ( (predict_input_done == 1) & (predict_start == 1) )
          ) );
      // predict_start <- 1
      // predict_input_done <- 0

      instr.SetUpdate ( upc , StartTrainOrPredict );
  } // WaitForStart

  { // WaitForTrainStartLoop
      auto instr = model.NewInstr("WaitForTrainStartLoop");

      instr.SetDecode( (upc == WaitForTrainStartDeactivateState) & (train_start == 0) );

      instr.SetUpdate( upc, WaitForTrainStartDeactivateState );

  } // WaitForTrainStartLoop

  { // WaitForPredictStartLoop
      auto instr = model.NewInstr("WaitForPredictStartLoop");

      instr.SetDecode( (upc == WaitForPredictStartDeactivateState) & (predict_start == 0) );

      instr.SetUpdate( upc, WaitForPredictStartDeactivateState );

  } // WaitForPredictStartLoop

  { // WaitForTrainStartDeactivate
      auto instr = model.NewInstr("WaitForTrainStartDeactivate");

      instr.SetDecode( (upc == WaitForTrainStartDeactivateState) & (train_start == 1) );

      instr.SetUpdate( index,
          Ite( (index == nu - 1) & (loop_count != nlp) , h0_16,
          Ite( (index == ntu - 1) & (loop_count == nlp) , index + 1,
            index + 1) ) );

      instr.SetUpdate( loop_count,
        Ite( (index == nu - 1 ) & ( loop_count != nlp) , loop_count + 1, loop_count ));

      instr.SetUpdate( upc, Ite( (index == ntu - 1) & (loop_count == nlp) , FinishState, StartReadState) );

  } // WaitForTrainStartDeactivate

  { // WaitForPredictStartDeactivate
      auto instr = model.NewInstr("WaitForPredictStartDeactivate");

      instr.SetDecode( (upc == WaitForPredictStartDeactivateState) & (predict_start == 1) );

      instr.SetUpdate( index,
          Ite( (index == nu - 1) & (loop_count != nlp) , h0_16,
          Ite( (index == ntu - 1) & (loop_count == nlp) , index + 1,
            index + 1) ) );

      instr.SetUpdate( loop_count,
        Ite( (index == nu - 1 ) & ( loop_count != nlp) , loop_count + 1, loop_count ));

      instr.SetUpdate( upc, Ite( (index == ntu - 1) & (loop_count == nlp) , FinishState, StartReadState) );

  } // WaitForPredictStartDeactivate

  { // Finish
      auto instr = model.NewInstr("Finish");

      instr.SetDecode( (upc == FinishState) );
      // do nothing
      instr.SetUpdate(upc, FinishState);
  } // Finish

  return uabs;
}