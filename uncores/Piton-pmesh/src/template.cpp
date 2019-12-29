
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
#define CPX_RETURNTYPE_WIDTH 4

instr.SetUpdate( noc1_val          , Ite( , , default_noc1_val          ));
instr.SetUpdate( noc1_address      , Ite( , , default_noc1_address      ));
instr.SetUpdate( noc1_noncacheable , Ite( , , default_noc1_noncacheable ));
instr.SetUpdate( noc1_size         , Ite( , default_noc1_size , default_noc1_size         ));
instr.SetUpdate( noc1_threadid     , Ite( , default_noc1_threadid, default_noc1_threadid     ));
instr.SetUpdate( noc1_mshrid       , Ite( , , default_noc1_mshrid       ));
instr.SetUpdate( noc1_type         , Ite( , , default_noc1_type         ));
instr.SetUpdate( noc1_data_0       , Ite( , , default_noc1_data_0       ));
instr.SetUpdate( noc1_data_1       , Ite( , , default_noc1_data_1       ));

// has the issue of L15_CPX_GEN_INTERRUPT
instr.SetUpdate( l15_transducer_val                  , Ite( , , default_l15_transducer_val                  ));
instr.SetUpdate( l15_transducer_returntype           , Ite( , , default_l15_transducer_returntype           ));
instr.SetUpdate( l15_transducer_noncacheable         , Ite( ,default_l15_transducer_noncacheable , default_l15_transducer_noncacheable         ));
instr.SetUpdate( l15_transducer_atomic               , Ite( , , default_l15_transducer_atomic               ));
instr.SetUpdate( l15_transducer_data_0               , Ite( , , default_l15_transducer_data_0               ));
instr.SetUpdate( l15_transducer_data_1               , Ite( , , default_l15_transducer_data_1               ));
instr.SetUpdate( l15_transducer_data_2               , Ite( , , default_l15_transducer_data_2               ));
instr.SetUpdate( l15_transducer_data_3               , Ite( , , default_l15_transducer_data_3               ));
instr.SetUpdate( l15_transducer_inval_address_15_4   , Ite( ,default_l15_transducer_inval_address_15_4 , default_l15_transducer_inval_address_15_4   ));
instr.SetUpdate( l15_transducer_inval_icache_all_way , Ite( , , default_l15_transducer_inval_icache_all_way ));
instr.SetUpdate( l15_transducer_inval_dcache_inval   , Ite( , , default_l15_transducer_inval_dcache_inval   ));
//instr.SetUpdate( l15_transducer_inval_way            , Ite( , , unknown(BOOL_WIDTH)() ));

instr.SetUpdate( noc3_val          , Ite( , , default_noc3_val           ));
instr.SetUpdate( noc3_type         , Ite( , , default_noc3_type          ));
instr.SetUpdate( noc3_data_0       , Ite( , default_noc3_data_0, default_noc3_data_0        ));
instr.SetUpdate( noc3_data_1       , Ite( , default_noc3_data_1, default_noc3_data_1        ));
instr.SetUpdate( noc3_mshrid       , Ite( , default_noc3_mshrid, default_noc3_mshrid        ));
instr.SetUpdate( noc3_threadid     , Ite( , default_noc3_threadid, default_noc3_threadid      ));
instr.SetUpdate( noc3_address      , Ite( , default_noc3_address, default_noc3_address       ));
instr.SetUpdate( noc3_invalidate   , Ite( , default_noc3_invalidate, default_noc3_invalidate    ));
instr.SetUpdate( noc3_with_data    , Ite( , , default_noc3_with_data     ));
instr.SetUpdate( noc3_fwdack_vector, Ite( , default_noc3_fwdack_vector, default_noc3_fwdack_vector ));

MapUpdate(instr , "address_to_mesi_map"      , address , , ); 
MapUpdate(instr , "address_to_data_map"      , address , , ); 
MapUpdate(instr , "address_to_mshr_map"      , address , , ); 
MapUpdate(instr , "address_to_mshr_data_map" , address , , ); 


  fetch_state_s1