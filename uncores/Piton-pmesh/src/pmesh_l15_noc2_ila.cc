/// \file the ila example of OpenPiton L1.5 NOC2 ILA
///  Hongce Zhang (hongcez@princeton.edu)
///

#include "pmesh_l15_noc2_ila.h"

// some assumptions on the interface:
// amo_op : 0
// blockinitstore : 0
// blockstore : 0
// csm_data : 0
// data_next_entry : 0
// invalidate_cacheline : 0
// l1rplway : 0
// prefetch : 0
// threadid : 0

// a request if not hit
// go to the NoC
// if hit, ?

/*

`define L15_MSHR_ID_WIDTH 2
`define L15_MSHR_ID_IFILL 2'd1
`define L15_MSHR_ID_LD 2'd2
`define L15_MSHR_ID_ST 2'd3

*/

// l2miss , f4b -> output
// hmc_fill ?? should be 0
// noc2decoder_l15_src_homeid, // forward to cpx not sure what is the use
// noc2decoder_l15_csm_mshrid, // to output l15_csm_req_ticket_s2 not sure the use
      
#define L15_CONTROL_PREFETCH_1B 0
#define L15_CONTROL_BLOCKSTOREINIT_1B (L15_CONTROL_PREFETCH_1B + 1)  // 1
#define L15_CONTROL_LOAD (L15_CONTROL_BLOCKSTOREINIT_1B + 1) // 2
#define L15_CONTROL_ATOMIC (L15_CONTROL_LOAD + 1) // 3
#define L15_CONTROL_ICACHE (L15_CONTROL_ATOMIC + 1) // 4
#define L15_CONTROL_SIZE_3B (L15_CONTROL_ICACHE + 3) // 7
#define L15_CONTROL_THREADID (L15_CONTROL_SIZE_3B + L15_THREADID_WIDTH) // 7+1 = 8
#define L15_CONTROL_L1_REPLACEMENT_WAY_2B (L15_CONTROL_THREADID + L15_WAY_WIDTH) // 8 + 2
#define L15_CONTROL_NC_1B (L15_CONTROL_L1_REPLACEMENT_WAY_2B + 1) // 11
#define L15_CONTROL_BLOCKSTORE_1B (L15_CONTROL_NC_1B + 1) // 12
#define L15_CONTROL_WIDTH (L15_CONTROL_BLOCKSTORE_1B + 1) // 13


