stopat acc_0.predict_input_done
stopat acc_0.mti_signal

# prelog: 16->17
# 17 -> ... -> 20 (next 19)
# 17 -> ... -> 21

#  SysC:
#
#     max -= 31;
#  ACTIVATE_VISIBLE_PREDICT_ENERGY_UPDATE:
#     for (j = 0; j < K; j++) // [17]
#     {
#        tmp1 = visibleEnergies[j];
#        wait();  //[19]
#        visibleEnergies[j] = tmp1 - max;
#        wait();  //[20]
#     }
#
#     sumOfpow2 = 0; // this is the numerator
#     //TODO: maximum exp of 300 k.1 added together, k>0
#  ACTIVATE_VISIBLE_PREDICT_SUM:
#     for (j = 0; j < K; j++) // [21] [23]
#
#
# visibleEnergyRead (next=24) (23,26)
# visibleEnergyWrite (written in 26)
# j : (next=24, previous) (newvalue@24)



assume -name {aux} \
{
    cycles[0] |-> (
      acc_0.read_rbm_num_visible_ln687_d == {acc_0.num_loops, acc_0.num_users, 
          acc_0.num_hidden, acc_0.num_visible}
           )
}

assume -name {aux-2} \
{
    acc_0.num_hidden + 1 == {acc_0.add_ln713_1_q, !acc_0.read_rbm_num_hidden_ln688_q[0]}
}

assume -name {aux-3} \
{
    (acc_0.add_ln713_1_q == acc_0.num_movies)
    // && (acc_0.num_movies == 5) 
    // && (acc_0.num_hidden == 10) 
    // && (acc_0.num_loops == 2) 
    // && (acc_0.num_testusers == 2) 
    // && (acc_0.num_users == 2) 
    // && (acc_0.num_visible == 25) 
}


assume -name {nvnm} \
{
    ( (acc_0.num_visible ==  (acc_0.num_movies<<2)+acc_0.num_movies) // 5x relationship
        && (acc_0.num_movies <= 16'h3333)
        && (acc_0.num_visible > 0)
        && (acc_0.num_hidden > 0)
        )
}

assume -name {global} \
{
      (
    GM.num_hidden == acc_0.num_hidden &&  // gloabl
    GM.num_loops == acc_0.num_loops && 
    GM.num_movies == acc_0.num_movies && 
    GM.num_testusers == acc_0.num_testusers && 
    GM.num_users == acc_0.num_users && 
    GM.num_visible == acc_0.num_visible )
}


assume -name {global-2} \
{
    GM.init_done == 1&&
    acc_0.init_done == 1
}


assume -name {onehot-enc} \
{
    $onehot(acc_0.state_rbm_0_cmos32soi_rbm_train_rbm_next) && $onehot(acc_0.state_rbm_0_cmos32soi_rbm_train_rbm) &&
    $onehot(acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm_next) && $onehot(acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm) &&
    $onehot(acc_0.state_rbm_0_cmos32soi_rbm_load_next) && $onehot(acc_0.state_rbm_0_cmos32soi_rbm_load) &&
    $onehot(acc_0.state_rbm_0_cmos32soi_rbm_store_next) && $onehot(acc_0.state_rbm_0_cmos32soi_rbm_store) &&
    $onehot(acc_0.state_rbm_0_cmos32soi_rbm_config_next) && $onehot(acc_0.state_rbm_0_cmos32soi_rbm_config)
}

# sequence
assume { ( cycles[0] |-> 1 ) }
assume { ( cycles[1] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[19] ) }
assume { ( cycles[2] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[20] ) }
assume { ( cycles[3] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[19] ) }
assume { ( cycles[4] |-> ( acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[20] && acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm_next[19] )) }


# begin-end-step
assume { ( (nondet_start == 1)   == cycles[2] ) }
assume { ( (GM_nondet_step == 1) == cycles[3] ) }
assume { ( (finish_signal == 1)  == cycles[4] ) }
assume { ( (last_cycle == 1 )    == cycles[4] ) }

# you can change them to acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[]
# but I feel it a little bit ugly


# pre-match
assume { (cycles[2] |-> ( GM.predict_sum == acc_0.mux_sum_ln400_z ) ) }
assume { (cycles[2] |-> ( GM.predict_v_cnt == acc_0.mux_v_ln388_q ) ) }
assume { (cycles[2] |-> ( GM.predict_h_cnt == acc_0.mux_h_ln400_z ) ) }  
assume { (cycles[2] |-> ( GM.predict_max == {acc_0.mux_rem_ln58_q[10:0], 
      acc_0.add_ln271_q[13:12], acc_0.sub_ln196_0_1_q, !acc_0.memwrite_rbm_visible_unit_ln365_q[46], 
      acc_0.memwrite_rbm_visible_unit_ln365_q[45]} ) ) } 
# when 59 h matched
#assume { (cycles[2] |-> ( GM.predict_max == acc_0.memwrite_rbm_visible_unit_ln365_q[60:45] ) ) }
assume { (cycles[2] |-> ( GM.jstate == acc_0.mux_j_ln423_z ) ) } 
assume { (cycles[0] |-> ( acc_0.mux_j_ln396_z < 5) ) }


assume -name {pre-set} -env \
{
    cycles[1] |-> (
        GM.per_v_pc ==  1 &&
        GM.predict_upc == 1 &&
        GM.upc ==  2 
        )
}


assume -name {mem-sel} -env { GM_pd_or_tr == 0 }

# mem-visibleEnergies-preserve
assume { ( ( visibleEnergies_CE2 & ~finorever ) |-> ( ( load_addr0 == visibleEnergies_A2 ) ##1 ( load_data0 == visibleEnergies_Q2 ) ) ) }
assume { ( GM_visibleEnergies_rec == in_term ) }
assume { ( GM_step && (load_addr0 == GM_visibleEnergies_raddr0) ) |-> (load_data0 == GM_visibleEnergies_rdata0) }

# mem-visibleEnergies-w-preserve
assume { ( (visibleEnergies_WE0 & ~finorever ) |-> ( store_addr2 == visibleEnergies_A0 && store_data2 == visibleEnergies_D0 ) ) }
assume { ( (GM_step) |-> (store_addr3 == GM_visibleEnergies_waddr0 && store_data3 == GM_visibleEnergies_wdata0 )  ) }

# load_info
assume { ( cycles[3] |->  ( reg0 == acc_0.mux_j_ln423_z ) )   }
# final-check
assert { ( finish |-> ( GM.jstate == reg0 ) ) }

# internal not shared
assert { ( last_cycle |-> (GM.per_v_pc == 1) )}
assert { ( last_cycle |-> (GM.predict_upc == 1 )) }
assert { ( last_cycle |-> (GM.upc == 2) )  }
assert { ( last_cycle |-> ( acc_0.mux_j_ln396_z < 5) ) }

# final-mem-check
assert { finish |-> ( store_addr3 == store_addr2 && store_data3 == store_data2 ) }



#check_assumptions -show
#check_assumptions