/// \file the ila example of AES block encryption
///  Hongce Zhang (hongcez@princeton.edu)
///

#include "pmesh_l15_pcx_ila.h"

// some assumptions on the interface:
// blockinitstore : 0
// blockstore : 0
// csm_data : 0
// data_next_entry : 0
// l1rplway : 0
// prefetch : 0
// threadid : 0

// a request if not hit
// go to the NoC
// if hit, ?

PMESH_L15_PCX_ILA::PMESH_L15_PCX_ILA()
    : // construct the model
      PMESH_L15_ILA("PMESH_L15_PCX_ILA"),
      // I/O interface: this is where the commands come from.
      address   (model.NewBvInput("transducer_l15_address"              , ADDR_WIDTH))          ,
      data      (model.NewBvInput("transducer_l15_data"                 , DATA_WIDTH))          ,
      data_next (model.NewBvInput("transducer_l15_data_next_entry"      , DATA_WIDTH))          ,
      nc        (model.NewBvInput("transducer_l15_nc"                   , BOOL_WIDTH))          ,
      rqtype    (model.NewBvInput("transducer_l15_rqtype"               , NOC1_REQ_TYPE_WIDTH)) ,
      size      (model.NewBvInput("transducer_l15_size"                 , NOC1_REQ_SIZE_WIDTH)) ,
      threadid  (model.NewBvInput("transducer_l15_threadid"             , BOOL_WIDTH))          ,
      val       (model.NewBvInput("transducer_l15_val"                  , BOOL_WIDTH))          ,
      amo_op    (model.NewBvInput("transducer_l15_amo_op"               , AMO_OP_WIDTH))        , // pcxdecoder_l15_amo_op L15_AMO_OP_WIDTH
      invalidate(model.NewBvInput("transducer_l15_invalidate_cacheline" , BOOL_WIDTH))          , // pcxdecoder_l15_invalidate_cacheline
      
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
      mesi_state    ( NewMap( "address_to_mesi_map", ADDR_WIDTH, MESI_WIDTH ) ), // write (l15_mesi_write_data_s2[1:0]) read mesi_state_way0_s2? ?flush_state_s2
      data_state    ( NewMap( "address_to_data_map", ADDR_WIDTH, DATA_WIDTH) ), // read (noc3: dcache_l15_dout_s3) write: (dcache_write_data_s2)
      missed_on_this( NewMap("address_to_mshr_map", ADDR_WIDTH, BOOL_WIDTH) ), // 
      mshr_data     ( NewMap("address_to_mshr_data_map", ADDR_WIDTH, 2*DATA_WIDTH) ),
      // l1d_way       ( NewMap("address_to_l1d_way_map", ADDR_WIDTH, WAY_WIDTH) ), // wmt_compare_match_way_s3
      fetch_state   ( model.NewBvState("fetch_state_s1", FETCH_STATE_WIDTH) ),


      // -----------------------------------------------------------------------------------------------

    {

  auto wmt_compare_func = FuncRef("wmt_compare_match_s3", SortRef::BOOL());
  auto wmt_compare_match_s3 = wmt_compare_func();

  // ------------------------------ CONSTANTS ---------------------------------- //

  auto L15_MSHR_ID_DEFAULT = BvConst(0, MSHR_ID_WIDTH);
  auto L15_MSHR_ID_IFILL   = BvConst(1, MSHR_ID_WIDTH);
  auto L15_MSHR_ID_LD      = BvConst(2, MSHR_ID_WIDTH);
  auto L15_MSHR_ID_ST      = BvConst(3, MSHR_ID_WIDTH);

  auto MESI_INVALID   = BvConst(0,MESI_WIDTH);
  auto MESI_SHARED    = BvConst(1,MESI_WIDTH);
  auto MESI_EXCLUSIVE = BvConst(2,MESI_WIDTH);
  auto MESI_MODIFIED  = BvConst(3,MESI_WIDTH);

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

  auto L15_FETCH_STATE_NORMAL = BvConst(0, FETCH_STATE_WIDTH);
  auto L15_FETCH_STATE_PCX_WRITEBACK_DONE = BvConst(1, FETCH_STATE_WIDTH);

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


// ------------------------------ DEFAULT VALUES ---------------------------------- //
// noc1 default values : 4191
  auto default_noc1_val          = b0;                              // unknown(BOOL_WIDTH)();
  auto default_noc1_address      = BvConst(0,ADDR_WIDTH );          // unknown(ADDR_WIDTH)() ;
  auto default_noc1_noncacheable = nc;                              // unknown(BOOL_WIDTH)()  ;
  auto default_noc1_size         = size;                            // unknown(NOC1_REQ_SIZE_WIDTH)()  ;
  auto default_noc1_type         = BvConst(0, NOC1_REQ_TYPE_WIDTH); // unknown(NOC1_REQ_TYPE_WIDTH)()  ;
  auto default_noc1_threadid     = threadid;                        // unknown(THREADID_WIDTH)();
  auto default_noc1_mshrid       = L15_MSHR_ID_DEFAULT;             // unknown(MSHR_ID_WIDTH)();
  auto default_noc1_data_0       = zero_data;                       // unknown(DATA_WIDTH)() ;
  auto default_noc1_data_1       = zero_data;                       // unknown(DATA_WIDTH)() ;

  // l15 default value 4139
  auto default_l15_transducer_val                  = b0;                               // unknown(BOOL_WIDTH)();
  auto default_l15_transducer_returntype           = BvConst(0, CPX_RETURNTYPE_WIDTH); // unknown(CPX_RETURNTYPE_WIDTH)();
  auto default_l15_transducer_noncacheable         = nc;                               // unknown(BOOL_WIDTH)();
  auto default_l15_transducer_atomic               = b0;                               // unknown(BOOL_WIDTH)();
  auto default_l15_transducer_data_0               = zero_data;                        // unknown(DATA_WIDTH)();
  auto default_l15_transducer_data_1               = zero_data;                        // unknown(DATA_WIDTH)();
  auto default_l15_transducer_data_2               = zero_data;                        // unknown(DATA_WIDTH)();
  auto default_l15_transducer_data_3               = zero_data;                        // unknown(DATA_WIDTH)();
  auto default_l15_transducer_inval_address_15_4   = address(15,4);                    // unknown(12)();
  auto default_l15_transducer_inval_icache_all_way = b0;                               // unknown(BOOL_WIDTH)();           // from cpx_icache_inval_s3
  auto default_l15_transducer_inval_dcache_inval   = b0;                               // unknown(BOOL_WIDTH)();           // from cpx_invalidate_l1_s3
  // auto default_l15_transducer_inval_way            = Map( "address_to_l1d_way_map",  WAY_WIDTH, address ); // unknown(WAY_WIDTH);

  // 4413
  auto default_noc3_val          = b0;                                    // unknown(BOOL_WIDTH)();
  auto default_noc3_type         = BvConst(0, NOC3_REQ_TYPE_WIDTH);       // unknown(NOC3_REQ_TYPE_WIDTH)();
  auto default_noc3_data_0       = Map("address_to_data_map", 2*DATA_WIDTH, address) (127, 64);                   // unknown(DATA_WIDTH)();
  auto default_noc3_data_1       = Map("address_to_data_map", 2*DATA_WIDTH, address) ( 63,  0);                   // unknown(DATA_WIDTH)();
  auto default_noc3_mshrid       = L15_MSHR_ID_DEFAULT;                   // unknown(MSHR_ID_WIDTH)();
  auto default_noc3_threadid     = threadid;                              // unknown(THREADID_WIDTH)();
  auto default_noc3_address      = BvConst(0, ADDR_WIDTH);                // unknown(ADDR_WIDTH)();
  auto default_noc3_invalidate   = b0;                                    // unknown(BOOL_WIDTH)();
  auto default_noc3_with_data    = b0;                                    // unknown(BOOL_WIDTH)();
  auto default_noc3_fwdack_vector  = BvConst(0, FWD_SUBCACHELINE_VECTOR); // unknown(FWD_SUBCACHELINE_VECTOR)();

  // ------------------------------ INSTRUCTIONS ---------------------------------- //

  // L1.5 fetch function -- what corresponds to instructions on L1.5 PCX interface
  model.SetFetch( lConcat({address, data, nc, rqtype, size, val, threadid, amo_op, invalidate })   );
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
    auto instr = model.NewInstr("LOAD_normal");

    instr.SetDecode( 
      ( rqtype == PCX_REQTYPE_LOAD) & (nc == 0) & (invalidate == 0)  & (fetch_state == L15_FETCH_STATE_NORMAL) );

    auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map
    auto DATA_cache = Map( "address_to_data_map", 128, address ); // Use the map

    auto hit = MESI_state != MESI_INVALID;

    // on miss : send out noc1 request eventually
    instr.SetUpdate(noc1_val          , Ite(! hit , b1                          , default_noc1_val         ));
    instr.SetUpdate(noc1_address      , Ite(! hit , address                     , default_noc1_address     ));
    instr.SetUpdate(noc1_noncacheable , Ite(! hit , b0                          , default_noc1_noncacheable));
    instr.SetUpdate(noc1_size         , Ite(! hit , size                        , default_noc1_size        ));
    instr.SetUpdate(noc1_type         , Ite(! hit , L15_NOC1_REQTYPE_LD_REQUEST , default_noc1_type        ));
    instr.SetUpdate(noc1_threadid     , Ite(! hit , threadid                    , default_noc1_threadid    ));
    instr.SetUpdate(noc1_mshrid       , Ite(! hit , L15_MSHR_ID_LD              , default_noc1_mshrid      ));
    instr.SetUpdate(noc1_data_0       , Ite(! hit , zero_data                   , default_noc1_data_0      ));
    instr.SetUpdate(noc1_data_1       , Ite(! hit , zero_data                   , default_noc1_data_1      ));
   
    // on the hit side : return the data on cpx
         
    instr.SetUpdate( l15_transducer_val                  , Ite( hit , b1               ,         default_l15_transducer_val ));
    instr.SetUpdate( l15_transducer_returntype           , Ite( hit , CPX_RESTYPE_LOAD ,         default_l15_transducer_returntype ) );
    instr.SetUpdate( l15_transducer_data_0               , Ite( hit , DATA_cache(127,64)       , default_l15_transducer_data_0  ) );
    instr.SetUpdate( l15_transducer_data_1               , Ite( hit , DATA_cache( 63, 0)       , default_l15_transducer_data_1  ) );
    instr.SetUpdate( l15_transducer_noncacheable         , Ite( hit , b0               ,         default_l15_transducer_noncacheable ) );
    instr.SetUpdate( l15_transducer_atomic               , Ite( hit , b0               ,         default_l15_transducer_atomic ) );
    instr.SetUpdate( l15_transducer_inval_icache_all_way , Ite( hit , b0               ,         default_l15_transducer_inval_icache_all_way ));
    instr.SetUpdate( l15_transducer_inval_dcache_inval   , Ite( hit , b0               ,         default_l15_transducer_inval_dcache_inval ));

    MapUpdate(instr, "address_to_mshr_map", address, Ite(hit, b0, b1) );

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
    auto instr = model.NewInstr("STORE_normal");

    instr.SetDecode( ( rqtype == PCX_REQTYPE_STORE) & (nc == 0) & (invalidate == 0)  & (fetch_state == L15_FETCH_STATE_NORMAL) );

    auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map
    auto DATA_cache = Map( "address_to_data_map", 128, address ); // Use the map
    auto missed_on_this = Map( "address_to_mshr_map", 1, address ); // Use the map
    auto mshr_data  = Map( "address_to_mshr_data_map", 2*DATA_WIDTH, address);

    auto hit = MESI_state != MESI_INVALID;
    auto writable = (MESI_state == MESI_EXCLUSIVE) | (MESI_state == MESI_MODIFIED);

    // write-back
    // eviction is upon ack-dt
    // Send ReqExRd if necessary  

    // L15_NOC1_GEN_DATA_ST_UPGRADE_IF_TAGCHECK_S_ELSE_ST_FILL_IF_TAGCHECK_I  4256
    instr.SetUpdate( noc1_val          , Ite(! writable & ! missed_on_this   , b1 , default_noc1_val ));
    instr.SetUpdate( noc1_address      , Ite(! writable & ! missed_on_this   , address                           , default_noc1_address  ) );
    instr.SetUpdate( noc1_noncacheable , Ite(! writable & ! missed_on_this   , b0                                , default_noc1_noncacheable  ) );
    instr.SetUpdate( noc1_size         , Ite(! writable & ! missed_on_this   , size                              , default_noc1_size  ) );
    instr.SetUpdate( noc1_threadid     , Ite(! writable & ! missed_on_this   , threadid                          , default_noc1_threadid ));
    instr.SetUpdate( noc1_mshrid       , Ite(! writable & ! missed_on_this   , L15_MSHR_ID_ST                    , default_noc1_mshrid ));
    instr.SetUpdate( noc1_type         ,
      Ite(! writable & ! missed_on_this,
      Ite(MESI_state == MESI_SHARED    , L15_NOC1_REQTYPE_ST_UPGRADE_REQUEST , L15_NOC1_REQTYPE_ST_FILL_REQUEST) , default_noc1_type ) );
    instr.SetUpdate( noc1_data_0       , Ite(! writable & ! missed_on_this   , data                              , default_noc1_data_0 ));
    instr.SetUpdate( noc1_data_1       , Ite(! writable & ! missed_on_this   , zero_data                         , default_noc1_data_1 ));
       

    // if hit mshr : L15_CPX_GEN_ST_ACK_WITH_POSSIBLE_INVAL  // 4033  
    // if not mshr : L15_CPX_GEN_ST_ACK_IF_TAGCHECK_M_E_WITH_POSSIBLE_INVAL
    instr.SetUpdate( l15_transducer_val                   , Ite( missed_on_this | writable , b1                    , default_l15_transducer_val) );
    instr.SetUpdate( l15_transducer_returntype            , Ite( missed_on_this | writable , CPX_RESTYPE_STORE_ACK , default_l15_transducer_returntype));
    instr.SetUpdate( l15_transducer_noncacheable          , Ite( missed_on_this | writable , b0                    , default_l15_transducer_noncacheable));
    instr.SetUpdate( l15_transducer_atomic                , Ite( missed_on_this | writable , b0                    , default_l15_transducer_atomic));
    //instr.SetUpdate( l15_transducer_data_0              , Ite( missed_on_this | writable ,                       , unknown(DATA_WIDTH)() ));
    //instr.SetUpdate( l15_transducer_data_1              , Ite( missed_on_this | writable ,                       , unknown(DATA_WIDTH)() ));
    //instr.SetUpdate( l15_transducer_data_2              , Ite( missed_on_this | writable ,                       , unknown(DATA_WIDTH)() ));
    //instr.SetUpdate( l15_transducer_data_3              , Ite( missed_on_this | writable ,                       , unknown(DATA_WIDTH)() ));
    // instr.SetUpdate( l15_transducer_inval_address_15_4 , Ite( missed_on_this | writable , address(15            , 4)                                  , unknown(12)() ));
    instr.SetUpdate( l15_transducer_inval_icache_all_way  , Ite( missed_on_this | writable , b0                    , default_l15_transducer_inval_icache_all_way ));
    //instr.SetUpdate( l15_transducer_inval_dcache_inval  , Ite( missed_on_this | writable ,                       , unknown(BOOL_WIDTH)() ));
    
    MapUpdate(instr, "address_to_mshr_data_map", address, missed_on_this,  Concat(data, data) ); // if hit mshr

    // L15_S3_MESI_WRITE_TAGCHECK_WAY_M_IF_E
    MapUpdate(instr, "address_to_mesi_map", address, writable & !missed_on_this , MESI_MODIFIED ) ; // unknown ?? (the write enable signal)
    
    // L15_DCACHE_WRITE_TAGCHECK_WAY_IF_ME_FROM_MSHR 2677
    MapUpdate(instr, "address_to_data_map", address, writable & !missed_on_this, mshr_data ); // unknown ??

    // L15_S3_MSHR_OP_DEALLOCATION_IF_TAGCHECK_M_E_ELSE_UPDATE_STATE_STMSHR 3902
    MapUpdate(instr, "address_to_mshr_map", address, writable & ! missed_on_this, b0 ); // if ME deallocate

    // do we need to send-back response
    // there is an ack
  }

  // assumption: you cannot have load upon load miss on the same thread
  { // non cache

    auto instr = model.NewInstr("LOAD_nc"); 
    // 656 L15_REQTYPE_LOAD_NC, it is the flush first
    // 1134
    instr.SetDecode( ( rqtype == PCX_REQTYPE_LOAD) & (nc == 1) & (fetch_state == L15_FETCH_STATE_NORMAL) );

    auto MESI_state = Map( "address_to_mesi_map",  MESI_WIDTH, address ); // Use the map
    auto DATA_cache = Map( "address_to_data_map", 128, address ); // Use the map

    auto dirty_data = Map( "address_to_data_map",  DATA_WIDTH, address ); // Use the map ???

    auto mod = MESI_state == MESI_MODIFIED;
    auto hit = MESI_state != MESI_INVALID;

    instr.SetUpdate(fetch_state, L15_FETCH_STATE_PCX_WRITEBACK_DONE);

    // L15_NOC1_GEN_WRITEBACK_GUARD_IF_TAGCHECK_M  4216
    instr.SetUpdate( noc1_val          , Ite( mod , b1                               , default_noc1_val          ));
    instr.SetUpdate( noc1_address      , Ite( mod , address                          , default_noc1_address      ));
    instr.SetUpdate( noc1_noncacheable , Ite( mod , b1                               , default_noc1_noncacheable ));
    instr.SetUpdate( noc1_size         , Ite( mod , size                             , default_noc1_size         ));
    instr.SetUpdate( noc1_threadid     , Ite( mod , threadid                         , default_noc1_threadid     ));
    instr.SetUpdate( noc1_mshrid       , Ite( mod , L15_MSHR_ID_LD                   , default_noc1_mshrid       ));
    instr.SetUpdate( noc1_type         , Ite( mod , L15_NOC1_REQTYPE_WRITEBACK_GUARD , default_noc1_type         ));
    instr.SetUpdate( noc1_data_0       , Ite( mod , zero_data                        , default_noc1_data_0       ));
    instr.SetUpdate( noc1_data_1       , Ite( mod , zero_data                        , default_noc1_data_1       ));


    // L15_CPX_GEN_INVALIDATION_IF_TAGCHECK_MES_AND_WAYMAP_VALID 3983
    instr.SetUpdate( l15_transducer_val                   , Ite( hit & wmt_compare_match_s3 , b1                , default_l15_transducer_val                    ));
    instr.SetUpdate( l15_transducer_returntype            , Ite( hit                        , CPX_RESTYPE_INVAL , default_l15_transducer_returntype             ));
    instr.SetUpdate( l15_transducer_noncacheable          , Ite( hit                        , b0                , default_l15_transducer_noncacheable           ));
    instr.SetUpdate( l15_transducer_atomic                , Ite( hit                        , b0                , default_l15_transducer_atomic                 ));
    instr.SetUpdate( l15_transducer_data_0                , Ite( hit                        , zero_data         , default_l15_transducer_data_0                 ));
    instr.SetUpdate( l15_transducer_data_1                , Ite( hit                        , zero_data         , default_l15_transducer_data_1                 ));
    instr.SetUpdate( l15_transducer_data_2                , Ite( hit                        , zero_data         , default_l15_transducer_data_2                 ));
    instr.SetUpdate( l15_transducer_data_3                , Ite( hit                        , zero_data         , default_l15_transducer_data_3                 ));
    instr.SetUpdate( l15_transducer_inval_address_15_4    , Ite( hit                        , default_l15_transducer_inval_address_15_4                , default_l15_transducer_inval_address_15_4     ));
    instr.SetUpdate( l15_transducer_inval_icache_all_way  , Ite( hit                        , b0                , default_l15_transducer_inval_icache_all_way   ));
    // instr.SetUpdate( l15_transducer_inval_dcache_inval , Ite(                            ,                   ,  )); depends on way

    // L15_NOC3_GEN_WRITEBACK_IF_TAGCHECK_M_FROM_DCACHE 4428
    instr.SetUpdate( noc3_val           , Ite( mod , b1                         ,         default_noc3_val           ));
    instr.SetUpdate( noc3_type          , Ite( mod , L15_NOC3_REQTYPE_WRITEBACK ,         default_noc3_type          ));
    instr.SetUpdate( noc3_data_0        , Ite( mod , DATA_cache(127, 64)        ,         default_noc3_data_0        ));
    instr.SetUpdate( noc3_data_1        , Ite( mod , DATA_cache( 63, 0)         ,         default_noc3_data_1        ));
    instr.SetUpdate( noc3_mshrid        , Ite( mod , L15_MSHR_ID_LD             ,         default_noc3_mshrid        ));
    instr.SetUpdate( noc3_threadid      , Ite( mod , threadid                   ,         default_noc3_threadid      ));
    instr.SetUpdate( noc3_address       , Ite( mod , address                    ,         default_noc3_address       ));
    instr.SetUpdate( noc3_invalidate    , Ite( mod , b0                         ,         default_noc3_invalidate    )); // will not care about it from noc2
    instr.SetUpdate( noc3_with_data     , Ite( mod , b1                         ,         default_noc3_with_data     ));
    instr.SetUpdate( noc3_fwdack_vector , Ite( mod , BvConst(0, FWD_SUBCACHELINE_VECTOR), default_noc3_fwdack_vector ));

    // L15_S3_MESI_INVALIDATE_TAGCHECK_WAY_IF_MES
    MapUpdate(inst, "address_to_mesi_map", address, hit , BvConst(MESI_INVALID,2) ); // unknown ??

    // no update to data 
    // MapUpdate(instr, "address_to_data_map", address, writable & !missed_on_this, data  ); // unknown ??

    // 
    // no mshr update

    // TODO: invalidation to L1 -- child
  }


  { // non cache

    auto instr = model.NewInstr("STORE_nc"); // write_through L15_REQTYPE_WRITETHROUGH 684 1242

    instr.SetDecode( ( rqtype == PCX_REQTYPE_STORE ) & (nc == 1) & (fetch_state == L15_FETCH_STATE_NORMAL) );

    //`define L15_INT_VEC_DIS 40'h9800000800
    auto predecode_int_vec_dis_s1 = (address(39,32) == BvConst(0x98,8) ) & (address(11:8) == BvConst(0x8,4) );

    auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map

    auto mod = MESI_state == MESI_MODIFIED;
    auto hit = MESI_state != MESI_INVALID;

    instr.SetUpdate(fetch_state, Ite(predecode_int_vec_dis_s1, L15_FETCH_STATE_NORMAL, L15_FETCH_STATE_PCX_WRITEBACK_DONE));

    // if interrupt    L15_NOC1_GEN_INTERRUPT_FWD   4347
    // if not inerrupt L15_NOC1_GEN_WRITEBACK_GUARD_IF_TAGCHECK_M 
    instr.SetUpdate( noc1_val          , Ite( mod | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1 , b1                             , b1                                ) , default_noc1_val          ));
    instr.SetUpdate( noc1_address      , Ite( mod | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1 , address                        , address                           ) , default_noc1_address      ));
    instr.SetUpdate( noc1_noncacheable , Ite( mod | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1 , b1                             , b1                                ) , default_noc1_noncacheable ));
    instr.SetUpdate( noc1_size         , Ite( mod | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1 , size                           , size                              ) , default_noc1_size         ));
    instr.SetUpdate( noc1_threadid     , Ite( mod | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1 , threadid                       , threadid                          ) , default_noc1_threadid     ));
    instr.SetUpdate( noc1_mshrid       , Ite( mod | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1 , L15_MSHR_ID_DEFAULT            , L15_MSHR_ID_ST                    ) , default_noc1_mshrid       ));
    instr.SetUpdate( noc1_type         , Ite( mod | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1 , L15_NOC1_REQTYPE_INTERRUPT_FWD , L15_NOC1_REQTYPE_WRITEBACK_GUARD  ) , default_noc1_type         ));
    instr.SetUpdate( noc1_data_0       , Ite( mod | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1 , data                           , zero_data                         ) , default_noc1_data_0       ));
    instr.SetUpdate( noc1_data_1       , Ite( mod | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1 , zero_data                      , zero_data                         ) , default_noc1_data_1       ));