PMESH_L15_NOC2_ILA::PMESH_L15_NOC2_ILA()
    : // construct the model
      PMESH_L15_ILA("PMESH_L15_NOC2_ILA"),
      // I/O interface: this is where the commands come from.

      icache_type    (model.NewBvInput("noc2decoder_l15_icache_type", BOOL_WIDTH)),  // 2 trans/4 trans in fetch_state?
      mshrid         (model.NewBvInput("noc2decoder_l15_mshrid"     , MSHR_ID_WIDTH)),
      threadid       (model.NewBvInput("noc2decoder_l15_threadid"   , THREADID_WIDTH)),
      val            (model.NewBvInput("noc2decoder_l15_val"        , BOOL_WIDTH)),
      reqtype        (model.NewBvInput("noc2decoder_l15_reqtype"    , NOC2_REQTYPE_WIDTH)),
      mesi_ack_state (model.NewBvInput("noc2decoder_l15_ack_state"  , MESI_WIDTH)), // could be used to write to mesi (if fill ?)
      fwdack_vector  (model.NewBvInput("noc2decoder_l15_fwd_subcacheline_vector", FWD_SUBCACHELINE_VECTOR)), // use for testing last child, and fwd to cpx
      address        (model.NewBvInput("noc2decoder_l15_address"    , ADDR_WIDTH)), 
      data_0         (model.NewBvInput("noc2decoder_l15_data_0"     , DATA_WIDTH)),
      data_1         (model.NewBvInput("noc2decoder_l15_data_1"     , DATA_WIDTH)),
      data_2         (model.NewBvInput("noc2decoder_l15_data_2"     , DATA_WIDTH)),
      data_3         (model.NewBvInput("noc2decoder_l15_data_3"     , DATA_WIDTH)),
  
      // Output states: l1.5 --> noc1 requests
      // not specifying these updates:
      // l15_noc1buffer_req_csm_data   
      // l15_noc1buffer_req_csm_ticket 
      // l15_noc1buffer_req_homeid
      // l15_noc1buffer_req_prefetch   
      noc1_val          ( model.NewBvState("l15_noc1buffer_req_val"          , BOOL_WIDTH) )          ,
      noc1_address      ( model.NewBvState("l15_noc1buffer_req_address"      , ADDR_WIDTH) )          ,
      noc1_noncacheable ( model.NewBvState("l15_noc1buffer_req_noncacheable" , BOOL_WIDTH) )          ,
      noc1_size         ( model.NewBvState("l15_noc1buffer_req_size"         , NOC1_REQ_SIZE_WIDTH) ) ,
      noc1_threadid     ( model.NewBvState("l15_noc1buffer_req_threadid"     , BOOL_WIDTH) )          , // not
      noc1_mshrid       ( model.NewBvState("l15_noc1buffer_req_mshrid"       , MSHR_ID_WIDTH) )       ,
      noc1_type         ( model.NewBvState("l15_noc1buffer_req_type"         , NOC1_REQ_TYPE_WIDTH) ) ,
      noc1_data_0       ( model.NewBvState("l15_noc1buffer_req_data_0"       , DATA_WIDTH))           ,
      noc1_data_1       ( model.NewBvState("l15_noc1buffer_req_data_1"       , DATA_WIDTH))           ,
            
      // l1.5 --> noc3 requests
      noc3_val          ( model.NewBvState("l15_noc3encoder_req_val"           , BOOL_WIDTH))              ,
      noc3_type         ( model.NewBvState("l15_noc3encoder_req_type"          , NOC3_REQ_TYPE_WIDTH))     ,
      noc3_data_0       ( model.NewBvState("l15_noc3encoder_req_data_0"        , DATA_WIDTH))              ,
      noc3_data_1       ( model.NewBvState("l15_noc3encoder_req_data_1"        , DATA_WIDTH))              ,
      noc3_mshrid       ( model.NewBvState("l15_noc3encoder_req_mshrid"        , MSHR_ID_WIDTH))           ,
      noc3_threadid     ( model.NewBvState("l15_noc3encoder_req_threadid"      , THREADID_WIDTH))          ,
      noc3_address      ( model.NewBvState("l15_noc3encoder_req_address"       , ADDR_WIDTH))              ,
      noc3_invalidate   ( model.NewBvState("l15_noc3encoder_req_was_inval"     , BOOL_WIDTH))              ,
      noc3_with_data    ( model.NewBvState("l15_noc3encoder_req_with_data"     , BOOL_WIDTH))              , // depends on M?
      noc3_fwdack_vector( model.NewBvState("l15_noc3encoder_req_fwdack_vector" , FWD_SUBCACHELINE_VECTOR)) , // seems to be a direct forwarding

      // Output states: l1.5 --> core
      l15_transducer_val                  ( model.NewBvState("l15_transducer_val"                  , BOOL_WIDTH) )            ,
      l15_transducer_returntype           ( model.NewBvState("l15_transducer_returntype"           , CPX_RETURNTYPE_WIDTH)  ) , // 0 if hit                                            , l15_cpxencoder_returntype
      l15_transducer_noncacheable         ( model.NewBvState("l15_transducer_noncacheable"         , BOOL_WIDTH)  )           ,
      l15_transducer_atomic               ( model.NewBvState("l15_transducer_atomic"               , BOOL_WIDTH)  )           ,
      l15_transducer_threadid             ( model.NewBvState("l15_transducer_threadid"             , THREADID_WIDTH) )        ,
      l15_transducer_data_0               ( model.NewBvState("l15_transducer_data_0"               , DATA_WIDTH) )            ,
      l15_transducer_data_1               ( model.NewBvState("l15_transducer_data_1"               , DATA_WIDTH) )            ,
      l15_transducer_data_2               ( model.NewBvState("l15_transducer_data_2"               , DATA_WIDTH) )            ,
      l15_transducer_data_3               ( model.NewBvState("l15_transducer_data_3"               , DATA_WIDTH) )            ,
      l15_transducer_inval_address_15_4   ( model.NewBvState("l15_transducer_inval_address_15_4"   , 12))                     ,
      l15_transducer_inval_icache_all_way ( model.NewBvState("l15_transducer_inval_icache_all_way" , BOOL_WIDTH))             ,
      l15_transducer_inval_dcache_inval   ( model.NewBvState("l15_transducer_inval_dcache_inval"   , BOOL_WIDTH))             , // will need way map table but instead go abstract ...
      // l15_transducer_inval_way            ( model.NewBvState("l15_transducer_inval_way"            , WAY_WIDTH))              ,

      // we don't model the way-map table, so not model here
      // l15_transducer_inval_icache_inval // always 0
      // l15_transducer_inval_dcache_all_way // always 0
      // l15_transducer_cross_invalidate // always 0
      // l15_transducer_cross_invalidate_way // always 0

            // We made the map as a mem (although in the design, it does not need to be so large)

      mesi_state      ( NewMap           ( "address_to_mesi_map"        , THREADID_WIDTH  , MESI_WIDTH ) )             , // write ( l15_mesi_write_data_s2[1:0]) read mesi_state_way0_s2? ?flush_state_s2
      data_state      ( NewMap           ( "address_to_data_map"        , THREADID_WIDTH  , 2*DATA_WIDTH) )            , // read  ( noc3: dcache_l15_dout_s3) write: ( dcache_write_data_s2)

      mshr_val        ( NewMap           ( "address_to_mshr_map"        , THREADID_WIDTH  , 4*2*BOOL_WIDTH) )          , // mshr_pipe_vals_s1
      mshr_ld_address ( NewMap           ( "address_to_mshr_ld_address" , THREADID_WIDTH  , 2*ADDR_WIDTH*BOOL_WIDTH) ) , // mshr_ld_address_array
      mshr_st_address ( NewMap           ( "address_to_mshr_st_address" , THREADID_WIDTH  , 2*ADDR_WIDTH*BOOL_WIDTH) ) , // mshr_st_address_array
      mshr_st_state   ( NewMap           ( "address_to_mshr_st_state"   , THREADID_WIDTH  , 2*2) )                     , // mshr_pipe_st_state_s1
      // be careful of the threadid issue, the update is width 2
      mshr_st_way     ( NewMap           ( "address_to_mshr_st_way"     , THREADID_WIDTH  , 2*2) )                     , // mshr_pipe_st_way_s1
      mshr_data       ( NewMap           ( "address_to_mshr_data_map"   , THREADID_WIDTH  , 2*DATA_WIDTH) )            , // mshr_pipe_write_buffer_s2
      mshr_ctrl       ( NewMap           ( "address_to_mshr_control"    , THREADID_WIDTH  , 13))                       , // mshr_pipe_readres_control_s1

      fetch_state     ( model.NewBvState ( "fetch_state_s1"             , FETCH_STATE_WIDTH) )
      // l1d_way      ( NewMap           ( "address_to_l1d_way_map"     , ADDR_WIDTH           , WAY_WIDTH) )               , // wmt_compare_match_way_s3

      // -----------------------------------------------------------------------------------------------

    {

  auto wmt_compare_func = FuncRef("wmt_compare_match_s3", SortRef::BOOL());
  auto wmt_compare_match_s3 = wmt_compare_func();

  auto lru_state_mes_func = FuncRef("lru_state_mes_s3", SortRef::BOOL());
  auto lru_state_mes_s3 = lru_state_mes_func();
  // ------------------------------ CONSTANTS ---------------------------------- //

  auto L15_REQTYPE_WIDTH                    = 6;
  auto L15_REQTYPE_NONE                     = BvConst(0, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_IGNORE                   = BvConst(1, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_IFILL                    = BvConst(2, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_STORE                    = BvConst(3, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_CAS                      = BvConst(4, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_SWP_LOADSTUB             = BvConst(5, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_INVALIDATION             = BvConst(6, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_DOWNGRADE                = BvConst(7, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ACKDT_LD_NC              = BvConst(8, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ACKDT_IFILL              = BvConst(9, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ACKDT_LD                 = BvConst(10, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ACKDT_ST_IM              = BvConst(11, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ACKDT_ST_SM              = BvConst(12, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ACK_WRITETHROUGH         = BvConst(13, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ACK_ATOMIC               = BvConst(14, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_LOAD_NC                  = BvConst(15, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_LOAD_PREFETCH            = BvConst(16, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_WRITETHROUGH             = BvConst(17, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ICACHE_INVALIDATION      = BvConst(18, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_PCX_INTERRUPT            = BvConst(19, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_L2_INTERRUPT             = BvConst(20, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_LOAD                     = BvConst(21, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_INT_VEC_DIS              = BvConst(22, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_WRITE_CONFIG_REG         = BvConst(23, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_LOAD_CONFIG_REG          = BvConst(24, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_DIAG_LOAD                = BvConst(25, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_DIAG_STORE               = BvConst(26, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_LINE_FLUSH               = BvConst(27, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_HMC_FILL                 = BvConst(28, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_HMC_DIAG_LOAD            = BvConst(29, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_HMC_DIAG_STORE           = BvConst(30, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_HMC_FLUSH                = BvConst(31, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ACK_PREFETCH             = BvConst(32, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_ICACHE_SELF_INVALIDATION = BvConst(33, L15_REQTYPE_WIDTH);
  auto L15_REQTYPE_DCACHE_SELF_INVALIDATION = BvConst(34, L15_REQTYPE_WIDTH);

  auto L15_MSHR_ID_DEFAULT = BvConst(0, MSHR_ID_WIDTH);
  auto L15_MSHR_ID_IFILL   = BvConst(1, MSHR_ID_WIDTH);
  auto L15_MSHR_ID_LD      = BvConst(2, MSHR_ID_WIDTH);
  auto L15_MSHR_ID_ST      = BvConst(3, MSHR_ID_WIDTH);

  auto MESI_INVALID                       = BvConst(0, MESI_WIDTH);
  auto MESI_SHARED                        = BvConst(1, MESI_WIDTH);
  auto MESI_EXCLUSIVE                     = BvConst(2, MESI_WIDTH);
  auto MESI_MODIFIED                      = BvConst(3, MESI_WIDTH);
  auto L15_MESI_TRANSITION_STATE_SM       = BvConst(1, MESI_WIDTH);
  auto L15_MESI_TRANSITION_STATE_IM       = BvConst(2, MESI_WIDTH);
  auto L15_MESI_TRANSITION_STATE_WAIT_ACK = BvConst(3, MESI_WIDTH);

  auto PCX_REQTYPE_LOAD         = BvConst(0  , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_IFILL        = BvConst(16 , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_STORE        = BvConst(1  , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_CAS1         = BvConst(2  , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_CAS2         = BvConst(3  , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_SWP_LOADSTUB = BvConst(6  , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_INTERRUPT    = BvConst(9  , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_FP1          = BvConst(12 , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_FP2          = BvConst(11 , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_STREAM_LOAD  = BvConst(4  , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_STREAM_STORE = BvConst(5  , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_FWD_REQ      = BvConst(13 , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_FWD_REPLY    = BvConst(14 , PCX_REQTYPE_WIDTH);
  auto PCX_REQTYPE_AMO          = BvConst(6  , PCX_REQTYPE_WIDTH);

  auto L15_FETCH_STATE_NORMAL              = BvConst(0, FETCH_STATE_WIDTH);
  auto L15_FETCH_STATE_PCX_WRITEBACK_DONE  = BvConst(1, FETCH_STATE_WIDTH);
  auto L15_FETCH_STATE_INVAL_2             = BvConst(2, FETCH_STATE_WIDTH);
  auto L15_FETCH_STATE_INVAL_3             = BvConst(3, FETCH_STATE_WIDTH);
  auto L15_FETCH_STATE_INVAL_4             = BvConst(4, FETCH_STATE_WIDTH);
  auto L15_FETCH_STATE_NOC2_WRITEBACK_DONE = BvConst(5, FETCH_STATE_WIDTH);
  auto L15_FETCH_STATE_ICACHE_INVAL_2      = BvConst(6, FETCH_STATE_WIDTH);

  auto L15_NOC1_REQTYPE_WRITEBACK_GUARD      = BvConst(1, NOC1_REQ_TYPE_WIDTH); // width 5
  auto L15_NOC1_REQTYPE_LD_REQUEST           = BvConst(2, NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_IFILL_REQUEST        = BvConst(3, NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_WRITETHROUGH_REQUEST = BvConst(4, NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_ST_UPGRADE_REQUEST   = BvConst(5, NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_ST_FILL_REQUEST      = BvConst(6, NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_CAS_REQUEST          = BvConst(7, NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_SWAP_REQUEST         = BvConst(8, NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_INTERRUPT_FWD        = BvConst(9, NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_AMO_ADD_REQUEST      = BvConst(10,NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_AMO_AND_REQUEST      = BvConst(11,NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_AMO_OR_REQUEST       = BvConst(12,NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_AMO_XOR_REQUEST      = BvConst(13,NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_AMO_MAX_REQUEST      = BvConst(14,NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_AMO_MAXU_REQUEST     = BvConst(15,NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_AMO_MIN_REQUEST      = BvConst(16,NOC1_REQ_TYPE_WIDTH);
  auto L15_NOC1_REQTYPE_AMO_MINU_REQUEST     = BvConst(17,NOC1_REQ_TYPE_WIDTH);

  auto L15_NOC3_REQTYPE_WRITEBACK        = BvConst(1, NOC3_REQ_TYPE_WIDTH);
  auto L15_NOC3_REQTYPE_INVAL_ACK        = BvConst(2, NOC3_REQ_TYPE_WIDTH);
  auto L15_NOC3_REQTYPE_DOWNGRADE_ACK    = BvConst(3, NOC3_REQ_TYPE_WIDTH);
  auto L15_NOC3_REQTYPE_ICACHE_INVAL_ACK = BvConst(4, NOC3_REQ_TYPE_WIDTH);

  auto CPX_RESTYPE_LOAD             = BvConst(0,  CPX_RETURNTYPE_WIDTH); // width 4
  auto CPX_RESTYPE_IFILL1           = BvConst(1,  CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_IFILL2           = BvConst(1,  CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_STREAM_LOAD      = BvConst(2,  CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_INVAL            = BvConst(3,  CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_STORE_ACK        = BvConst(4,  CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_STREAM_STORE_ACK = BvConst(6,  CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_INTERRUPT        = BvConst(7,  CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_FP               = BvConst(8,  CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_FWD_REQ          = BvConst(10, CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_FWD_REPLY        = BvConst(11, CPX_RETURNTYPE_WIDTH);
  auto CPX_RESTYPE_ATOMIC_RES       = BvConst(13, CPX_RETURNTYPE_WIDTH);

  auto L15_AMO_OP_NONE  = BvConst(0x0,AMO_OP_WIDTH); // width 4
  auto L15_AMO_OP_LR    = BvConst(0x1,AMO_OP_WIDTH); // width 4
  auto L15_AMO_OP_SC    = BvConst(0x2,AMO_OP_WIDTH);
  auto L15_AMO_OP_SWAP  = BvConst(0x3,AMO_OP_WIDTH);
  auto L15_AMO_OP_ADD   = BvConst(0x4,AMO_OP_WIDTH);
  auto L15_AMO_OP_AND   = BvConst(0x5,AMO_OP_WIDTH);
  auto L15_AMO_OP_OR    = BvConst(0x6,AMO_OP_WIDTH);
  auto L15_AMO_OP_XOR   = BvConst(0x7,AMO_OP_WIDTH);
  auto L15_AMO_OP_MAX   = BvConst(0x8,AMO_OP_WIDTH);
  auto L15_AMO_OP_MAXU  = BvConst(0x9,AMO_OP_WIDTH);
  auto L15_AMO_OP_MIN   = BvConst(0xa,AMO_OP_WIDTH);
  auto L15_AMO_OP_MINU  = BvConst(0xb,AMO_OP_WIDTH);
  auto L15_AMO_OP_CAS1  = BvConst(0xc,AMO_OP_WIDTH);


  // predecoder operations
  auto L15_REQTYPE_SWP_LOADSTUB = BvConst(0x5, 6);
  auto L15_REQTYPE_AMO_LR       = BvConst(35 , 6);
  auto L15_REQTYPE_AMO_SC       = BvConst(36 , 6);
  auto L15_REQTYPE_AMO_ADD      = BvConst(38 , 6);
  auto L15_REQTYPE_AMO_AND      = BvConst(39 , 6);
  auto L15_REQTYPE_AMO_OR       = BvConst(40 , 6);
  auto L15_REQTYPE_AMO_XOR      = BvConst(41 , 6);
  auto L15_REQTYPE_AMO_MAX      = BvConst(42 , 6);
  auto L15_REQTYPE_AMO_MAXU     = BvConst(43 , 6);
  auto L15_REQTYPE_AMO_MIN      = BvConst(44 , 6);
  auto L15_REQTYPE_AMO_MINU     = BvConst(45 , 6);
  auto L15_REQTYPE_CAS          = BvConst(0x4, 6);

  auto MSG_TYPE_LOAD_FWD        = BvConst(16, NOC2_MSG_WIDTH);
  auto MSG_TYPE_STORE_FWD       = BvConst(17, NOC2_MSG_WIDTH);
  auto MSG_TYPE_INV_FWD         = BvConst(18, NOC2_MSG_WIDTH);

  // ------------------------------ Shared Expressions ---------------------------------- //


  auto mshr_pipe_readres_control_s1 = mshr_ctrl;
  auto predecode_mshr_read_control_s1 = mshr_pipe_readres_control_s1;

  std::vector<ExprRef> mshr_val_array = { mshr_val(3,0), mshr_val(7,4) };
  std::vector<ExprRef> mshr_st_state_array = { mshr_st_state(1,0), mshr_st_state(3,2) };
  std::vector<ExprRef> mshr_st_address_array = { mshr_st_address(ADDR_WIDTH-1,0), mshr_st_address(2*ADDR_WIDTH-1, ADDR_WIDTH) } ;
  std::vector<ExprRef> mshr_ld_address_array = { mshr_ld_address(ADDR_WIDTH-1,0), mshr_ld_address(2*ADDR_WIDTH-1, ADDR_WIDTH) } ;
  std::vector<ExprRef> mshr_st_way_array = { mshr_st_way(1,0), mshr_st_way(3,2) };


  auto predecode_size_s1 = predecode_mshr_read_control_s1(L15_CONTROL_SIZE_3B , L15_CONTROL_SIZE_3B - 2);
  // predecode_threadid_s1 = predecode_mshr_read_control_s1[`L15_CONTROL_THREADID -: `L15_THREADID_WIDTH];
  auto predecode_threadid_s1 = threadid;
  auto predecode_l1_replacement_way_s1 = predecode_mshr_read_control_s1(L15_CONTROL_L1_REPLACEMENT_WAY_2B, L15_CONTROL_L1_REPLACEMENT_WAY_2B-1);
  auto predecode_non_cacheable_s1 = predecode_mshr_read_control_s1(L15_CONTROL_NC_1B);
  auto nc = predecode_non_cacheable_s1; // alias
  // 4.16.14: disable blockstores
  auto predecode_blockstore_bit_s1 = predecode_mshr_read_control_s1(L15_CONTROL_BLOCKSTORE_1B);
  auto predecode_blockstore_init_s1 = predecode_mshr_read_control_s1(L15_CONTROL_BLOCKSTOREINIT_1B);
  auto predecode_prefetch_bit_s1 = predecode_mshr_read_control_s1(L15_CONTROL_PREFETCH_1B);
  // predecode_invalidate_index_s1 = predecode_mshr_read_control_s1[`L15_CONTROL_INVALIDATE_INDEX_1B -: 1];
  // predecode_l2_miss_s1 = l2miss;
  // predecode_f4b_s1 = f4b;
  auto predecode_atomic_s1 = predecode_mshr_read_control_s1(L15_CONTROL_ATOMIC);
  auto predecode_dcache_load_s1 = predecode_mshr_read_control_s1(L15_CONTROL_LOAD);
  auto predecode_fwd_subcacheline_vector_s1 = noc2decoder_l15_fwd_subcacheline_vector;

  auto predecode_dcache_noc2_store_im_s1 = Ite(predecode_threadid_s1 == 0, mshr_st_state_array[0], mshr_st_state_array[1] ) == L15_MESI_TRANSITION_STATE_IM;
  auto predecode_dcache_noc2_store_sm_s1 = Ite(predecode_threadid_s1 == 0, mshr_st_state_array[0], mshr_st_state_array[1] ) == L15_MESI_TRANSITION_STATE_SM;


  auto predecode_address_plus0_s1 = lConcat( { address(39,6), BvConst(0, 2) , address(3,0) } );
  auto predecode_address_plus1_s1 = lConcat( { address(39,6), BvConst(1, 2) , address(3,0) } );
  auto predecode_address_plus2_s1 = lConcat( { address(39,6), BvConst(2, 2) , address(3,0) } );
  auto predecode_address_plus3_s1 = lConcat( { address(39,6), BvConst(3, 2) , address(3,0) } );

  auto predecode_is_last_inval_s1 = Ite(icache_type == 1, 
      ( (fetch_state == L15_FETCH_STATE_ICACHE_INVAL_2)  & (predecode_fwd_subcacheline_vector_s1(3,2)  == BvConst(3,2) ) ) |
      ( (fetch_state == L15_FETCH_STATE_NORMAL) & (predecode_fwd_subcacheline_vector_s1(3,0) == BvConst(3,4)))
    ,
      ((fetch_state == L15_FETCH_STATE_INVAL_4) & (predecode_fwd_subcacheline_vector_s1(3) == 1 )) |
      ((fetch_state == L15_FETCH_STATE_INVAL_3) & (predecode_fwd_subcacheline_vector_s1(3,2) == 1)) |
      ((fetch_state == L15_FETCH_STATE_INVAL_2) & (predecode_fwd_subcacheline_vector_s1(3,1) == 1)) |
      ((fetch_state == L15_FETCH_STATE_NORMAL)  & (predecode_fwd_subcacheline_vector_s1(3,0) == 1))
    );

  auto tagcheck_state = Map( "address_to_mesi_map",  MESI_WIDTH, address ); // tagcheck_state_s2 , s3 , ...
  auto tagcheck_state_mes_s2 = ( tagcheck_state == MESI_MODIFIED ) | ( tagcheck_state == MESI_SHARED ) | ( tagcheck_state == MESI_EXCLUSIVE );
  auto tagcheck_state_me_s2 = ( tagcheck_state == MESI_MODIFIED ) | ( tagcheck_state == MESI_EXCLUSIVE );
  auto tagcheck_state_m_s3 = (tagcheck_state == MESI_MODIFIED);

// ------------------------------ DEFAULT VALUES ---------------------------------- //
// noc1 default values : 4191
  auto default_noc1_val          = b0;                              // unknown(BOOL_WIDTH)();
  auto default_noc1_address      = BvConst(0,ADDR_WIDTH );          // unknown(ADDR_WIDTH)() ;
  auto default_noc1_noncacheable = predecode_non_cacheable_s1;                              // unknown(BOOL_WIDTH)()  ;
  auto default_noc1_size         = predecode_size_s1;                            // unknown(NOC1_REQ_SIZE_WIDTH)()  ;
  auto default_noc1_type         = BvConst(0, NOC1_REQ_TYPE_WIDTH); // unknown(NOC1_REQ_TYPE_WIDTH)()  ;
  auto default_noc1_threadid     = threadid;                        // unknown(THREADID_WIDTH)();
  auto default_noc1_mshrid       = mshrid;                          // unknown(MSHR_ID_WIDTH)();
  auto default_noc1_data_0       = zero_data;                       // unknown(DATA_WIDTH)() ;
  auto default_noc1_data_1       = zero_data;                       // unknown(DATA_WIDTH)() ;

  // l15 default value 4139
  auto default_l15_transducer_val                  = b0;                               // unknown(BOOL_WIDTH)();
  auto default_l15_transducer_returntype           = BvConst(0, CPX_RETURNTYPE_WIDTH); // unknown(CPX_RETURNTYPE_WIDTH)();
  auto default_l15_transducer_noncacheable         = predecode_non_cacheable_s1;                               // unknown(BOOL_WIDTH)();
  auto default_l15_transducer_atomic               = b0;                               // unknown(BOOL_WIDTH)();
  auto default_l15_transducer_threadid             = threadid;                         // unknown(THREADID_WIDTH)();
  auto default_l15_transducer_data_0               = zero_data;                        // unknown(DATA_WIDTH)();
  auto default_l15_transducer_data_1               = zero_data;                        // unknown(DATA_WIDTH)();
  auto default_l15_transducer_data_2               = zero_data;                        // unknown(DATA_WIDTH)();
  auto default_l15_transducer_data_3               = zero_data;                        // unknown(DATA_WIDTH)();
  auto default_l15_transducer_inval_address_15_4   = BvConst(0,12);                    // unknown(12)();
  auto default_l15_transducer_inval_icache_all_way = b0;                               // unknown(BOOL_WIDTH)();           // from cpx_icache_inval_s3
  auto default_l15_transducer_inval_dcache_inval   = b0;                               // unknown(BOOL_WIDTH)();           // from cpx_invalidate_l1_s3
  // auto default_l15_transducer_inval_way            = Map( "address_to_l1d_way_map",  WAY_WIDTH, address ); // unknown(WAY_WIDTH);

  // 4413
  auto default_noc3_val          = b0;                                    // unknown(BOOL_WIDTH)();
  auto default_noc3_type         = BvConst(0, NOC3_REQ_TYPE_WIDTH);       // unknown(NOC3_REQ_TYPE_WIDTH)();
  auto default_noc3_data_0       = Map("address_to_data_map", 2*DATA_WIDTH, address) (127, 64);                   // unknown(DATA_WIDTH)();
  auto default_noc3_data_1       = Map("address_to_data_map", 2*DATA_WIDTH, address) ( 63,  0);                   // unknown(DATA_WIDTH)();
  auto default_noc3_mshrid       = mshrid;                     // unknown(MSHR_ID_WIDTH)();
  auto default_noc3_threadid     = threadid;                              // unknown(THREADID_WIDTH)();
  auto default_noc3_address      = BvConst(0, ADDR_WIDTH);                // unknown(ADDR_WIDTH)();
  auto default_noc3_invalidate   = b0;                                    // unknown(BOOL_WIDTH)();
  auto default_noc3_with_data    = b0;                                    // unknown(BOOL_WIDTH)();
  auto default_noc3_fwdack_vector  = BvConst(0, FWD_SUBCACHELINE_VECTOR); // unknown(FWD_SUBCACHELINE_VECTOR)();


  // ------------------------------ INSTRUCTIONS ---------------------------------- //

  // L1.5 fetch function -- what corresponds to instructions on L1.5 NOC2 interface
  model.SetFetch( lConcat({icache_type, mshrid, threadid, val, reqtype, mesi_ack_state, fwdack_vector, address, data_0, data_1, data_2, data_3}) );
  // Valid instruction: what means to have valid command (valid = 1)
  model.SetValid( val == 1 );

  // 
  // HZ's note about modeling cache
  // This is in some sense a shared memory
  // 1. we can model it as a memory, but this does not
  // mean it must have that size (The same address
  // and etc. of facet axiom function can be adjusted
  // to factor in the conflict-eviction and etc.
  // This part should be 
  // 2. It is a shared state
  // because we have multiple interface
  // This is in some sence similar to the ViCL approach
  // But hte difference is that we treat the Cache state
  // as "state", and encode the updates as SMT queries
  // 
  // For the verification, we need to somehow use an uninterpreted
  // function-like mapping on what it got from the mem
  // 

  // l15_pcxdecoder_ack ??

  // add instructions
  {
    auto instr = model.NewInstr("STORE_FWD");

    instr.SetDecode( ( reqtype == MSG_TYPE_STORE_FWD)  );
    // 509
    auto predecode_address = 
      Ite(icache_type == 1, // L15_REQTYPE_ICACHE_INVALIDATION
        Ite(fetch_state == L15_FETCH_STATE_ICACHE_INVAL_2, predecode_address_plus2_s1, predecode_address_plus0_s1)
      , // L15_REQTYPE_INVALIDATION
        Ite(fetch_state == L15_FETCH_STATE_INVAL_2, predecode_address_plus1_s1, 
        Ite(fetch_state == L15_FETCH_STATE_INVAL_3, predecode_address_plus2_s1,
        Ite(fetch_state == L15_FETCH_STATE_INVAL_4, predecode_address_plus3_s1,
                                                    predecode_address_plus0_s1))));


    // L15_REQTYPE_ICACHE_INVALIDATION 1474
    // decoder_cpx_operation_s1 = L15_CPX_GEN_ICACHE_INVALIDATION 3970
    // if predecode_is_last_inval_s1, decoder_noc3_operation_s1 = L15_NOC3_GEN_ICACHE_INVAL_ACK   4484

    // ----------------------------------------------------------------------------------
    // icache_type == 0
    // L15_REQTYPE_INVALIDATION
      // L15_S3_MSHR_OP_UPDATE_ST_MSHR_IM_IF_INDEX_TAGCHECK_WAY_MATCHES 3912
      // let's not worry about this issue
        /*auto stbuf_compare_match_val_s3 = */
        /*  ((mshr_val_array[1](3) == 1) &  (mshr_st_state_array[1] == L15_MESI_TRANSITION_STATE_SM) & */
        /*   (mshr_st_address_array[1](39,4) == predecode_address(39,4))) |*/
        /*  ((mshr_val_array[0](3) == 1) &  (mshr_st_state_array[0] == L15_MESI_TRANSITION_STATE_SM) & */
        /*   (mshr_st_address_array[0](39,4) == predecode_address(39,4)))*/
        /*  ;*/

      MapUpdate(instr, "address_to_mshr_st_state", threadid, (icache_type == 0), Concat(L15_MESI_TRANSITION_STATE_IM,L15_MESI_TRANSITION_STATE_IM) ); // if ME deallocate
      // L15_S3_MESI_INVALIDATE_TAGCHECK_WAY_IF_MES 2824
      MapUpdate(instr, "address_to_mesi_map", address, (icache_type == 0) & tagcheck_state_mes_s2, MESI_INVALID ) ; // s2 ?
      // L15_CPX_GEN_INVALIDATION_IF_TAGCHECK_MES_AND_WAYMAP_VALID 3983


      instr.SetUpdate( l15_transducer_val                  , Ite( icache_type == 1, b1 , Ite(wmt_compare_match_s3 & tagcheck_state_mes_s3, b1, b0)));
      instr.SetUpdate( l15_transducer_returntype           , Ite( icache_type == 1,  CPX_RESTYPE_INVAL, CPX_RESTYPE_INVAL )                       );
      instr.SetUpdate( l15_transducer_noncacheable         , default_l15_transducer_noncacheable                                                  );
      instr.SetUpdate( l15_transducer_atomic               , default_l15_transducer_atomic                                                        );
      instr.SetUpdate( l15_transducer_threadid             , default_l15_transducer_threadid                                                      );
      instr.SetUpdate( l15_transducer_data_0               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_data_1               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_data_2               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_data_3               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_inval_address_15_4   , predecode_address(15,4)                                                              );
      instr.SetUpdate( l15_transducer_inval_icache_all_way , Ite( icache_type == 1, b1, b0 )                                                      );
      instr.SetUpdate( l15_transducer_inval_dcache_inval   , Ite( icache_type == 1, b0, b1 )                                                      );

      //                                                                                 L15_NOC3_GEN_ICACHE_INVAL_ACK                                        L15_NOC3_GEN_INVAL_ACK_FROM_DCACHE , L15_NOC3_GEN_INVAL_ACK_IF_TAGCHECK_M_FROM_DCACHE

      instr.SetUpdate( noc3_val           , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , b1                                , default_noc3_val           ) , Ite(predecode_is_last_inval_s1 , b1                         , tagcheck_state_m_s3   )   ) ) ;
      instr.SetUpdate( noc3_type          , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , L15_NOC3_REQTYPE_ICACHE_INVAL_ACK , default_noc3_type          ) , Ite(predecode_is_last_inval_s1 , L15_NOC3_REQTYPE_INVAL_ACK , L15_NOC3_REQTYPE_INVAL_ACK                )   ) ) ;
      instr.SetUpdate( noc3_data_0        , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_data_0               , default_noc3_data_0        ) , Ite(predecode_is_last_inval_s1 , default_noc3_data_0        , default_noc3_data_0     )   ) ) ;
      instr.SetUpdate( noc3_data_1        , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_data_1               , default_noc3_data_1        ) , Ite(predecode_is_last_inval_s1 , default_noc3_data_1        , default_noc3_data_1     )   ) ) ;
      instr.SetUpdate( noc3_mshrid        , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_mshrid               , default_noc3_mshrid        ) , Ite(predecode_is_last_inval_s1 , default_noc3_mshrid        , default_noc3_mshrid     )   ) ) ;
      instr.SetUpdate( noc3_threadid      , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_threadid             , default_noc3_threadid      ) , Ite(predecode_is_last_inval_s1 , default_noc3_threadid      , default_noc3_threadid )   ) ) ;
      instr.SetUpdate( noc3_address       , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , predecode_address                 , default_noc3_address       ) , Ite(predecode_is_last_inval_s1 , predecode_address          , predecode_address           )   ) ) ;
      instr.SetUpdate( noc3_invalidate    , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_invalidate           , default_noc3_invalidate    ) , Ite(predecode_is_last_inval_s1 , default_noc3_invalidate    , default_noc3_invalidate )   ) ) ;
      instr.SetUpdate( noc3_with_data     , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_with_data            , default_noc3_with_data     ) , Ite(predecode_is_last_inval_s1 , Ite(tagcheck_state_m_s3    , b1 , b0) , b1 ) )   ) ;
      instr.SetUpdate( noc3_fwdack_vector , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_fwdack_vector        , default_noc3_fwdack_vector ) , Ite(predecode_is_last_inval_s1 , default_noc3_fwdack_vector , default_noc3_fwdack_vector  )   ) ) ;

      auto fetch_state_nxt = 
        Ite(fetch_state == L15_FETCH_STATE_NORMAL,
          Ite(icache_type == 1, L15_FETCH_STATE_ICACHE_INVAL_2, L15_FETCH_STATE_INVAL_2),
        Ite(fetch_state == L15_FETCH_STATE_INVAL_2, L15_FETCH_STATE_INVAL_3,
        Ite(fetch_state == L15_FETCH_STATE_INVAL_3, L15_FETCH_STATE_INVAL_4,
        Ite(fetch_state == L15_FETCH_STATE_ICACHE_INVAL_2, L15_FETCH_STATE_NORMAL, 
        L15_FETCH_STATE_NORMAL ) ) ) );

    instr.SetUpdate(fetch_state, fetch_state_nxt );

    // update the address-->MESI map is done when it receive instruction from noc2

    // instr.SetUpdate( "address_to_mesi_map", MapUpdate(mesi_state, address, 
    //  Ite( hit, MESI_state, unknown_choice(MESI_SHARED, MESI_EXCLUSIVE) )  ) );

    // It may also update other addresses (conflict eviction and etc.)
    // But I treat that feature as micro-architectual behavior: related
    // to cache size/associativity/lru policy/...
    // So the current spec is free on that behavior (any hehavior is okay)

    // ----------------------------------------------------------------------------
    // update data : instr.SetUpdate( "address_to_data_map", MapUpdate() ); 
    // this is different: if l1.5 only, will not (miss: will update after
    // hear back from noc2, hit not either)
    // if noc, will update (instruction will have a different complete time)

  }

  {
    auto instr = model.NewInstr("INV_FWD");

    instr.SetDecode( ( reqtype == MSG_TYPE_INV_FWD)  );

    // 533 
    auto predecode_address = 
      Ite(icache_type == 1, // L15_REQTYPE_ICACHE_INVALIDATION
        Ite(fetch_state == L15_FETCH_STATE_ICACHE_INVAL_2, predecode_address_plus2_s1, predecode_address_plus0_s1)
      , // L15_REQTYPE_INVALIDATION
        Ite(fetch_state == L15_FETCH_STATE_INVAL_2, predecode_address_plus1_s1, 
        Ite(fetch_state == L15_FETCH_STATE_INVAL_3, predecode_address_plus2_s1,
        Ite(fetch_state == L15_FETCH_STATE_INVAL_4, predecode_address_plus3_s1,
                                                    predecode_address_plus0_s1))));


    // L15_REQTYPE_ICACHE_INVALIDATION 1474
    // decoder_cpx_operation_s1 = L15_CPX_GEN_ICACHE_INVALIDATION 3970
    // if predecode_is_last_inval_s1, decoder_noc3_operation_s1 = L15_NOC3_GEN_ICACHE_INVAL_ACK   4484

    // ----------------------------------------------------------------------------------
    // icache_type == 0
    // L15_REQTYPE_INVALIDATION
      // L15_S3_MSHR_OP_UPDATE_ST_MSHR_IM_IF_INDEX_TAGCHECK_WAY_MATCHES 3912
      // let's not worry about this issue
        /*auto stbuf_compare_match_val_s3 = */
        /*  ((mshr_val_array[1](3) == 1) &  (mshr_st_state_array[1] == L15_MESI_TRANSITION_STATE_SM) & */
        /*   (mshr_st_address_array[1](39,4) == predecode_address(39,4))) |*/
        /*  ((mshr_val_array[0](3) == 1) &  (mshr_st_state_array[0] == L15_MESI_TRANSITION_STATE_SM) & */
        /*   (mshr_st_address_array[0](39,4) == predecode_address(39,4)))*/
        /*  ;*/

      MapUpdate(instr, "address_to_mshr_st_state", threadid, (icache_type == 0), Concat(L15_MESI_TRANSITION_STATE_IM,L15_MESI_TRANSITION_STATE_IM) ); // if ME deallocate
      // L15_S3_MESI_INVALIDATE_TAGCHECK_WAY_IF_MES 2824
      MapUpdate(instr, "address_to_mesi_map", address, (icache_type == 0) & tagcheck_state_mes_s2, MESI_INVALID ) ; // s2 ?
      // L15_CPX_GEN_INVALIDATION_IF_TAGCHECK_MES_AND_WAYMAP_VALID 3983

      instr.SetUpdate( l15_transducer_val                  , Ite( icache_type == 1, b1 , Ite(wmt_compare_match_s3 & tagcheck_state_mes_s3, b1, b0)));
      instr.SetUpdate( l15_transducer_returntype           , Ite( icache_type == 1,  CPX_RESTYPE_INVAL, CPX_RESTYPE_INVAL )                       );
      instr.SetUpdate( l15_transducer_noncacheable         , default_l15_transducer_noncacheable                                                  );
      instr.SetUpdate( l15_transducer_atomic               , default_l15_transducer_atomic                                                        );
      instr.SetUpdate( l15_transducer_threadid             , default_l15_transducer_threadid                                                      );
      instr.SetUpdate( l15_transducer_data_0               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_data_1               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_data_2               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_data_3               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_inval_address_15_4   , predecode_address(15,4)                                                              );
      instr.SetUpdate( l15_transducer_inval_icache_all_way , Ite( icache_type == 1, b1, b0 )                                                      );
      instr.SetUpdate( l15_transducer_inval_dcache_inval   , Ite( icache_type == 1, b0, b1 )                                                      );

      //                                                                                 L15_NOC3_GEN_ICACHE_INVAL_ACK                                        L15_NOC3_GEN_INVAL_ACK_FROM_DCACHE , L15_NOC3_GEN_INVAL_ACK_IF_TAGCHECK_M_FROM_DCACHE

      instr.SetUpdate( noc3_val           , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , b1                                , default_noc3_val           ) , Ite(predecode_is_last_inval_s1 , b1                         , Ite(tagcheck_state_m_s3, b1, b0)   )   ) ) ;
      instr.SetUpdate( noc3_type          , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , L15_NOC3_REQTYPE_ICACHE_INVAL_ACK , default_noc3_type          ) , Ite(predecode_is_last_inval_s1 , L15_NOC3_REQTYPE_INVAL_ACK , L15_NOC3_REQTYPE_INVAL_ACK                )   ) ) ;
      instr.SetUpdate( noc3_data_0        , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_data_0               , default_noc3_data_0        ) , Ite(predecode_is_last_inval_s1 , default_noc3_data_0        , default_noc3_data_0     )   ) ) ;
      instr.SetUpdate( noc3_data_1        , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_data_1               , default_noc3_data_1        ) , Ite(predecode_is_last_inval_s1 , default_noc3_data_1        , default_noc3_data_1     )   ) ) ;
      instr.SetUpdate( noc3_mshrid        , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_mshrid               , default_noc3_mshrid        ) , Ite(predecode_is_last_inval_s1 , default_noc3_mshrid        , default_noc3_mshrid     )   ) ) ;
      instr.SetUpdate( noc3_threadid      , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_threadid             , default_noc3_threadid      ) , Ite(predecode_is_last_inval_s1 , default_noc3_threadid      , default_noc3_threadid )   ) ) ;
      instr.SetUpdate( noc3_address       , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , predecode_address                 , default_noc3_address       ) , Ite(predecode_is_last_inval_s1 , predecode_address          , predecode_address           )   ) ) ;
      instr.SetUpdate( noc3_invalidate    , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_invalidate           , default_noc3_invalidate    ) , Ite(predecode_is_last_inval_s1 , default_noc3_invalidate    , default_noc3_invalidate )   ) ) ;
      instr.SetUpdate( noc3_with_data     , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_with_data            , default_noc3_with_data     ) , Ite(predecode_is_last_inval_s1 , Ite(tagcheck_state_m_s3    , b1 , b0) , b1 ) )   ) ;
      instr.SetUpdate( noc3_fwdack_vector , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_fwdack_vector        , default_noc3_fwdack_vector ) , Ite(predecode_is_last_inval_s1 , default_noc3_fwdack_vector , default_noc3_fwdack_vector  )   ) ) ;

      auto fetch_state_nxt = 
        Ite(fetch_state == L15_FETCH_STATE_NORMAL,
          Ite(icache_type == 1, L15_FETCH_STATE_ICACHE_INVAL_2, L15_FETCH_STATE_INVAL_2),
        Ite(fetch_state == L15_FETCH_STATE_INVAL_2, L15_FETCH_STATE_INVAL_3,
        Ite(fetch_state == L15_FETCH_STATE_INVAL_3, L15_FETCH_STATE_INVAL_4,
        Ite(fetch_state == L15_FETCH_STATE_ICACHE_INVAL_2, L15_FETCH_STATE_NORMAL, 
        L15_FETCH_STATE_NORMAL ) ) ) );

    instr.SetUpdate(fetch_state, fetch_state_nxt );
  }


  {
    auto instr = model.NewInstr("LOAD_FWD");
    // 550
    instr.SetDecode( ( reqtype == MSG_TYPE_LOAD_FWD)  );

    auto predecode_address = 
      Ite(icache_type == 1, // L15_REQTYPE_ICACHE_INVALIDATION
        Ite(fetch_state == L15_FETCH_STATE_ICACHE_INVAL_2, predecode_address_plus2_s1, predecode_address_plus0_s1)
      , // L15_REQTYPE_INVALIDATION
        Ite(fetch_state == L15_FETCH_STATE_INVAL_2, predecode_address_plus1_s1, 
        Ite(fetch_state == L15_FETCH_STATE_INVAL_3, predecode_address_plus2_s1,
        Ite(fetch_state == L15_FETCH_STATE_INVAL_4, predecode_address_plus3_s1,
                                                    predecode_address_plus0_s1))));


    // L15_REQTYPE_ICACHE_INVALIDATION 1474
    // decoder_cpx_operation_s1 = L15_CPX_GEN_ICACHE_INVALIDATION 3970
    // if predecode_is_last_inval_s1, decoder_noc3_operation_s1 = L15_NOC3_GEN_ICACHE_INVAL_ACK   4484

    // ----------------------------------------------------------------------------------
    // icache_type == 0
    // L15_REQTYPE_INVALIDATION
      // L15_S3_MSHR_OP_UPDATE_ST_MSHR_IM_IF_INDEX_TAGCHECK_WAY_MATCHES 3912
      // let's not worry about this issue
        /*auto stbuf_compare_match_val_s3 = */
        /*  ((mshr_val_array[1](3) == 1) &  (mshr_st_state_array[1] == L15_MESI_TRANSITION_STATE_SM) & */
        /*   (mshr_st_address_array[1](39,4) == predecode_address(39,4))) |*/
        /*  ((mshr_val_array[0](3) == 1) &  (mshr_st_state_array[0] == L15_MESI_TRANSITION_STATE_SM) & */
        /*   (mshr_st_address_array[0](39,4) == predecode_address(39,4)))*/
        /*  ;*/

      // L15_S3_MESI_WRITE_TAGCHECK_WAY_S_IF_ME
      MapUpdate(instr, "address_to_mesi_map", address, (icache_type == 0) & tagcheck_state_me_s2, L15_MESI_STATE_S ) ; // s2 ?
      
      // icache  == 0 is different

      // decoder_mesi_write_op_s1 = `L15_S3_MESI_WRITE_TAGCHECK_WAY_S_IF_ME;
      /*
            if (predecode_is_last_inval_s1)
                decoder_noc3_operation_s1 = `L15_NOC3_GEN_DOWNGRADE_ACK_FROM_DCACHE;
            else
                decoder_noc3_operation_s1 = `L15_NOC3_GEN_DOWNGRADE_ACK_IF_TAGCHECK_M_FROM_DCACHE;*/

      instr.SetUpdate( l15_transducer_val                  , Ite( icache_type == 1, b1 , b0));
      instr.SetUpdate( l15_transducer_returntype           , Ite( icache_type == 1,  CPX_RESTYPE_INVAL, default_l15_transducer_returntype )       );
      instr.SetUpdate( l15_transducer_noncacheable         , default_l15_transducer_noncacheable                                                  );
      instr.SetUpdate( l15_transducer_atomic               , default_l15_transducer_atomic                                                        );
      instr.SetUpdate( l15_transducer_threadid             , default_l15_transducer_threadid                                                      );
      instr.SetUpdate( l15_transducer_data_0               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_data_1               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_data_2               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_data_3               , default_l15_transducer_data_0                                                        );
      instr.SetUpdate( l15_transducer_inval_address_15_4   , predecode_address(15,4)                                                              );
      instr.SetUpdate( l15_transducer_inval_icache_all_way , Ite( icache_type == 1, b1, default_l15_transducer_inval_icache_all_way )             );
      instr.SetUpdate( l15_transducer_inval_dcache_inval   , Ite( icache_type == 1, b0, default_l15_transducer_inval_dcache_inval )               );

      //                                                                                 L15_NOC3_GEN_ICACHE_INVAL_ACK                                        L15_NOC3_GEN_DOWNGRADE_ACK_FROM_DCACHE , L15_NOC3_GEN_DOWNGRADE_ACK_IF_TAGCHECK_M_FROM_DCACHE

      instr.SetUpdate( noc3_val           , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , b1                                , default_noc3_val           ) , Ite(predecode_is_last_inval_s1 , b1                         , Ite(tagcheck_state_m_s3,b1,b0)   )   ) ) ;
      instr.SetUpdate( noc3_type          , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , L15_NOC3_REQTYPE_ICACHE_INVAL_ACK , default_noc3_type          ) , Ite(predecode_is_last_inval_s1 , L15_NOC3_REQTYPE_DOWNGRADE_ACK , L15_NOC3_REQTYPE_DOWNGRADE_ACK                )   ) ) ;
      instr.SetUpdate( noc3_data_0        , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_data_0               , default_noc3_data_0        ) , Ite(predecode_is_last_inval_s1 , default_noc3_data_0        , default_noc3_data_0     )   ) ) ;
      instr.SetUpdate( noc3_data_1        , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_data_1               , default_noc3_data_1        ) , Ite(predecode_is_last_inval_s1 , default_noc3_data_1        , default_noc3_data_1     )   ) ) ;
      instr.SetUpdate( noc3_mshrid        , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_mshrid               , default_noc3_mshrid        ) , Ite(predecode_is_last_inval_s1 , default_noc3_mshrid        , default_noc3_mshrid     )   ) ) ;
      instr.SetUpdate( noc3_threadid      , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_threadid             , default_noc3_threadid      ) , Ite(predecode_is_last_inval_s1 , default_noc3_threadid      , default_noc3_threadid )   ) ) ;
      instr.SetUpdate( noc3_address       , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , predecode_address                 , default_noc3_address       ) , Ite(predecode_is_last_inval_s1 , predecode_address          , predecode_address           )   ) ) ;
      instr.SetUpdate( noc3_invalidate    , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_invalidate           , default_noc3_invalidate    ) , Ite(predecode_is_last_inval_s1 , default_noc3_invalidate    , default_noc3_invalidate )   ) ) ;
      instr.SetUpdate( noc3_with_data     , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_with_data            , default_noc3_with_data     ) , Ite(predecode_is_last_inval_s1 , Ite(tagcheck_state_m_s3    , b1 , b0) , b1 ) )   ) ;
      instr.SetUpdate( noc3_fwdack_vector , Ite(icache_type == 1 , Ite(predecode_is_last_inval_s1 , default_noc3_fwdack_vector        , default_noc3_fwdack_vector ) , Ite(predecode_is_last_inval_s1 , default_noc3_fwdack_vector , default_noc3_fwdack_vector  )   ) ) ;

      auto fetch_state_nxt = 
        Ite(fetch_state == L15_FETCH_STATE_NORMAL,
          Ite(icache_type == 1, L15_FETCH_STATE_ICACHE_INVAL_2, L15_FETCH_STATE_INVAL_2),
        Ite(fetch_state == L15_FETCH_STATE_INVAL_2, L15_FETCH_STATE_INVAL_3,
        Ite(fetch_state == L15_FETCH_STATE_INVAL_3, L15_FETCH_STATE_INVAL_4,
        Ite(fetch_state == L15_FETCH_STATE_ICACHE_INVAL_2, L15_FETCH_STATE_NORMAL, 
        L15_FETCH_STATE_NORMAL ) ) ) );

    instr.SetUpdate(fetch_state, fetch_state_nxt );

  }
    auto stbuf_compare_match_s3_0 = (mshr_val_array[0](3) == 1)
                                & (mshr_st_state_array[0] == L15_MESI_TRANSITION_STATE_SM) 
                                & (mshr_st_address_array[0](39,4) == predecode_address(39,4));
    auto stbuf_compare_match_s3_1 = (mshr_val_array[1](3) == 1)
                                & (mshr_st_state_array[1] == L15_MESI_TRANSITION_STATE_SM) 
                                & (mshr_st_address_array[1](39,4) == predecode_address(39,4));

  auto stbuf_compare_match_val_s3 = stbuf_compare_match_s3_0 | stbuf_compare_match_s3_1;

  {
    auto instr = model.NewInstr("DATA_ACK");

    instr.SetDecode( ( reqtype == MSG_TYPE_DATA_ACK)  );
    auto predecode_address_s1 = 
      Ite ( mshrid == L15_MSHR_ID_LD, 
        Ite( noc2decoder_l15_threadid == 0 , mshr_ld_address_array[0], mshr_ld_address_array[1] ),
      Ite ( mshrid == L15_MSHR_ID_ST,
        Ite( noc2decoder_l15_threadid == 0 , mshr_st_address_array[0], mshr_st_address_array[1] ),
      default_predecode_address_s1));

    auto predecode_icache_bit_s1 = predecode_mshr_read_control_s1(L15_CONTROL_ICACHE);

    auto predecode_reqtype_s1 = Ite(nc, 
      Ite(predecode_icache_bit_s1, L15_REQTYPE_ACKDT_IFILL, 
      Ite(predecode_dcache_load_s1, L15_REQTYPE_ACKDT_LD_NC,
      Ite(predecode_atomic_s1, L15_REQTYPE_ACK_ATOMIC,
        L15_REQTYPE_IGNORE
        ))),

      Ite(predecode_icache_bit_s1, L15_REQTYPE_ACKDT_IFILL, 
      Ite(predecode_dcache_load_s1, L15_REQTYPE_ACKDT_LD,
      Ite(predecode_dcache_noc2_store_im_s1, L15_REQTYPE_ACKDT_ST_IM,
      Ite(predecode_dcache_noc2_store_sm_s1, L15_REQTYPE_ACKDT_ST_SM,
        L15_REQTYPE_IGNORE
        ))))
      );

  // val:
  auto l15_transducer_val_next = 
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_IFILL,  b1, // L15_CPX_GEN_IFILL_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD_NC,  b1, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), Ite(lru_state_mes_s3 & wmt_compare_match_s3, b1, b0), // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state != L15_FETCH_STATE_NORMAL), b1, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACK_ATOMIC, b1, // L15_CPX_GEN_ATOMIC_ACK_FROM_NOC2 4060
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), Ite(lru_state_mes_s3 & wmt_compare_match_s3, b1, b0), // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID 3990
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state != L15_FETCH_STATE_NORMAL), b1, // L15_CPX_GEN_ST_ACK
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM, b1, // L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL
    default_l15_transducer_val
    ))))))));

  // type:
  auto l15_transducer_returntype_next = 
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_IFILL,  CPX_RESTYPE_IFILL1, // L15_CPX_GEN_IFILL_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD_NC, CPX_RESTYPE_LOAD,  // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), CPX_RESTYPE_INVAL, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state != L15_FETCH_STATE_NORMAL), CPX_RESTYPE_LOAD, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACK_ATOMIC, CPX_RESTYPE_ATOMIC_RES, // L15_CPX_GEN_ATOMIC_ACK_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), CPX_RESTYPE_INVAL, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state != L15_FETCH_STATE_NORMAL), CPX_RESTYPE_STORE_ACK, // L15_CPX_GEN_ST_ACK
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM, CPX_RESTYPE_STORE_ACK, // L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL
    default_l15_transducer_returntype
    ))))))));

  // invlidate dcache:
  l15_transducer_inval_dcache_inval_next = 
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_IFILL,  default_l15_transducer_inval_dcache_inval, // L15_CPX_GEN_IFILL_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD_NC, default_l15_transducer_inval_dcache_inval,  // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), b1, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state != L15_FETCH_STATE_NORMAL), default_l15_transducer_inval_dcache_inval, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACK_ATOMIC, default_l15_transducer_inval_dcache_inval, // L15_CPX_GEN_ATOMIC_ACK_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), b1,  // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state != L15_FETCH_STATE_NORMAL), default_l15_transducer_inval_dcache_inval,  // L15_CPX_GEN_ST_ACK
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM, Ite(wmt_compare_match_s3 && stbuf_compare_match_val_s3, b1, b0), // L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL
    default_l15_transducer_inval_dcache_inval
    ))))))));

  // atomic:
  auto l15_transducer_atomic_next = 
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_IFILL,  default_l15_transducer_atomic, // L15_CPX_GEN_IFILL_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD_NC, default_l15_transducer_atomic,  // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_atomic, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state != L15_FETCH_STATE_NORMAL), default_l15_transducer_atomic, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACK_ATOMIC, b1, // L15_CPX_GEN_ATOMIC_ACK_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_atomic, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state != L15_FETCH_STATE_NORMAL), default_l15_transducer_atomic, // L15_CPX_GEN_ST_ACK
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM, default_l15_transducer_atomic, // L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL
    default_l15_transducer_atomic
    ))))))));

  // data:
  auto l15_transducer_data_0_next =
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_IFILL,  data_0, // L15_CPX_GEN_IFILL_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD_NC,  data_0, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_data_0, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state != L15_FETCH_STATE_NORMAL), data_0, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACK_ATOMIC, data_0, // L15_CPX_GEN_ATOMIC_ACK_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_data_0, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state != L15_FETCH_STATE_NORMAL), default_l15_transducer_data_0, // L15_CPX_GEN_ST_ACK
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM, default_l15_transducer_data_0, // L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL
    default_l15_transducer_data_0
    ))))))));

  auto l15_transducer_data_1_next =
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_IFILL,  data_1, // L15_CPX_GEN_IFILL_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD_NC,  data_1, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_data_1, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state != L15_FETCH_STATE_NORMAL), data_1, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACK_ATOMIC, data_1, // L15_CPX_GEN_ATOMIC_ACK_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_data_1, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state != L15_FETCH_STATE_NORMAL), default_l15_transducer_data_1, // L15_CPX_GEN_ST_ACK
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM, default_l15_transducer_data_1, // L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL
    default_l15_transducer_data_1
    ))))))));

  auto l15_transducer_data_2_next =
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_IFILL,  data_2, // L15_CPX_GEN_IFILL_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD_NC,  data_2, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_data_2, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state != L15_FETCH_STATE_NORMAL), data_2, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACK_ATOMIC, data_2, // L15_CPX_GEN_ATOMIC_ACK_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_data_2, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state != L15_FETCH_STATE_NORMAL), default_l15_transducer_data_2, // L15_CPX_GEN_ST_ACK
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM, default_l15_transducer_data_2, // L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL
    default_l15_transducer_data_2
    ))))))));

  auto l15_transducer_data_3_next =
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_IFILL,  data_3, // L15_CPX_GEN_IFILL_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD_NC,  data_3, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_data_3, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state != L15_FETCH_STATE_NORMAL), data_3, // L15_CPX_GEN_LD_RESPONSE_FROM_NOC2
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACK_ATOMIC, data_3, // L15_CPX_GEN_ATOMIC_ACK_FROM_NOC2
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), default_l15_transducer_data_3, // L15_CPX_GEN_INVALIDATION_IF_LRU_MES_AND_WAYMAP_VALID
    Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state != L15_FETCH_STATE_NORMAL), default_l15_transducer_data_3, // L15_CPX_GEN_ST_ACK
    Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM, default_l15_transducer_data_3, // L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL
    default_l15_transducer_data_3
    ))))))));


  instr.SetUpdate( l15_transducer_val                  , l15_transducer_val_next );
  instr.SetUpdate( l15_transducer_returntype           , l15_transducer_returntype_next );
  instr.SetUpdate( l15_transducer_noncacheable         , default_l15_transducer_noncacheable );
  instr.SetUpdate( l15_transducer_atomic               , l15_transducer_atomic_next );
  instr.SetUpdate( l15_transducer_threadid             , default_l15_transducer_threadid );
  instr.SetUpdate( l15_transducer_data_0               , l15_transducer_data_0_next               );
  instr.SetUpdate( l15_transducer_data_1               , l15_transducer_data_1_next               );
  instr.SetUpdate( l15_transducer_data_2               , l15_transducer_data_2_next               );
  instr.SetUpdate( l15_transducer_data_3               , l15_transducer_data_3_next               );
  instr.SetUpdate( l15_transducer_inval_address_15_4   , predecode_address_s1(15,4) );
  instr.SetUpdate( l15_transducer_inval_icache_all_way , b0 );
  instr.SetUpdate( l15_transducer_inval_dcache_inval   , l15_transducer_inval_dcache_inval_next );

  auto noc1_val_next = 
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), Ite( lru_state_m_s3, b1, b0), // L15_NOC1_GEN_WRITEBACK_GUARD_IF_LRU_M
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), Ite( lru_state_m_s3, b1, b0), // L15_NOC1_GEN_WRITEBACK_GUARD_IF_LRU_M
      default_noc1_val
      );
  auto noc1_address_next =    
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), lru_way_address_s3, // L15_NOC1_GEN_WRITEBACK_GUARD_IF_LRU_M
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), lru_way_address_s3, // L15_NOC1_GEN_WRITEBACK_GUARD_IF_LRU_M
      default_noc1_address
      );
  auto noc1_type_next =  
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), L15_NOC1_REQTYPE_WRITEBACK_GUARD, // L15_NOC1_GEN_WRITEBACK_GUARD_IF_LRU_M
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), L15_NOC1_REQTYPE_WRITEBACK_GUARD, // L15_NOC1_GEN_WRITEBACK_GUARD_IF_LRU_M
      default_noc1_type
      );      


  instr.SetUpdate( noc1_val          , noc1_val_next             ) ;
  instr.SetUpdate( noc1_address      , noc1_address_next         ) ;
  instr.SetUpdate( noc1_noncacheable , default_noc1_noncacheable ) ;
  instr.SetUpdate( noc1_size         , default_noc1_size         ) ;
  instr.SetUpdate( noc1_threadid     , default_noc1_threadid     ) ;
  instr.SetUpdate( noc1_mshrid       , default_noc1_mshrid       ) ;
  instr.SetUpdate( noc1_type         , noc1_type_next            ) ;
  instr.SetUpdate( noc1_data_0       , default_noc1_data_0       ) ;
  instr.SetUpdate( noc1_data_1       , default_noc1_data_1       ) ;


  Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_IFILL,  // default
  Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD_NC,  // default
  Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
  Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state != L15_FETCH_STATE_NORMAL), // 
  Ite(predecode_reqtype_s1 == L15_REQTYPE_ACK_ATOMIC, // 
  Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
  Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state != L15_FETCH_STATE_NORMAL), // default
  Ite(predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM, // default
  // default
  )

