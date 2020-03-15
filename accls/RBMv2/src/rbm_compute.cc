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
  auto mem                = uabs.state("mem");
  auto edges_mem          = uabs.NewMemState("edges", EDGEMEM_ADDR_WIDTH ,8 ) ;

  auto nlp                = uabs.state("num_loops"                          ) ;
  auto nm                 = ZExt(uabs.state("num_movies"             ) , 32 ) ;
  auto nu                 = uabs.state("num_users"                          ) ;
  auto ntu                = uabs.state("num_testusers"                      ) ;
  auto out_rd_request     = uabs.state("rd_request"                         ) ;
  // auto out_rd_complete    = uabs.state("rd_complete"                        ) ;
  // auto out_rd_length      = uabs.state("rd_length"                          ) ;
  // auto out_rd_index       = uabs.state("rd_index"                           ) ;
  auto train_input_done   = uabs.NewBvState("train_input_done",  1) ;
  auto predict_input_done = uabs.NewBvState("predict_input_done",1) ;
  auto nv = uabs.state("num_visible");
  auto train_start = uabs.NewBvState("train_start"        , 1);
  auto predict_start = uabs.NewBvState("predict_start"        , 1);

  // function for training/prediction
  auto data_mem_sort      = SortRef::MEM(DATAMEM_ADDR_WIDTH   , 8);
  auto edges_mem_sort     = SortRef::MEM(EDGEMEM_ADDR_WIDTH   , 8);
  auto predict_mem_sort   = SortRef::MEM(PREDICT_RESULT_WIDTH , 8);
  auto nh_bv_sort         = SortRef::BV(16);
  auto nv_bv_sort         = SortRef::BV(16);
  
  // data, edges, nh, nv -> edges
  auto train_func         = FuncRef("train_func", edges_mem_sort, {data_mem_sort, edges_mem_sort, nh_bv_sort, nv_bv_sort} ); 
   // data, edges, nh, nv -> predict_result
  auto predict_func       = FuncRef("predict_func", predict_mem_sort, {data_mem_sort, edges_mem_sort, nh_bv_sort, nv_bv_sort} );


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
  auto FinishState  = BvConst(4,4);



  auto trainUabs   = AddChildTrain(uabs); // the reason to have it here is to have the
  auto predictUabs = AddChildPredict(uabs);

  { // StartRead
      auto instr = model.NewInstr("StartRead");

      instr.SetDecode( (upc == StartReadState) & (i < nv) );

      instr.SetUpdate( data , Store(data, i(8,0), Load(mem, ZExt(index + i, 32) )));
      instr.SetUpdate( i, i+1);
      instr.SetUpdate( upc , StartReadState );

  } // StartRead
  { // FinishRead
      auto instr = model.NewInstr("FinishRead");

      instr.SetDecode( (upc == StartReadState) & (i == nv) );

      instr.SetUpdate ( data , Store(data,nv(8,0), BvConst(1,8)));
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