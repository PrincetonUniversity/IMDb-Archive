/// \file the ila example of RBM 2nd-level
/// ila rbm level 2 -- Compute
/// Hongce Zhang (hongcez@princeton.edu)
///


#include <rbm_ila.h>

Ila RBM::AddChildComputeUabs(InstrRef& inst) {
  auto init_done = model.state("init_done");
  auto done = model.state("done");

  auto uabs = model.NewChild("compute");

  uabs.SetValid( (init_done == 1) & (done == 0) );
  // states
  auto index      = uabs.NewBvState("index"      , 16);
  auto loop_count = uabs.NewBvState("loop_count" , 16);
  auto upc        = uabs.NewBvState("upc"        ,  4);
  auto i          = uabs.NewBvState("i"          , 16);
  auto wi         = uabs.NewBvState("wi"         , 16);

  auto data               = uabs.state("data");
  auto mem                = uabs.state("mem");
  auto edges_mem          = uabs.NewMemState("edges", EDGEMEM_ADDR_WIDTH ,8 ) ;
  auto predict_result     = uabs.state("predict_result");

  auto nlp                = uabs.state("num_loops"                          ) ;
  auto nm                 = uabs.state("num_movies"             ) ; // 16
  auto nu                 = uabs.state("num_users"                          ) ;
  auto ntu                = uabs.state("num_testusers"                      ) ;
  auto out_rd_request     = uabs.state("rd_request"                         ) ;
  // auto out_rd_complete    = uabs.state("rd_complete"                        ) ;
  // auto out_rd_length      = uabs.state("rd_length"                          ) ;
  // auto out_rd_index       = uabs.state("rd_index"                           ) ;
  auto nv = uabs.state("num_visible");
  auto nh = uabs.state("num_hidden");

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
  uabs.AddInit(i == 0);
  // uabs.AddInit(out_rd_complete    == 0);

  // pc states
  auto StartReadState        = BvConst(0,4);
  auto StartWriteState       = BvConst(1,4);
  auto FinishState           = BvConst(2,4);

  { // StartRead
      auto instr = uabs.NewInstr("StartRead");

      instr.SetDecode( (upc == StartReadState) & (i < nv) );

      instr.SetUpdate( data , Store(data, i(8,0), Load(mem, ZExt(nv, 32) * ZExt(index, 32)  + ZExt(i, 32) )));
      instr.SetUpdate( i, i+1);
      instr.SetUpdate( upc , StartReadState );

  } // StartRead

  { // FinishReadTrain
      auto instr = uabs.NewInstr("FinishReadTrain");

      instr.SetDecode( (upc == StartReadState) & (i == nv) & (loop_count < nlp) );

      instr.SetUpdate( data , Store(data,nv(8,0), BvConst(1,8)));

      instr.SetUpdate(edges_mem, train_func({data, edges_mem, nh, nv}));

      instr.SetUpdate( index,
          Ite( (index == nu - 1) , h0_16,
            index + 1) );

      instr.SetUpdate( loop_count,
        Ite( (index == nu - 1 ) , loop_count + 1, loop_count ));

      instr.SetUpdate( i, BvConst(0, 16) );

      instr.SetUpdate( upc , StartReadState );
  } // FinishReadTrain


  { // FinishReadPredict
      auto instr = uabs.NewInstr("FinishReadPredict");

      instr.SetDecode( (upc == StartReadState) & (i == nv) & (loop_count == nlp) );

      instr.SetUpdate( data , Store(data,nv(8,0), BvConst(1,8)));

      instr.SetUpdate(predict_result, predict_func({data, edges_mem, nh, nv}));

      instr.SetUpdate( index, index + 1);
      instr.SetUpdate( wi, BvConst(0, 16) );

      instr.SetUpdate( upc , StartWriteState );
  } // FinishReadPredict


  { // write
      auto instr = uabs.NewInstr("WriteBack");

      instr.SetDecode( (upc == StartWriteState) & (wi < nm) );

      instr.SetUpdate( mem, Store(mem, ZExt(nm, 32) * ZExt(index, 32) + ZExt(wi, 32), Load(predict_result, wi(6,0) ) ) );

      instr.SetUpdate( wi, wi+1 );

      instr.SetUpdate( upc , StartWriteState );
  } // FinishReadPredict

  { // write
      auto instr = uabs.NewInstr("WriteBackDone");

      instr.SetDecode( (upc == StartWriteState) & (wi == nm) );

      instr.SetUpdate( i, BvConst(0,16) );

      instr.SetUpdate( upc , Ite( index != ntu  , StartReadState, FinishState ));
  } // FinishReadPredict

  { // Finish
      auto instr = uabs.NewInstr("Finish");

      instr.SetDecode( (upc == FinishState) );
      // do nothing
      // instr.SetUpdate(upc, FinishState);
      instr.SetUpdate(done, b1);
  } // Finish


  return uabs;
}