// if interrupt    L15_CPX_GEN_ST_ACK   4028
// if no interrupt L15_CPX_GEN_INVALIDATION_IF_TAGCHECK_MES_AND_WAYMAP_VALID 3983

    // L15_CPX_GEN_INVALIDATION_IF_TAGCHECK_MES_AND_WAYMAP_VALID 3983
    instr.SetUpdate( l15_transducer_val                  , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , b1                                          , Ite(wmt_compare_match_s3 , b1                                                           , b0) ) , default_l15_transducer_val ));
    instr.SetUpdate( l15_transducer_returntype           , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , CPX_RESTYPE_STORE_ACK                       , CPX_RESTYPE_INVAL )      , default_l15_transducer_returntype           ));
    instr.SetUpdate( l15_transducer_noncacheable         , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , default_l15_transducer_noncacheable         , b0                )      , default_l15_transducer_noncacheable         ));
    instr.SetUpdate( l15_transducer_atomic               , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , default_l15_transducer_atomic               , b0                )      , default_l15_transducer_atomic               ));
    instr.SetUpdate( l15_transducer_data_0               , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , default_l15_transducer_data_0               , zero_data         )      , default_l15_transducer_data_0               ));
    instr.SetUpdate( l15_transducer_data_1               , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , default_l15_transducer_data_1               , zero_data         )      , default_l15_transducer_data_1               ));
    instr.SetUpdate( l15_transducer_data_2               , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , default_l15_transducer_data_2               , zero_data         )      , default_l15_transducer_data_2               ));
    instr.SetUpdate( l15_transducer_data_3               , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , default_l15_transducer_data_3               , zero_data         )      , default_l15_transducer_data_3               ));
    instr.SetUpdate( l15_transducer_inval_address_15_4   , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , default_l15_transducer_inval_address_15_4   , b1                )      , default_l15_transducer_inval_address_15_4   ));
    instr.SetUpdate( l15_transducer_inval_icache_all_way , Ite( hit | predecode_int_vec_dis_s1 , Ite( predecode_int_vec_dis_s1 , default_l15_transducer_inval_icache_all_way , b0                )      , default_l15_transducer_inval_icache_all_way ));
    instr.SetUpdate( l15_transducer_inval_dcache_inval   , Ite( hit | predecode_int_vec_dis_s1 , Ite(predecode_int_vec_dis_s1  , default_l15_transducer_inval_dcache_inval   , b1 )                     , default_l15_transducer_inval_dcache_inval )); 

