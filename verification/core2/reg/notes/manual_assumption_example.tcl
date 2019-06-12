# the read pipeline is clear
assume -name reg_rd_assume0 {~(__CYCLE_CNT__ == 0) || ((m1.reg_rd_done_out == 0) && (m1.reg_rd_delay1 == 0) && (m1.reg_rd_delay2 == 0) && (m1.reg_rd_delay3 == 0) && (m1.reg_rd_delay4 == 0))}
# no further read command/instruction issued
assume -name reg_rd_assume1 {~(__CYCLE_CNT__ > 0) || (m1.reg_rd_start == 0)}
# (optional) avoid noise
assume -name reg_rd_assume2 {m1.host_addr_reg == 16'h1020}

# stable state via assuming var map at to consecutive cycle
assume -name variable_map_assume_1_reg {(~ __CYCLE_CNT__ == 0)|| (__m0__)}
assume -name variable_map_assume_2_reg {(~ __CYCLE_CNT__ == 0)|| (__m1__)}
assume -name variable_map_assume_3_reg {(~ __CYCLE_CNT__ == 0)|| (__m2__)}
assume -name variable_map_assume_4_reg {(~ __CYCLE_CNT__ == 0)|| (__m3__)}
assume -name variable_map_assume_5_reg {(~ __CYCLE_CNT__ == 0)|| (__m4__)}
assume -name variable_map_assume_6_reg {(~ __CYCLE_CNT__ == 0)|| (__m5__)}
assume -name variable_map_assume_7_reg {(~ __CYCLE_CNT__ == 0)|| (__m6__)}
assume -name variable_map_assume_8_reg {(~ __CYCLE_CNT__ == 0)|| (__m7__)}
assume -name variable_map_assume_9_reg {(~ __CYCLE_CNT__ == 0 )|| (__ILA_SO_FMAC_RX_PKT_CNT1023_LO == m1.FMAC_RX_PKT_CNT1023_LO[31:0])}
assume -name variable_map_assume_10_reg {(~ __CYCLE_CNT__ == 0)|| (__m8__)}
assume -name variable_map_assume_11_reg {(~ __CYCLE_CNT__ == 0)|| (__ILA_SO_FMAC_RX_PKT_CNT127_LO == m1.FMAC_RX_PKT_CNT127_LO[31:0])}
assume -name variable_map_assume_12_reg {(~ __CYCLE_CNT__ == 0)|| (__m9__)}
assume -name variable_map_assume_13_reg {(~ __CYCLE_CNT__ == 0)|| (__ILA_SO_FMAC_RX_PKT_CNT1518_LO == m1.FMAC_RX_PKT_CNT1518_LO[31:0])}
assume -name variable_map_assume_14_reg {(~ __CYCLE_CNT__ == 0)|| (__m10__)}
assume -name variable_map_assume_15_reg {(~ __CYCLE_CNT__ == 0)|| (__ILA_SO_FMAC_RX_PKT_CNT255_LO == m1.FMAC_RX_PKT_CNT255_LO[31:0])}
assume -name variable_map_assume_16_reg {(~ __CYCLE_CNT__ == 0)|| (__m11__)}
assume -name variable_map_assume_17_reg {(~ __CYCLE_CNT__ == 0)|| (__ILA_SO_FMAC_RX_PKT_CNT511_LO == m1.FMAC_RX_PKT_CNT511_LO[31:0])}
assume -name variable_map_assume_18_reg {(~ __CYCLE_CNT__ == 0)|| (__m12__)}
assume -name variable_map_assume_19_reg {(~ __CYCLE_CNT__ == 0)|| (__ILA_SO_FMAC_RX_PKT_CNT64_LO == m1.FMAC_RX_PKT_CNT64_LO[31:0])}
assume -name variable_map_assume_20_reg {(~ __CYCLE_CNT__ == 0)|| (__m13__)}
assume -name variable_map_assume_21_reg {(~ __CYCLE_CNT__ == 0)|| (__m14__)}
assume -name variable_map_assume_22_reg {(~ __CYCLE_CNT__ == 0)|| (__m15__)}
assume -name variable_map_assume_23_reg {(~ __CYCLE_CNT__ == 0)|| (__m16__)}
assume -name variable_map_assume_24_reg {(~ __CYCLE_CNT__ == 0)|| (__m17__)}
assume -name variable_map_assume_25_reg {(~ __CYCLE_CNT__ == 0)|| (__m18__)}
assume -name variable_map_assume_26_reg {(~ __CYCLE_CNT__ == 0)|| (__m19__)}
assume -name variable_map_assume_27_reg {(~ __CYCLE_CNT__ == 0)|| (__m20__)}
assume -name variable_map_assume_28_reg {(~ __CYCLE_CNT__ == 0)|| (__m21__)}
assume -name variable_map_assume_29_reg {(~ __CYCLE_CNT__ == 0)|| (__m22__)}
assume -name variable_map_assume_30_reg {(~ __CYCLE_CNT__ == 0)|| (__m23__)}
assume -name variable_map_assume_31_reg {(~ __CYCLE_CNT__ == 0)|| (__m24__)}
assume -name variable_map_assume_32_reg {(~ __CYCLE_CNT__ == 0)|| (__m25__)}
assume -name variable_map_assume_33_reg {(~ __CYCLE_CNT__ == 0)|| (__m26__)}
assume -name variable_map_assume_34_reg {(~ __CYCLE_CNT__ == 0)|| (__m27__)}
assume -name variable_map_assume_35_reg {(~ __CYCLE_CNT__ == 0)|| (__m28__)}
assume -name variable_map_assume_36_reg {(~ __CYCLE_CNT__ == 0)|| (__m29__)}
assume -name variable_map_assume_37_reg {(~ __CYCLE_CNT__ == 0)|| (__m30__)}

