stopat acc_0.predict_input_done
stopat acc_0.mti_signal

# prelog: ??->14->29
# 29 -> ... 32(next 29)
# 56 -> ... 22->21 (next=56)
# 56 -> ... 23 

# hidden_read (next=56)
# edges_read (56)
# visibleEnergies_write(59,next=22)
# h_cnt : newvalue@56
# v_cnt : newvalue@20
# j : newvalue@(next=56)

assume -name {aux} \
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


assume -name {global-3} \
{
    cycles[0] |-> (
      acc_0.read_rbm_num_visible_ln687_d == {acc_0.num_loops, acc_0.num_users, 
          acc_0.num_hidden, acc_0.num_visible}
           )
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
assume { ( cycles[0] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[14] ) }
assume { ( cycles[1] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[15] ) }
assume { ( cycles[2] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[29] ) }
assume { ( cycles[3] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[30] ) }
assume { ( cycles[4] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[31] ) }
assume { ( cycles[5] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[32] ) }
assume { ( cycles[6] |-> acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[29] ) }


# begin-end-step
assume { ( (nondet_start == 1)   == cycles[2] ) }
assume { ( (GM_nondet_step == 1) == cycles[2] ) }
assume { ( (finish_signal == 1)  == cycles[5] ) }

# you can change them to acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm[]
# but I feel it a little bit ugly


# pre-match
assume { (cycles[1] |-> ( GM.predict_sum == acc_0.mux_sum_ln400_z ) ) }
assume { (cycles[1] |-> ( GM.predict_v_cnt == acc_0.mux_v_ln388_q ) ) }
assume { (cycles[1] |-> ( GM.predict_h_cnt == acc_0.mux_h_ln400_z ) ) }
assume { (cycles[2] |-> ( GM.predict_max == acc_0.mux_predict_vector_ln471_0_q[386:371] ) ) }
assume { (cycles[0] |-> ( GM.jstate == acc_0.mux_j_ln396_z ) ) } 


assume -name {pre-set} -env \
{
    cycles[2] |-> (
        GM.per_v_pc ==  0 &&
        GM.predict_upc == 1 &&
        GM.upc ==  2 
        )
}


assume -name {mem-sel} -env { GM_pd_or_tr == 1 }

# mem-hidden-preserve
assume { ( ( hidden_unit_CE2 & ~finorever ) |-> ( ( load_addr0 == hidden_unit_A2 ) ##1 ( load_data0 == hidden_unit_Q2 ) ) ) }
assume { ( GM_hidden_unit_rec == in_term ) }
assume { ( GM_step && (load_addr0 == GM_hidden_unit_raddr0) ) |-> (load_data0 == GM_hidden_unit_rdata0) }

# mem-edges-preserve
assume { ( ( edges_CE1 & ~ finorever ) |-> ( ( load_addr1 == edges_A1 ) ##1 ( load_data1 == edges_Q1 ) ) ) }
assume { ( GM_edges_rec == in_term ) }
assume { ( GM_step && (load_addr1 == GM_edges_raddr0) ) |-> (load_data1 == GM_edges_rdata0)  }


# visibleEnergies-preserve
assume { (GM_visibleEnergies_rec == in_term) && (GM_visibleEnergies_compare == finish) }

# load_info
assume { ( cycles[3] |->  ( reg0 == acc_0.mux_v_ln388_q ) )   }
assume { ( cycles[3] |->  ( reg1 == acc_0.mux_h_ln400_z ) )   }
assume { ( cycles[5] |->  ( reg2 == acc_0.mux_sum_ln400_z ) ) }
assume { ( cycles[5] |->  ( reg3 == acc_0.mux_predict_vector_ln471_0_q[386:371] ) ) }
# final-check
assert { ( finish |-> ( GM.predict_v_cnt == reg0 ) ) }
assert { ( finish |-> ( GM.predict_h_cnt == reg1 ) ) }
assert { ( finish |-> ( GM.predict_sum == reg2 )   ) }
assert { ( finish |-> ( GM.predict_max == reg3 )   ) }

# internal not shared
assert { ( GM_step_1d |-> (GM.per_v_pc == 0) )}
assert { ( GM_step_1d |-> (GM.predict_upc == 1 )) }
assert { ( GM_step_1d |-> (GM.upc == 2) )  }

# final-mem-check
assert { finish |-> (~GM_visibleEnergies_mismatch) }



#check_assumptions -show
#check_assumptions