// if no interrupt L15_NOC3_GEN_WRITEBACK_IF_TAGCHECK_M_FROM_DCACHE 4428
    instr.SetUpdate( noc3_val           , Ite( mod & ! predecode_int_vec_dis_s1 , b1                         , default_noc3_val           ));
    instr.SetUpdate( noc3_type          , Ite( mod & ! predecode_int_vec_dis_s1 , L15_NOC3_REQTYPE_WRITEBACK , default_noc3_type          ));
    instr.SetUpdate( noc3_data_0        , Ite( mod & ! predecode_int_vec_dis_s1 , default_noc3_data_0        , default_noc3_data_0        ));
    instr.SetUpdate( noc3_data_1        , Ite( mod & ! predecode_int_vec_dis_s1 , default_noc3_data_1        , default_noc3_data_1        ));
    instr.SetUpdate( noc3_mshrid        , Ite( mod & ! predecode_int_vec_dis_s1 , default_noc3_mshrid        , default_noc3_mshrid        ));
    instr.SetUpdate( noc3_threadid      , Ite( mod & ! predecode_int_vec_dis_s1 , default_noc3_threadid      , default_noc3_threadid      ));
    instr.SetUpdate( noc3_address       , Ite( mod & ! predecode_int_vec_dis_s1 , default_noc3_address       , default_noc3_address       ));
    instr.SetUpdate( noc3_invalidate    , Ite( mod & ! predecode_int_vec_dis_s1 , default_noc3_invalidate    , default_noc3_invalidate    ));
    instr.SetUpdate( noc3_with_data     , Ite( mod & ! predecode_int_vec_dis_s1 , b1                         , default_noc3_with_data     ));
    instr.SetUpdate( noc3_fwdack_vector , Ite( mod & ! predecode_int_vec_dis_s1 , default_noc3_fwdack_vector , default_noc3_fwdack_vector ));

    // L15_S3_MESI_INVALIDATE_TAGCHECK_WAY_IF_MES
    MapUpdate(inst, "address_to_mesi_map", address, hit & ! predecode_int_vec_dis_s1 , BvConst(MESI_INVALID,2) ); // unknown ??
    
    // no mshr
    // MapUpdate(instr , "address_to_mshr_data_map" , address , , ); 

    // no update to data
    // MapUpdate(instr , "address_to_data_map"      , address , , ); 
    // TODO: invalidation to L1
  }
  {
    auto instr = model.NewInstr("AMOs");

    // 704
    instr.SetDecode( ( rqtype == PCX_REQTYPE_AMO ) & (amo_op != L15_AMO_OP_NONE) & (fetch_state == L15_FETCH_STATE_NORMAL) );

    instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_PCX_WRITEBACK_DONE);

    auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map

    auto mod = MESI_state == MESI_MODIFIED;
    auto hit = MESI_state != MESI_INVALID;
    // 1393
    // L15_NOC1_GEN_WRITEBACK_GUARD_IF_TAGCHECK_M 4347
    instr.SetUpdate( noc1_val          , Ite( mod , b1                               , default_noc1_val          ));
    instr.SetUpdate( noc1_address      , Ite( mod , address                          , default_noc1_address      ));
    instr.SetUpdate( noc1_noncacheable , Ite( mod , b1                               , default_noc1_noncacheable ));
    instr.SetUpdate( noc1_size         , Ite( mod , size                             , default_noc1_size         ));
    instr.SetUpdate( noc1_threadid     , Ite( mod , threadid                         , default_noc1_threadid     ));
    instr.SetUpdate( noc1_mshrid       , Ite( mod , L15_MSHR_ID_ST                   , default_noc1_mshrid       ));
    instr.SetUpdate( noc1_type         , Ite( mod , L15_NOC1_REQTYPE_WRITEBACK_GUARD , default_noc1_type         ));
    instr.SetUpdate( noc1_data_0       , Ite( mod , zero_data                        , default_noc1_data_0       ));
    instr.SetUpdate( noc1_data_1       , Ite( mod , zero_data                        , default_noc1_data_1       ));

    // L15_CPX_GEN_INVALIDATION_IF_TAGCHECK_MES_AND_WAYMAP_VALID 3983
    // has the issue of L15_CPX_GEN_INTERRUPT
    instr.SetUpdate( l15_transducer_val                  , Ite( hit , Ite(wmt_compare_match_s3                  , b1                                              , b0) , default_l15_transducer_val                  ));
    instr.SetUpdate( l15_transducer_returntype           , Ite( hit , CPX_RESTYPE_INVAL                         , default_l15_transducer_returntype           ));
    instr.SetUpdate( l15_transducer_noncacheable         , Ite( hit , default_l15_transducer_noncacheable       , default_l15_transducer_noncacheable         ));
    instr.SetUpdate( l15_transducer_atomic               , Ite( hit , b0                                        , default_l15_transducer_atomic               ));
    instr.SetUpdate( l15_transducer_data_0               , Ite( hit , zero_data                                 , default_l15_transducer_data_0               ));
    instr.SetUpdate( l15_transducer_data_1               , Ite( hit , zero_data                                 , default_l15_transducer_data_1               ));
    instr.SetUpdate( l15_transducer_data_2               , Ite( hit , zero_data                                 , default_l15_transducer_data_2               ));
    instr.SetUpdate( l15_transducer_data_3               , Ite( hit , zero_data                                 , default_l15_transducer_data_3               ));
    instr.SetUpdate( l15_transducer_inval_address_15_4   , Ite( hit , default_l15_transducer_inval_address_15_4 , default_l15_transducer_inval_address_15_4   ));
    instr.SetUpdate( l15_transducer_inval_icache_all_way , Ite( hit , b1                                        , default_l15_transducer_inval_icache_all_way ));
    instr.SetUpdate( l15_transducer_inval_dcache_inval   , Ite( hit , b1                                        , default_l15_transducer_inval_dcache_inval   ));

    // L15_NOC3_GEN_WRITEBACK_IF_TAGCHECK_M_FROM_DCACHE
    instr.SetUpdate( noc3_val           , Ite( mod , b1                         , default_noc3_val           ));
    instr.SetUpdate( noc3_type          , Ite( mod , L15_NOC3_REQTYPE_WRITEBACK , default_noc3_type          ));
    instr.SetUpdate( noc3_data_0        , Ite( mod , default_noc3_data_0        , default_noc3_data_0        ));
    instr.SetUpdate( noc3_data_1        , Ite( mod , default_noc3_data_1        , default_noc3_data_1        ));
    instr.SetUpdate( noc3_mshrid        , Ite( mod , default_noc3_mshrid        , default_noc3_mshrid        ));
    instr.SetUpdate( noc3_threadid      , Ite( mod , default_noc3_threadid      , default_noc3_threadid      ));
    instr.SetUpdate( noc3_address       , Ite( mod , default_noc3_address       , default_noc3_address       ));
    instr.SetUpdate( noc3_invalidate    , Ite( mod , default_noc3_invalidate    , default_noc3_invalidate    ));
    instr.SetUpdate( noc3_with_data     , Ite( mod , b1                         , default_noc3_with_data     ));
    instr.SetUpdate( noc3_fwdack_vector , Ite( mod , default_noc3_fwdack_vector , default_noc3_fwdack_vector ));

    // L15_S3_MESI_INVALIDATE_TAGCHECK_WAY_IF_MES
    MapUpdate(inst, "address_to_mesi_map", address, hit , BvConst(MESI_INVALID,2) ); // unknown ??

    // TODO: invalidation to L1
  }

  {
    auto instr = model.NewInstr("IFILL");

    // 663
    instr.SetDecode( ( rqtype == PCX_REQTYPE_IFILL ) & ( invalidate == 0 )  & (fetch_state == L15_FETCH_STATE_NORMAL)  );

    // NOC1 :  L15_NOC1_GEN_INSTRUCTION_LD_REQUEST 4241
    instr.SetUpdate( noc1_val          , b1                             ) ;
    instr.SetUpdate( noc1_address      , address                        ) ;
    instr.SetUpdate( noc1_noncacheable , default_noc1_noncacheable      ) ;
    instr.SetUpdate( noc1_size         , default_noc1_size              ) ;
    instr.SetUpdate( noc1_threadid     , default_noc1_threadid          ) ;
    instr.SetUpdate( noc1_mshrid       , L15_MSHR_ID_IFILL              ) ;
    instr.SetUpdate( noc1_type         , L15_NOC1_REQTYPE_IFILL_REQUEST ) ;
    instr.SetUpdate( noc1_data_0       , default_noc1_data_0            ) ;
    instr.SetUpdate( noc1_data_1       , default_noc1_data_1            ) ;

    // L15_S1_MSHR_OP_ALLOCATE
    MapUpdate(instr , "address_to_mshr_map"      , address , b1 ); 
    // 1230
  }
  {
    auto instr = model.NewInstr("ICACHE_INVALIDATE");

    // 665
    instr.SetDecode( ( rqtype == PCX_REQTYPE_IFILL ) & ( invalidate == 1 )  & (fetch_state == L15_FETCH_STATE_NORMAL)  );
    // cpx L15_CPX_GEN_ICACHE_INVALIDATION : 3970

    // has the issue of L15_CPX_GEN_INTERRUPT
    instr.SetUpdate( l15_transducer_val                  , b1                                          );
    instr.SetUpdate( l15_transducer_returntype           , CPX_RESTYPE_INVAL                           );
    instr.SetUpdate( l15_transducer_noncacheable         , default_l15_transducer_noncacheable         );
    instr.SetUpdate( l15_transducer_atomic               , default_l15_transducer_atomic               );
    instr.SetUpdate( l15_transducer_data_0               , default_l15_transducer_data_0               );
    instr.SetUpdate( l15_transducer_data_1               , default_l15_transducer_data_1               );
    instr.SetUpdate( l15_transducer_data_2               , default_l15_transducer_data_2               );
    instr.SetUpdate( l15_transducer_data_3               , default_l15_transducer_data_3               );
    instr.SetUpdate( l15_transducer_inval_address_15_4   , default_l15_transducer_inval_address_15_4   );
    instr.SetUpdate( l15_transducer_inval_icache_all_way , default_l15_transducer_inval_icache_all_way );
    instr.SetUpdate( l15_transducer_inval_dcache_inval   , default_l15_transducer_inval_dcache_inval   );
  }

  {
    auto instr = model.NewInstr("DCACHE_INVALIDATE");

    // 657
    instr.SetDecode( ( rqtype == PCX_REQTYPE_LOAD ) & ( invalidate == 1 ) & ( nc == 0 ) & (fetch_state == L15_FETCH_STATE_NORMAL)  );
    // L15_REQTYPE_DCACHE_SELF_INVALIDATION
    // 1491
    // cpx : L15_CPX_GEN_DCACHE_INVALIDATION 3976
    instr.SetUpdate( l15_transducer_val                  , b1                                          );
    instr.SetUpdate( l15_transducer_returntype           , CPX_RESTYPE_INVAL                           );
    instr.SetUpdate( l15_transducer_noncacheable         , default_l15_transducer_noncacheable         );
    instr.SetUpdate( l15_transducer_atomic               , default_l15_transducer_atomic               );
    instr.SetUpdate( l15_transducer_data_0               , default_l15_transducer_data_0               );
    instr.SetUpdate( l15_transducer_data_1               , default_l15_transducer_data_1               );
    instr.SetUpdate( l15_transducer_data_2               , default_l15_transducer_data_2               );
    instr.SetUpdate( l15_transducer_data_3               , default_l15_transducer_data_3               );
    instr.SetUpdate( l15_transducer_inval_address_15_4   , default_l15_transducer_inval_address_15_4   );
    instr.SetUpdate( l15_transducer_inval_icache_all_way , default_l15_transducer_inval_icache_all_way );
    instr.SetUpdate( l15_transducer_inval_dcache_inval   , b1   );

  }

  {
    auto instr = model.NewInstr("INTERRUPT");

    // 780
    instr.SetDecode( (rqtype == PCX_REQTYPE_INTERRUPT)  & (fetch_state == L15_FETCH_STATE_NORMAL) & (nc == 0) );
    /*
                    predecode_reqtype_s1 = `L15_REQTYPE_PCX_INTERRUPT;
                    predecode_interrupt_broadcast_s1 = predecode_non_cacheable_s1;
    */

    // noc1 : L15_NOC1_GEN_INTERRUPT_FWD 4347

    instr.SetUpdate( noc1_val          , b1                             ) ;
    instr.SetUpdate( noc1_address      , default_noc1_address           ) ;
    instr.SetUpdate( noc1_noncacheable , default_noc1_noncacheable      ) ;
    instr.SetUpdate( noc1_size         , default_noc1_size              ) ;
    instr.SetUpdate( noc1_threadid     , default_noc1_threadid          ) ;
    instr.SetUpdate( noc1_mshrid       , default_noc1_mshrid            ) ;
    instr.SetUpdate( noc1_type         , L15_NOC1_REQTYPE_INTERRUPT_FWD ) ;
    instr.SetUpdate( noc1_data_0       , data                           ) ;
    instr.SetUpdate( noc1_data_1       , default_noc1_data_1            ) ;

  }

  {
    auto instr = model.NewInstr("INTERRUPT_BROADCAST");

    // 780
    instr.SetDecode( (rqtype == PCX_REQTYPE_INTERRUPT)  & (fetch_state == L15_FETCH_STATE_NORMAL) & (nc == 1) );
    /*
                    predecode_reqtype_s1 = `L15_REQTYPE_PCX_INTERRUPT;
                    predecode_interrupt_broadcast_s1 = predecode_non_cacheable_s1;
    */
    // 1722
    // cpx : L15_CPX_GEN_BROADCAST_ACK

    // has the issue of L15_CPX_GEN_INTERRUPT
    instr.SetUpdate( l15_transducer_val                  , b1                                          );
    instr.SetUpdate( l15_transducer_returntype           , CPX_RESTYPE_INTERRUPT                       );
    instr.SetUpdate( l15_transducer_noncacheable         , default_l15_transducer_noncacheable         );
    instr.SetUpdate( l15_transducer_atomic               , default_l15_transducer_atomic               );
    instr.SetUpdate( l15_transducer_data_0               , default_l15_transducer_data_0               );
    instr.SetUpdate( l15_transducer_data_1               , default_l15_transducer_data_1               );
    instr.SetUpdate( l15_transducer_data_2               , default_l15_transducer_data_2               );
    instr.SetUpdate( l15_transducer_data_3               , default_l15_transducer_data_3               );
    instr.SetUpdate( l15_transducer_inval_address_15_4   , default_l15_transducer_inval_address_15_4   );
    instr.SetUpdate( l15_transducer_inval_icache_all_way , default_l15_transducer_inval_icache_all_way );
    instr.SetUpdate( l15_transducer_inval_dcache_inval   , default_l15_transducer_inval_dcache_inval   );

  }

  // DIAG
  // L15_REQTYPE_LOAD_CONFIG_REG, L15_REQTYPE_DIAG_LOAD, L15_REQTYPE_HMC_DIAG_LOAD, L15_REQTYPE_LOAD_PREFETCH
  // L15_REQTYPE_WRITE_CONFIG_REG, L15_REQTYPE_DIAG_STORE, L15_REQTYPE_LINE_FLUSH, L15_REQTYPE_HMC_DIAG_STORE, L15_REQTYPE_HMC_FLUSH
  //

  // does not handle L15_REQTYPE_LOAD_PREFETCH at all


  { // child instruction for after write-back
    pcx_writeback = model.NewChild(fetch_state_s1 == L15_FETCH_STATE_PCX_WRITEBACK_DONE);
    { // the load non-cacheable
      auto instr = pcx_writeback.NewInstr( "LOAD_nc_writeback_done" );

      instr.SetDecode( ( rqtype == L15_REQTYPE_LOAD) & (nc == 1) );
      // 1162 

      // L15_NOC1_GEN_DATA_LD_REQUEST 4224
      instr.SetUpdate( noc1_val          , b1                          );
      instr.SetUpdate( noc1_address      , address                     );
      instr.SetUpdate( noc1_noncacheable , default_noc1_noncacheable   );
      instr.SetUpdate( noc1_size         , default_noc1_size           );
      instr.SetUpdate( noc1_threadid     , default_noc1_threadid       );
      instr.SetUpdate( noc1_mshrid       , L15_MSHR_ID_LD              );
      instr.SetUpdate( noc1_type         , L15_NOC1_REQTYPE_LD_REQUEST );
      instr.SetUpdate( noc1_data_0       , default_noc1_data_0         );
      instr.SetUpdate( noc1_data_1       , default_noc1_data_1         );

      // L15_S1_MSHR_OP_ALLOCATE
      MapUpdate(instr , "address_to_mshr_map", address, b1);

      instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_NORMAL);
    }
    { // the store non-cacheable
      auto instr = pcx_writeback.NewInstr( "STORE_nc_writeback_done" );

      instr.SetDecode( ( rqtype == L15_REQTYPE_STORE ) & (nc == 1) );

      // 1286

      // L15_NOC1_GEN_DATA_WRITETHROUGH_REQUEST_FROM_PCX : 4248
      instr.SetUpdate( noc1_val          , b1                                    );
      instr.SetUpdate( noc1_address      , address                               );
      instr.SetUpdate( noc1_noncacheable , nc                                    );
      instr.SetUpdate( noc1_size         , size                                  );
      instr.SetUpdate( noc1_threadid     , threadid                              );
      instr.SetUpdate( noc1_mshrid       , L15_MSHR_ID_ST                        );
      instr.SetUpdate( noc1_type         , L15_NOC1_REQTYPE_WRITETHROUGH_REQUEST );
      instr.SetUpdate( noc1_data_0       , data                                  );
      instr.SetUpdate( noc1_data_1       , default_noc1_data_1                   );

      // L15_S3_MSHR_OP_UPDATE_ST_MSHR_WAIT_ACK 3930   don't care

      // L15_S1_MSHR_OP_ALLOCATE
      MapUpdate(instr , "address_to_mshr_map", address, b1);

      instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_NORMAL);
    }
    { // the load non-cacheable
      auto instr = pcx_writeback.NewInstr( "AMOs_pcx_writeback_done" );

      instr.SetDecode( rqtype == PCX_REQTYPE_AMO ); // 1450

      instr.SetUpdate(l15_noc1buffer_req_address,      address,     );
      instr.SetUpdate(l15_noc1buffer_req_noncacheable, BvConst(1,1) );
      instr.SetUpdate(l15_noc1buffer_req_size,         size ,       );
      
      /*
                decoder_mshr_allocation_type_s1 = `L15_MSHR_ID_LD;
      */

      auto atm_noc1_req_type = 
        Ite( amo_op == L15_AMO_OP_SWAP , L15_NOC1_GEN_DATA_SWAP_REQUEST_FROM_PCX    ,
        Ite( amo_op == L15_AMO_OP_ADD  , L15_NOC1_REQTYPE_AMO_ADD_REQUEST           ,
        Ite( amo_op == L15_AMO_OP_AND  , L15_NOC1_REQTYPE_AMO_AND_REQUEST           ,
        Ite( amo_op == L15_AMO_OP_OR   , L15_NOC1_REQTYPE_AMO_OR_REQUEST            ,
        Ite( amo_op == L15_AMO_OP_XOR  , L15_NOC1_REQTYPE_AMO_XOR_REQUEST           ,
        Ite( amo_op == L15_AMO_OP_MAX  , L15_NOC1_REQTYPE_AMO_MAX_REQUEST           ,
        Ite( amo_op == L15_AMO_OP_MAXU , L15_NOC1_REQTYPE_AMO_MAXU_REQUEST          ,
        Ite( amo_op == L15_AMO_OP_MIN  , L15_NOC1_REQTYPE_AMO_MIN_REQUEST           ,
        Ite( amo_op == L15_AMO_OP_MINU , L15_NOC1_REQTYPE_AMO_MINU_REQUEST          ,
        Ite( amo_op == L15_AMO_OP_CAS1 , L15_NOC1_REQTYPE_CAS_REQUEST               ,
          default_noc1_type ))))))))));
      auto atm_noc1_data_1 = 
        Ite( amo_op == L15_AMO_OP_SWAP , data_next , default_noc1_data_1);

      instr.SetUpdate( noc1_val          , b1                        );
      instr.SetUpdate( noc1_address      , address                   );
      instr.SetUpdate( noc1_noncacheable , default_noc1_noncacheable );
      instr.SetUpdate( noc1_size         , default_noc1_size         );
      instr.SetUpdate( noc1_threadid     , default_noc1_threadid     );
      instr.SetUpdate( noc1_mshrid       , L15_MSHR_ID_LD            );
      instr.SetUpdate( noc1_type         , atm_noc1_req_type         );
      instr.SetUpdate( noc1_data_0       , data                      );
      instr.SetUpdate( noc1_data_1       , atm_noc1_data_1           );

      instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_NORMAL);

      // L15_S1_MSHR_OP_ALLOCATE
      MapUpdate(instr , "address_to_mshr_map", address, b1);
    }
  }
}