auto noc3_val_next =     
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), Ite(lru_state_m_s3, b1, b0), // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), Ite(lru_state_m_s3, b1, b0), // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
      default_noc1_type
      );            
auto noc3_type_next =    
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), L15_NOC3_REQTYPE_WRITEBACK, // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), L15_NOC3_REQTYPE_WRITEBACK, // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
      default_noc1_type
      );                  
auto noc3_address_next =  
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), lru_way_address_s3, // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), lru_way_address_s3, // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
      default_noc1_type
      );                 
auto noc3_with_data_next = 
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) & (fetch_state == L15_FETCH_STATE_NORMAL), b1, // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
      Ite( (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM) & (fetch_state == L15_FETCH_STATE_NORMAL), b1, // L15_NOC3_GEN_WRITEBACK_IF_LRU_M_FROM_DCACHE
      default_noc1_type
      );                

instr.SetUpdate( noc3_val          , noc3_val_next              ) ;
instr.SetUpdate( noc3_type         , noc3_type_next             ) ;
instr.SetUpdate( noc3_data_0       , default_noc3_data_0        ) ;
instr.SetUpdate( noc3_data_1       , default_noc3_data_1        ) ;
instr.SetUpdate( noc3_mshrid       , default_noc3_mshrid        ) ;
instr.SetUpdate( noc3_threadid     , default_noc3_threadid      ) ;
instr.SetUpdate( noc3_address      , noc3_address_next          ) ;
instr.SetUpdate( noc3_invalidate   , default_noc3_invalidate    ) ;
instr.SetUpdate( noc3_with_data    , noc3_with_data_next        ) ;
instr.SetUpdate( noc3_fwdack_vector, default_noc3_fwdack_vector ) ;


      auto fetch_state_nxt = 
        Ite(fetch_state == L15_FETCH_STATE_NORMAL,
          Ite( 
            (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD) | 
            (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM), 
            L15_FETCH_STATE_NOC2_WRITEBACK_DONE, L15_FETCH_STATE_NORMAL),
        Ite(fetch_state == L15_FETCH_STATE_NOC2_WRITEBACK_DONE, L15_FETCH_STATE_NORMAL,
            L15_FETCH_STATE_NORMAL ) );

    instr.SetUpdate(fetch_state, fetch_state_nxt );

    MapUpdate(instr, "address_to_mesi_map", address, 
      (
      (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD & fetch_state != L15_FETCH_STATE_NORMAL) |    // L15_S3_MESI_WRITE_LRU_WAY_ACK_STATE   2852
      (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM & fetch_state != L15_FETCH_STATE_NORMAL) | // L15_S3_MESI_WRITE_LRU_WAY_ACK_STATE
      (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_SM)                                           // L15_S3_MESI_WRITE_MSHR_WAY_ACK_STATE
      )  , mesi_ack_state
      );

    MapUpdate(instr, "address_to_mshr_map", threadid, 
      ! (
      (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD & fetch_state == L15_FETCH_STATE_NORMAL) |
      (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM & fetch_state == L15_FETCH_STATE_NORMAL) )
       , b0);

    // s3_mshr_val_s3 = val_s3 && stbuf_compare_lru_match_val_s3; but we don't care
    MapUpdate(instr, "address_to_mshr_st_state", threadid, 
      (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_LD & fetch_state == L15_FETCH_STATE_NORMAL) |  // L15_S3_MSHR_OP_UPDATE_ST_MSHR_IM_IF_INDEX_LRU_WAY_MATCHES: 3921
      (predecode_reqtype_s1 == L15_REQTYPE_ACKDT_ST_IM & fetch_state == L15_FETCH_STATE_NORMAL) // L15_S3_MSHR_OP_UPDATE_ST_MSHR_IM_IF_INDEX_LRU_WAY_MATCHES
      , Concat(L15_MESI_TRANSITION_STATE_IM,L15_MESI_TRANSITION_STATE_IM) ); // if ME deallocate
      
  }

  {
    auto instr = model.NewInstr("NODATA_ACK");

    instr.SetDecode( ( reqtype == MSG_TYPE_NODATA_ACK)  );

    // 612
    auto predecode_icache_bit_s1 = predecode_mshr_read_control_s1(L15_CONTROL_ICACHE);
    auto predecode_address_s1 = Ite( threadid == 0, mshr_st_address_array[0], mshr_st_address_array[1] );

    /*
    Ite(mshrid == L15_MSHR_ID_ST,
      L15_REQTYPE_ACK_WRITETHROUGH // 1692
      L15_REQTYPE_ACK_PREFETCH // do nothing
      );
            decoder_s3_mshr_operation_s1 = `L15_S3_MSHR_OP_DEALLOCATION;
            decoder_cpx_operation_s1 = `L15_CPX_GEN_ST_ACK;
    */
    // L15_CPX_GEN_ST_ACK 4028
    instr.SetUpdate( l15_transducer_val                  , Ite(mshrid == L15_MSHR_ID_ST , b1                                        , default_l15_transducer_val                  ));
    instr.SetUpdate( l15_transducer_returntype           , Ite(mshrid == L15_MSHR_ID_ST , CPX_RESTYPE_STORE_ACK                     , default_l15_transducer_returntype           ));
    instr.SetUpdate( l15_transducer_noncacheable         , Ite(mshrid == L15_MSHR_ID_ST , default_l15_transducer_noncacheable       , default_l15_transducer_noncacheable         ));
    instr.SetUpdate( l15_transducer_atomic               , Ite(mshrid == L15_MSHR_ID_ST , b0                                        , default_l15_transducer_atomic               ));
    instr.SetUpdate( l15_transducer_threadid             , Ite(mshrid == L15_MSHR_ID_ST , default_l15_transducer_threadid           , default_l15_transducer_threadid             ));
    instr.SetUpdate( l15_transducer_data_0               , Ite(mshrid == L15_MSHR_ID_ST , default_l15_transducer_data_0             , default_l15_transducer_data_0               ));
    instr.SetUpdate( l15_transducer_data_1               , Ite(mshrid == L15_MSHR_ID_ST , default_l15_transducer_data_1             , default_l15_transducer_data_1               ));
    instr.SetUpdate( l15_transducer_data_2               , Ite(mshrid == L15_MSHR_ID_ST , default_l15_transducer_data_2             , default_l15_transducer_data_2               ));
    instr.SetUpdate( l15_transducer_data_3               , Ite(mshrid == L15_MSHR_ID_ST , default_l15_transducer_data_3             , default_l15_transducer_data_3               ));
    instr.SetUpdate( l15_transducer_inval_address_15_4   , Ite(mshrid == L15_MSHR_ID_ST , default_l15_transducer_inval_address_15_4 , default_l15_transducer_inval_address_15_4   ));
    instr.SetUpdate( l15_transducer_inval_icache_all_way , Ite(mshrid == L15_MSHR_ID_ST , b0                                        , default_l15_transducer_inval_icache_all_way ));
    instr.SetUpdate( l15_transducer_inval_dcache_inval   , Ite(mshrid == L15_MSHR_ID_ST , b0                                        , default_l15_transducer_inval_dcache_inval   ));

    // L15_S3_MSHR_OP_DEALLOCATION
    MapUpdate(instr, "address_to_mshr_map", threadid, mshrid == L15_MSHR_ID_ST, b0 ); // if ME deallocate
  }

  {
    auto instr = model.NewInstr("INTERRUPT");

    instr.SetDecode( ( reqtype == MSG_TYPE_INTERRUPT)  );
    // 627
    // predecode_reqtype_s1 = `L15_REQTYPE_L2_INTERRUPT;
    // has the issue of L15_CPX_GEN_INTERRUPT 4073 4121
    instr.SetUpdate( l15_transducer_val                  , b1);
    instr.SetUpdate( l15_transducer_returntype           , CPX_RESTYPE_INTERRUPT);
    instr.SetUpdate( l15_transducer_noncacheable         , b0);
    instr.SetUpdate( l15_transducer_atomic               , b0);
    instr.SetUpdate( l15_transducer_threadid             , b0);
    instr.SetUpdate( l15_transducer_data_0               , data_0);  // L15_CPX_SOURCE_NOC2_BUFFER
    instr.SetUpdate( l15_transducer_data_1               , data_1);
    instr.SetUpdate( l15_transducer_data_2               , data_2);
    instr.SetUpdate( l15_transducer_data_3               , data_3);
    instr.SetUpdate( l15_transducer_inval_address_15_4   , BvConst(0,12));
    instr.SetUpdate( l15_transducer_inval_icache_all_way , b0);
    instr.SetUpdate( l15_transducer_inval_dcache_inval   , b0);
  }
// not specifying these updates:
// l15_noc1buffer_req_data0  
// l15_noc1buffer_req_data1  
// l15_noc1buffer_csm_data   
// l15_noc1buffer_csm_ticket 
// l15_noc1buffer_req_homeid 
// l15_noc1buffer_req_mshrid 
// l15_noc1buffer_req_noncacheable
// l15_noc1buffer_req_prefetch   
// l15_noc1buffer_req_threadid 
}
