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
      
PMESH_L15_NOC2_ILA::PMESH_L15_NOC2_ILA()
    : // construct the model
      PMESH_L15_ILA("PMESH_L15_NOC2_ILA"),
      // I/O interface: this is where the commands come from.

      noc2_icache_type    (model.NewBvInput("noc2decoder_l15_icache_type", 1)),  // 2 trans/4 trans in fetch_state?
      noc2_mshrid         (model.NewBvInput("noc2decoder_l15_mshrid"     , 2)),
      noc2_threadid       (model.NewBvInput("noc2decoder_l15_threadid"   , 1)),
      noc2_val            (model.NewBvInput("noc2decoder_l15_val"        , 1)),
      noc2_reqtype        (model.NewBvInput("noc2decoder_l15_reqtype"    , 8)),
      noc2_mesi_ack_state (model.NewBvInput("noc2decoder_l15_ack_state"  , 2)), // could be used to write to mesi (if fill ?)
      noc2_fwdack_vector  (model.NewBvInput("noc2decoder_l15_fwd_subcacheline_vector", 4)), // use for testing last child, and fwd to cpx
      noc2_address        (model.NewBvInput("noc2decoder_l15_address"    , 40)), 
      noc2_data_0         (model.NewBvInput("noc2decoder_l15_data_0"     , 64)),
      noc2_data_1         (model.NewBvInput("noc2decoder_l15_data_1"     , 64)),
      noc2_data_2         (model.NewBvInput("noc2decoder_l15_data_2"     , 64)),
      noc2_data_3         (model.NewBvInput("noc2decoder_l15_data_3"     , 64)),
      
      // Output states: l1.5 --> noc1 requests
      // not specifying these updates:
      // l15_noc1buffer_req_csm_data   
      // l15_noc1buffer_req_csm_ticket 
      // l15_noc1buffer_req_homeid
      // l15_noc1buffer_req_prefetch   

      noc1_address      ( model.NewBvState("l15_noc1buffer_req_address"      , 40) ),
      noc1_noncacheable ( model.NewBvState("l15_noc1buffer_req_noncacheable" , 1) ),
      noc1_size         ( model.NewBvState("l15_noc1buffer_req_size"         , 3) ),
      noc1_threadid     ( model.NewBvState("l15_noc1buffer_req_threadid"     , 1) ), // not
      noc1_mshrid       ( model.NewBvState("l15_noc1buffer_req_mshrid"       , 2) ),
      noc1_type         ( model.NewBvState("l15_noc1buffer_req_type"         , 5) ),
      noc1_address      ( model.NewBvState("l15_noc1buffer_req_address"      , 40) ),
      noc1_non_cacheable( model.NewBvState("l15_noc1buffer_req_non_cacheable", 1) ),
      noc1_data_0       ( model.NewBvState("l15_noc1buffer_req_data_0"       , 64)),
      noc1_data_1       ( model.NewBvState("l15_noc1buffer_req_data_1"       , 64)),

      // l1.5 --> noc3 requests
      noc3_val          ( model.NewBvState("l15_noc3encoder_req_val"        , 1)),
      noc3_type         ( model.NewBvState("l15_noc3encoder_req_type"       , 3)),
      noc3_data_0       ( model.NewBvState("l15_noc3encoder_req_data_0"     , 64)),
      noc3_data_1       ( model.NewBvState("l15_noc3encoder_req_data_1"     , 64)),
      noc3_mshrid       ( model.NewBvState("l15_noc3encoder_req_mshrid"     , 2)),
      noc3_threadid     ( model.NewBvState("l15_noc3encoder_req_threadid"   , 1)),
      noc3_address      ( model.NewBvState("l15_noc3encoder_req_address"    , 40)),
      noc3_invalidate   ( model.NewBvState("l15_noc3encoder_req_was_inval"  , 1)),
      noc3_with_data    ( model.NewBvState("l15_noc3encoder_req_with_data"  , 1)),     // depends on M?
      noc3_fwdack_vector( model.NewBvState("l15_noc3encoder_req_fwdack_vector", 4)), // seems to be a direct forwarding

      // Output states: l1.5 --> core
      l15_transducer_val             ( model.NewBvState("l15_transducer_val"              , 1)  ),
      l15_transducer_returntype      ( model.NewBvState("l15_transducer_returntype"       , 4)  ), // 0 if hit, l15_cpxencoder_returntype
      l15_transducer_noncacheable    ( model.NewBvState("l15_transducer_noncacheable"     , 1)  ),
      l15_transducer_atomic          ( model.NewBvState("l15_transducer_atomic"           , 1)  ),
      l15_transducer_data_0          ( model.NewBvState("l15_transducer_data_0"           , 64) ),
      l15_transducer_data_1          ( model.NewBvState("l15_transducer_data_1"           , 64) ),
      l15_transducer_data_2          ( model.NewBvState("l15_transducer_data_2"           , 64) ),
      l15_transducer_data_3          ( model.NewBvState("l15_transducer_data_3"           , 64) ),

      // We made the map as a mem (although in the design, it does not need to be so large)
      mesi_state( NewMap( "address_to_mesi_map", 40, 2 ) ),
      data_state( NewMap( "address_to_data_map", 40, 64) ),
      missed_on_this( NewMap("address_to_mshr_map", 40, 1) ),
      fetch_state_s1( model.NewBvState("fetch_state_s1", 3) ),
      
      // -----------------------------------------------------------------------------------------------


    {

  // ------------------------------ CONSTANTS ---------------------------------- //

  auto PCX_REQTYPE_LOAD  = BvConst(0,5);
  auto PCX_REQTYPE_STORE = BvConst(1,5);
  auto PCX_REQTYPE_IFILL = BvConst(0x10, 5);
  auto PCX_REQTYPE_AMO   = BvConst(6,5);
  auto PCX_REQTYPE_INTERRUPT = BvConst(5,5);

  auto L15_FETCH_STATE_NORMAL = BvConst(0, 3);
  auto L15_FETCH_STATE_PCX_WRITEBACK_DONE = BvConst(1, 3);
  auto L15_NOC1_REQTYPE_WRITEBACK_GUARD = BvConst(1,5);
  auto L15_NOC1_REQTYPE_LD_REQUEST = BvConst(2,5);
  auto L15_NOC1_REQTYPE_IFILL_REQUEST = BvConst(3,5);
  auto L15_NOC1_REQTYPE_WRITETHROUGH_REQUEST = BvConst(4,5);
  auto L15_NOC1_REQTYPE_ST_UPGRADE_REQUEST = BvConst(5,5);
  auto L15_NOC1_REQTYPE_ST_FILL_REQUEST = BvConst(6,5);
  auto L15_NOC1_REQTYPE_CAS_REQUEST = BvConst(7,5);
  auto L15_NOC1_REQTYPE_SWAP_REQUEST = BvConst(8,5);
  auto L15_NOC1_REQTYPE_INTERRUPT_FWD = BvConst(9,5);
  auto L15_NOC1_REQTYPE_AMO_ADD_REQUEST = BvConst( 10,5);
  auto L15_NOC1_REQTYPE_AMO_AND_REQUEST = BvConst( 11, 5);
  auto L15_NOC1_REQTYPE_AMO_OR_REQUEST = BvConst(  12, 5);
  auto L15_NOC1_REQTYPE_AMO_XOR_REQUEST = BvConst( 13, 5);
  auto L15_NOC1_REQTYPE_AMO_MAX_REQUEST = BvConst( 14, 5);
  auto L15_NOC1_REQTYPE_AMO_MAXU_REQUEST = BvConst(15, 5);
  auto L15_NOC1_REQTYPE_AMO_MIN_REQUEST = BvConst( 16, 5);
  auto L15_NOC1_REQTYPE_AMO_MINU_REQUEST = BvConst(17, 5);

  auto CPX_RESTYPE_LOAD = BvConst(0, 4); 
  auto CPX_RESTYPE_IFILL1 = BvConst(1, 4); 
  auto CPX_RESTYPE_IFILL2 = BvConst(1, 4); 
  auto CPX_RESTYPE_STREAM_LOAD = BvConst(2, 4); 
  auto CPX_RESTYPE_INVAL = BvConst(3, 4); 
  auto CPX_RESTYPE_STORE_ACK = BvConst(4, 4); 
  auto CPX_RESTYPE_STREAM_STORE_ACK = BvConst(6, 4); 
  auto CPX_RESTYPE_INTERRUPT = BvConst(7, 4); 
  auto CPX_RESTYPE_FP = BvConst(8, 4); 
  auto CPX_RESTYPE_FWD_REQ = BvConst(10, 4); 
  auto CPX_RESTYPE_FWD_REPLY = BvConst(11, 4); 
  auto CPX_RESTYPE_ATOMIC_RES = BvConst(13, 4);


  // ------------------------------ INSTRUCTIONS ---------------------------------- //

  // L1.5 fetch function -- what corresponds to instructions on L1.5 PCX interface
  model.SetFetch( lConcat({address, data, nc, rqtype, size, val })   );
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

    instr.SetDecode( ( rqtype == PCX_REQTYPE_LOAD) & (nc == 0) & (invalidate == 0) & () );

    auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map
    auto DATA_cache = Map( "address_to_data_map", 64, address ); // Use the map

    auto hit = MESI_state != MESI_INVALID;

    // on miss : send out noc1 request eventually

    instr.SetUpdate(l15_noc1buffer_req_address,      Ite(! hit, address,       unknown(40)() ) );
    instr.SetUpdate(l15_noc1buffer_req_noncacheable, Ite(! hit, BvConst(0,1) , unknown(1)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_size,         Ite(! hit, size ,         unknown(3)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_type,         Ite(! hit, BvConst(2,5) , unknown(5)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_data_0,       Ite(! hit, BvConst(0,64), unknown(64)() ) );
    instr.SetUpdate(l15_noc1buffer_req_data_1,       Ite(! hit, BvConst(0,64), unknown(64)() ) );

    // on the hit side : return the data on cpx

    instr.SetUpdate( l15_transducer_val,             Ite( hit , BvConst(1,1), unknown(1)() ) );
    instr.SetUpdate( l15_transducer_returntype,      Ite( hit , CPX_RESTYPE_LOAD , unknown(4)() ) );
    instr.SetUpdate( l15_transducer_data_0,          Ite( hit , DATA_cache , unknown(64)()  ) );



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

    instr.SetDecode( ( rqtype == PCX_REQTYPE_STORE) & (nc == 0) );

    auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map
    auto DATA_cache = Map( "address_to_data_map", 64, address ); // Use the map
    auto missed_on_this = Map( "address_to_mshr_map", 1, address ); // Use the map

    auto hit = MESI_state != MESI_INVALID;
    auto writable = (MESI_state == MESI_EXCLUSIVE) | (MESI_state == MESI_MODIFIED);

    // write-back
    // eviction is upon ack-dt

    // Send ReqExRd if necessary

    instr.SetUpdate(l15_noc1buffer_req_address,      Ite(! writable & ! missed_on_this, address,       unknown(40)() ) );
    instr.SetUpdate(l15_noc1buffer_req_noncacheable, Ite(! writable & ! missed_on_this, BvConst(0,1) , unknown(1)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_size,         Ite(! writable & ! missed_on_this, size ,         unknown(3)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_type, 
      Ite(! writable & ! missed_on_this, 
        Ite(MESI_state == MESI_SHARED, L15_NOC1_REQTYPE_ST_UPGRADE_REQUEST, L15_NOC1_REQTYPE_ST_FILL_REQUEST) , unknown(5)() ) );
  
    // update MESI state
    // update DATA state

    MapUpdate(instr, "address_to_mesi_map", address, writable , BvConst(MESI_MODIFIED,2) ) ; // unknown ?? (the write enable signal)
    MapUpdate(instr, "address_to_data_map", address, writable , data  ); // unknown ??
    MapUpdate(instr, "address_to_mshr_map", address, ! writable & ! missed_on_this, BvConst(1,1) );
    // do we need to send-back response
    // there is an ack
  }

  // assumption: you cannot have load upon load miss on the same thread
  { // non cache

    auto instr = model.NewInstr("LOAD_nc");

    instr.SetDecode( ( rqtype == PCX_REQTYPE_LOAD) & (nc == 1) );

    auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map

    auto mod = MESI_state == MESI_MODIFIED;
    auto hit = MESI_state != MESI_INVALID;

    instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_PCX_WRITEBACK_DONE);

    instr.SetUpdate(l15_noc1buffer_req_address,      Ite( mod, address,       unknown(40)() ) );
    instr.SetUpdate(l15_noc1buffer_req_noncacheable, unknown(1)() ); // Ite( mod, nc , unknown(1)() )
    instr.SetUpdate(l15_noc1buffer_req_size,         Ite( mod, size ,         unknown(3)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_type,         Ite( mod, L15_NOC1_REQTYPE_WRITEBACK_GUARD , unknown(5)()  ) );

    MapUpdate(inst, "address_to_mesi_map", address, hit , BvConst(MESI_INVALID,2) ); // unknown ??
    
    // TODO: invalidation to L1
  }

  { // non cache

    auto instr = model.NewInstr("STORE_nc");

    instr.SetDecode( ( rqtype == PCX_REQTYPE_STORE ) & (nc == 1) );

    auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map

    auto mod = MESI_state == MESI_MODIFIED;

    instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_PCX_WRITEBACK_DONE);

    instr.SetUpdate(l15_noc1buffer_req_address,      Ite( mod, address,       unknown(40)() ) );
    instr.SetUpdate(l15_noc1buffer_req_noncacheable, unknown(1)() ); // Ite( mod, nc , unknown(1)() )
    instr.SetUpdate(l15_noc1buffer_req_size,         Ite( mod, size ,         unknown(3)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_type,         Ite( mod, L15_NOC1_REQTYPE_WRITEBACK_GUARD , unknown(5)()  ) );
    
    MapUpdate(inst, "address_to_mesi_map", address, hit , BvConst(MESI_INVALID,2) ); // unknown ??
    
    // TODO: invalidation to L1
  }
  {
    auto instr = model.NewInstr("AMOs");

    instr.SetDecode( rqtype == PCX_REQTYPE_AMO );

    instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_PCX_WRITEBACK_DONE);

    instr.SetUpdate(l15_noc1buffer_req_address,      Ite( mod, address,       unknown(40)() ) );
    instr.SetUpdate(l15_noc1buffer_req_noncacheable, unknown(1)() ); // Ite( mod, nc , unknown(1)() )
    instr.SetUpdate(l15_noc1buffer_req_size,         Ite( mod, size ,         unknown(3)()  ) );
    instr.SetUpdate(l15_noc1buffer_req_type,         Ite( mod, L15_NOC1_REQTYPE_WRITEBACK_GUARD , unknown(5)()  ) );
    
    MapUpdate(inst, "address_to_mesi_map", address, hit , BvConst(MESI_INVALID,2) ); // unknown ??

    // TODO: invalidation to L1
  }

  {
    auto instr = model.NewInstr("IFILL");

    instr.SetDecode( ( rqtype == PCX_REQTYPE_IFILL ) & ( invalidate == 0 ) );
  }
  {
    auto instr = model.NewInstr("ICACHE_INVALIDATE");

    instr.SetDecode( ( rqtype == PCX_REQTYPE_IFILL ) & ( invalidate == 1 ) );
  }
  {
    auto instr = model.NewInstr("DCACHE_INVALIDATE");

    instr.SetDecode( (rqtype == PCX_REQTYPE_LOAD) & ( invalidate == 1) );
  }
  {
    auto instr = model.NewInstr("INTERRUPT");

    instr.SetDecode( (rqtype == PCX_REQTYPE_INTERRUPT) );
  }


  // DIAG
  // L15_REQTYPE_LOAD_CONFIG_REG, L15_REQTYPE_DIAG_LOAD, L15_REQTYPE_HMC_DIAG_LOAD, L15_REQTYPE_LOAD_PREFETCH
  // L15_REQTYPE_WRITE_CONFIG_REG, L15_REQTYPE_DIAG_STORE, L15_REQTYPE_LINE_FLUSH, L15_REQTYPE_HMC_DIAG_STORE, L15_REQTYPE_HMC_FLUSH
  //

  // does not handle L15_REQTYPE_LOAD_PREFETCH at all


  { // child instruction for after write-back
    pcx_writeback = model.NewChild(fetch_state_s1 == L15_FETCH_STATE_PCX_WRITEBACK_DONE);
    { // the load non-cacheable
      auto instr = pcx_writeback.NewInstr( "LOAD_nc_pcx_writeback_done" );

      instr.SetDecode( ( rqtype == 0) & (nc == 1) );

      // auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map
      // auto DATA_cache = Map( "address_to_data_map", 64, address ); // Use the map

      // auto hit = MESI_state != MESI_INVALID;
      // on miss : send out noc1 request eventually
      instr.SetUpdate(l15_noc1buffer_req_address,      address,     );
      instr.SetUpdate(l15_noc1buffer_req_noncacheable, BvConst(1,1) );
      instr.SetUpdate(l15_noc1buffer_req_size,         size ,       );
      instr.SetUpdate(l15_noc1buffer_req_type,         BvConst(2,5) );
      instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_NORMAL);
    }
    { // the store non-cacheable
      auto instr = pcx_writeback.NewInstr( "STORE_nc_pcx_writeback_done" );

      instr.SetDecode( ( rqtype == 0) & (nc == 1) );

      // auto MESI_state = Map( "address_to_mesi_map",  2, address ); // Use the map
      // auto DATA_cache = Map( "address_to_data_map", 64, address ); // Use the map

      // auto hit = MESI_state != MESI_INVALID;
      // on miss : send out noc1 request eventually
      instr.SetUpdate(l15_noc1buffer_req_address,      address,     );
      instr.SetUpdate(l15_noc1buffer_req_noncacheable, BvConst(1,1) );
      instr.SetUpdate(l15_noc1buffer_req_size,         size ,       );
      instr.SetUpdate(l15_noc1buffer_req_type,         L15_REQTYPE_WRITETHROUGH );
      instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_NORMAL);
    }
    { // the load non-cacheable
      auto instr = pcx_writeback.NewInstr( "AMOs_pcx_writeback_done" );

      instr.SetDecode( rqtype == PCX_REQTYPE_AMO );

      instr.SetUpdate(l15_noc1buffer_req_address,      address,     );
      instr.SetUpdate(l15_noc1buffer_req_noncacheable, BvConst(1,1) );
      instr.SetUpdate(l15_noc1buffer_req_size,         size ,       );
      
      auto L15_AMO_OP_LR    = BvConst(0x1,4);
      auto L15_AMO_OP_SC    = BvConst(0x2,4);
      auto L15_AMO_OP_SWAP  = BvConst(0x3,4);
      auto L15_AMO_OP_ADD   = BvConst(0x4,4);
      auto L15_AMO_OP_AND   = BvConst(0x5,4);
      auto L15_AMO_OP_OR    = BvConst(0x6,4);
      auto L15_AMO_OP_XOR   = BvConst(0x7,4);
      auto L15_AMO_OP_MAX   = BvConst(0x8,4);
      auto L15_AMO_OP_MAXU  = BvConst(0x9,4);
      auto L15_AMO_OP_MIN   = BvConst(0xa,4);
      auto L15_AMO_OP_MINU  = BvConst(0xb,4);
      auto L15_AMO_OP_CAS1  = BvConst(0xc,4);


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

      auto noc1_req_type = 
        Ite( transducer_l15_amo_op == L15_AMO_OP_SWAP, L15_NOC1_GEN_DATA_SWAP_REQUEST_FROM_PCX, 
        Ite( transducer_l15_amo_op == L15_AMO_OP_LR,   L15_NOC1_GEN_WRITEBACK_GUARD_IF_TAGCHECK_M, 
        Ite( transducer_l15_amo_op == L15_AMO_OP_SC,   L15_NOC1_GEN_WRITEBACK_GUARD_IF_TAGCHECK_M, 
        Ite( transducer_l15_amo_op == L15_AMO_OP_ADD,  L15_NOC1_REQTYPE_AMO_ADD_REQUEST, 
        Ite( transducer_l15_amo_op == L15_AMO_OP_AND,  L15_NOC1_REQTYPE_AMO_AND_REQUEST,
        Ite( transducer_l15_amo_op == L15_AMO_OP_OR,   L15_NOC1_REQTYPE_AMO_OR_REQUEST,
        Ite( transducer_l15_amo_op == L15_AMO_OP_XOR,  L15_NOC1_REQTYPE_AMO_XOR_REQUEST,
        Ite( transducer_l15_amo_op == L15_AMO_OP_MAX,  L15_NOC1_REQTYPE_AMO_MAX_REQUEST,
        Ite( transducer_l15_amo_op == L15_AMO_OP_MAXU, L15_NOC1_REQTYPE_AMO_MAXU_REQUEST,
        Ite( transducer_l15_amo_op == L15_AMO_OP_MIN,  L15_NOC1_REQTYPE_AMO_MIN_REQUEST,
        Ite( transducer_l15_amo_op == L15_AMO_OP_MINU, L15_NOC1_REQTYPE_AMO_MINU_REQUEST,
        Ite( transducer_l15_amo_op == L15_AMO_OP_CAS1, L15_NOC1_REQTYPE_CAS_REQUEST,
          L15_NOC1_GEN_WRITEBACK_GUARD_IF_TAGCHECK_M))))))))))));

      instr.SetUpdate(l15_noc1buffer_req_type,         L15_REQTYPE_WRITETHROUGH );
      instr.SetUpdate(fetch_state_s1, L15_FETCH_STATE_NORMAL);
    }
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
