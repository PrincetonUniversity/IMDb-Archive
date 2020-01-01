
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
instr.SetUpdate( l15_transducer_threadid             , Ite( ,default_l15_transducer_threadid , default_l15_transducer_threadid               ));
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




---------------------------------------------------------------------------------------

L15_REQTYPE_ICACHE_INVALIDATION  instr.SetUpdate(fetch_state, L15_FETCH_STATE_ICACHE_INVAL_2);
  L15_FETCH_STATE_NORMAL

L15_REQTYPE_INVALIDATION || L15_REQTYPE_DOWNGRADE   instr.SetUpdate(fetch_state, L15_FETCH_STATE_INVAL_2);
  L15_FETCH_STATE_INVAL_3
  L15_FETCH_STATE_INVAL_4
  L15_FETCH_STATE_NORMAL

L15_REQTYPE_ACKDT_LD || L15_REQTYPE_ACKDT_ST_IM
    instr.SetUpdate(fetch_state, L15_FETCH_STATE_NOC2_WRITEBACK_DONE);
L15_FETCH_STATE_NOC2_WRITEBACK_DONE
    L15_FETCH_STATE_NORMAL
    
    
---------------------------------------------------------------------------------------

L15_REQTYPE_ICACHE_INVALIDATION  1474
  decoder_cpx_operation_s1 = `L15_CPX_GEN_ICACHE_INVALIDATION;
  if predecode_is_last_inval_s1
    decoder_noc3_operation_s1 = `L15_NOC3_GEN_ICACHE_INVAL_ACK;

L15_REQTYPE_INVALIDATION  1508
            decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_UPDATE_ST_MSHR_IM_IF_INDEX_TAGCHECK_WAY_MATCHES;
            decoder_mesi_write_op_s1 = `L15_S3_MESI_INVALIDATE_TAGCHECK_WAY_IF_MES;

            decoder_cpx_operation_s1 = `L15_CPX_GEN_INVALIDATION_IF_TAGCHECK_MES_AND_WAYMAP_VALID;

    if (predecode_is_last_inval_s1)
        decoder_noc3_operation_s1 = `L15_NOC3_GEN_INVAL_ACK_FROM_DCACHE;
    else
        decoder_noc3_operation_s1 = `L15_NOC3_GEN_INVAL_ACK_IF_TAGCHECK_M_FROM_DCACHE;

L15_REQTYPE_DOWNGRADE 1549
            decoder_mesi_write_op_s1 = `L15_S3_MESI_WRITE_TAGCHECK_WAY_S_IF_ME;

            if (predecode_is_last_inval_s1)
                decoder_noc3_operation_s1 = `L15_NOC3_GEN_DOWNGRADE_ACK_FROM_DCACHE;
            else
                decoder_noc3_operation_s1 = `L15_NOC3_GEN_DOWNGRADE_ACK_IF_TAGCHECK_M_FROM_DCACHE;


L15_REQTYPE_ACKDT_IFILL
            decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_DEALLOCATION;
            decoder_cpx_operation_s1 = `L15_CPX_GEN_IFILL_RESPONSE_FROM_NOC2;


L15_REQTYPE_ACKDT_LD_NC
            decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_DEALLOCATION;
            decoder_cpx_operation_s1 = `L15_CPX_GEN_LD_RESPONSE_FROM_NOC2;

L15_REQTYPE_ACKDT_LD 1580
  if (fetch_state_s1 == `L15_FETCH_STATE_NORMAL)
                decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_UPDATE_ST_MSHR_IM_IF_INDEX_LRU_WAY_MATCHES;
                decoder_cpx_operation_s1 = `L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID;
                decoder_noc1_operation_s1 = `L15_NOC1_GEN_WRITEBACK_GUARD_IF_LRU_M;
                decoder_noc3_operation_s1 = `L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE;
  else
                decoder_dtag_operation_s1 = `L15_DTAG_OP_WRITE;
                decoder_dcache_operation_s1 = `L15_DCACHE_WRITE_LRU_WAY_FROM_NOC2;
                decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_DEALLOCATION;
                decoder_mesi_write_op_s1 = `L15_S3_MESI_WRITE_LRU_WAY_ACK_STATE;
                decoder_cpx_operation_s1 = `L15_CPX_GEN_LD_RESPONSE_FROM_NOC2;


L15_REQTYPE_ACK_ATOMIC
            decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_DEALLOCATION;
            decoder_cpx_operation_s1 = `L15_CPX_GEN_ATOMIC_ACK_FROM_NOC2;

L15_REQTYPE_ACKDT_ST_IM
            if (fetch_state_s1 == `L15_FETCH_STATE_NORMAL)
                decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_UPDATE_ST_MSHR_IM_IF_INDEX_LRU_WAY_MATCHES;
                decoder_cpx_operation_s1 = `L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID;
                decoder_noc1_operation_s1 = `L15_NOC1_GEN_WRITEBACK_GUARD_IF_LRU_M;
                decoder_noc3_operation_s1 = `L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE;
            else
                decoder_dtag_operation_s1 = `L15_DTAG_OP_WRITE;
                decoder_s2_mshr_operation_s1 = `L15_S2_MSHR_OP_READ_WRITE_CACHE;
                decoder_dcache_operation_s1 = `L15_DCACHE_WRITE_LRU_WAY_FROM_NOC2_AND_MSHR;
                decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_DEALLOCATION;
                decoder_mesi_write_op_s1 = `L15_S3_MESI_WRITE_LRU_WAY_ACK_STATE;
                decoder_cpx_operation_s1 = `L15_CPX_GEN_ST_ACK;


L15_REQTYPE_ACKDT_ST_SM
            decoder_s2_mshr_operation_s1 = `L15_S2_MSHR_OP_READ_WRITE_CACHE;
            decoder_dcache_operation_s1 = `L15_DCACHE_WRITE_MSHR_WAY_FROM_MSHR;
            decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_DEALLOCATION;
            decoder_mesi_write_op_s1 = `L15_S3_MESI_WRITE_MSHR_WAY_ACK_STATE;
            decoder_cpx_operation_s1 = `L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL;

L15_REQTYPE_L2_INTERRUPT
            decoder_cpx_operation_s1 = `L15_CPX_GEN_INTERRUPT;
  

L15_REQTYPE_ACK_WRITETHROUGH 1692
            decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_DEALLOCATION;
            decoder_cpx_operation_s1 = `L15_CPX_GEN_ST_ACK;

L15_REQTYPE_ACK_PREFETCH 1700 do nothing