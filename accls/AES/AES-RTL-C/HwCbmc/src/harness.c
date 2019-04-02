#include "AES.h"
#include <assert.h>
#include <stdio.h>
#define TRUE 1
#define FALSE 0

// ************* hw-cbmc interface ***********//
/* Unwinding Bound */

extern const unsigned int bound;

/* Next Timeframe  */

void next_timeframe(void);
void set_inputs(void);

/*
  Type declarations
*/

typedef unsigned char _u8;
typedef unsigned __CPROVER_bitvector[128] _u128;
typedef unsigned __CPROVER_bitvector[32] _u32;
typedef unsigned __CPROVER_bitvector[10] _u10;
typedef unsigned __CPROVER_bitvector[2] _u2;
typedef unsigned __CPROVER_bitvector[1] _u1;

/*
  Module Verilog::
*/

struct module_S {
  _Bool clk;
  _u8 in;
  _u8 out;
};

struct module_S4 {
  _Bool clk;
  _u32 in;
  _u32 out;

  struct module_S S_0;
  struct module_S S_1;
  struct module_S S_2;
  struct module_S S_3;
};

struct module_expand_key_128 {
  _Bool clk;
  _u128 in;
  _u8 rcon;
  _u128 out_1;
  _u128 out_2;

  struct module_S4 S4_0;
};

struct module_xS {
  _Bool clk;
  _u8 in;
  _u8 out;
};

struct module_T {
  _Bool clk;
  _u8 in;
  _u32 out;

  struct module_S s0;
  struct module_xS s4;
};

struct module_table_lookup {
  _Bool clk;
  _u32 state;
  _u32 p0;
  _u32 p1;
  _u32 p2;
  _u32 p3;

  struct module_T t0;
  struct module_T t1;
  struct module_T t2;
  struct module_T t3;
};

struct module_one_round {
  _Bool clk;
  _u128 state_in;
  _u128 key;
  _u128 state_out;

  struct module_table_lookup t0;
  struct module_table_lookup t1;
  struct module_table_lookup t2;
  struct module_table_lookup t3;
};

struct module_final_round {
  _Bool clk;
  _u128 state_in;
  _u128 key_in;
  _u128 state_out;

  struct module_S4 S4_1;
  struct module_S4 S4_2;
  struct module_S4 S4_3;
  struct module_S4 S4_4;
};

struct module_aes_128 {
  _Bool clk;
  _u128 state;
  _u128 key;
  _u128 out;

  struct module_expand_key_128 a1;
  struct module_expand_key_128 a2;
  struct module_expand_key_128 a3;
  struct module_expand_key_128 a4;
  struct module_expand_key_128 a5;
  struct module_expand_key_128 a6;
  struct module_expand_key_128 a7;
  struct module_expand_key_128 a8;
  struct module_expand_key_128 a9;
  struct module_expand_key_128 a10;

  struct module_one_round r1;
  struct module_one_round r2;
  struct module_one_round r3;
  struct module_one_round r4;
  struct module_one_round r5;
  struct module_one_round r6;
  struct module_one_round r7;
  struct module_one_round r8;
  struct module_one_round r9;

  struct module_final_round rf;
};

/*
  Hierarchy Instantiation
*/

extern struct module_aes_128 aes_128;

// *********** Implementation in C **************

_u8 nondet_bv8();

void main() {

  /* -------- Symbolic -------- */
  _u8 in_state[16];
  for (int i = 0; i < 16; i++) {
    in_state[i] = nondet_bv8();
  }

  _u8 in_key[16];
  for (int i = 0; i < 16; i++) {
    in_key[i] = nondet_bv8();
  }

  /* -------- RTL -------- */
  // input
  _u128 rtl_in_state = 0;
  for (int i = 0; i < 16; i++) {
    _u128 extended_byte = in_state[i];
    rtl_in_state = rtl_in_state | (extended_byte << (i * 8));
  }

  _u128 rtl_in_key = 0;
  for (int i = 0; i < 16; i++) {
    _u128 extended_byte = in_key[i];
    rtl_in_key = rtl_in_key | (extended_byte << (i * 8));
  }

  // execute
  for (int i = 0; i < 25; i++) {
    aes_128.state = rtl_in_state;
    aes_128.key = rtl_in_key;
    set_inputs();
    next_timeframe();
  }

  // output
  _u8 out_rtl_bytes[16];
  for (int i = 0; i < 16; i++) {
    out_rtl_bytes[i] = (aes_128.out >> (8 * i)) & 0xff;
  }

  /* -------- C -------- */
  // input
  _u32 c_in_state[4];
  for (int i = 0; i < 4; i++) {
    c_in_state[i] = 0;
    c_in_state[i] = (c_in_state[i] << 8) | in_state[12 - (i * 4)];
    c_in_state[i] = (c_in_state[i] << 8) | in_state[13 - (i * 4)];
    c_in_state[i] = (c_in_state[i] << 8) | in_state[14 - (i * 4)];
    c_in_state[i] = (c_in_state[i] << 8) | in_state[15 - (i * 4)];
  }

  _u32 c_in_key[4];
  for (int i = 0; i < 4; i++) {
    c_in_key[i] = 0;
    c_in_key[i] = (c_in_key[i] << 8) | in_key[12 - (i * 4)];
    c_in_key[i] = (c_in_key[i] << 8) | in_key[13 - (i * 4)];
    c_in_key[i] = (c_in_key[i] << 8) | in_key[14 - (i * 4)];
    c_in_key[i] = (c_in_key[i] << 8) | in_key[15 - (i * 4)];
  }

  _u32 c_tmp_result[4];
  for (int i = 0; i < 4; i++) {
    c_tmp_result[i] = c_in_state[i];
  }

  // execute
  encrypt_128_key_expand_inline_no_branch(c_tmp_result, c_in_key);

  // output
  _u8 out_c_bytes[16];
  for (int i = 0; i < 4; i++) {
    out_c_bytes[15 - (i * 4)] = c_tmp_result[i] & 0xff;
    out_c_bytes[14 - (i * 4)] = (c_tmp_result[i] >> 8) & 0xff;
    out_c_bytes[13 - (i * 4)] = (c_tmp_result[i] >> 16) & 0xff;
    out_c_bytes[12 - (i * 4)] = (c_tmp_result[i] >> 24) & 0xff;
  }

  /* -------- Compare -------- */
  assert(out_rtl_bytes[0] == out_c_bytes[0]);
  assert(out_rtl_bytes[1] == out_c_bytes[1]);
  assert(out_rtl_bytes[2] == out_c_bytes[2]);
  assert(out_rtl_bytes[3] == out_c_bytes[3]);
  assert(out_rtl_bytes[4] == out_c_bytes[4]);
  assert(out_rtl_bytes[5] == out_c_bytes[5]);
  assert(out_rtl_bytes[6] == out_c_bytes[6]);
  assert(out_rtl_bytes[7] == out_c_bytes[7]);
  assert(out_rtl_bytes[8] == out_c_bytes[8]);
  assert(out_rtl_bytes[9] == out_c_bytes[9]);
  assert(out_rtl_bytes[10] == out_c_bytes[10]);
  assert(out_rtl_bytes[11] == out_c_bytes[11]);
  assert(out_rtl_bytes[12] == out_c_bytes[12]);
  assert(out_rtl_bytes[13] == out_c_bytes[13]);
  assert(out_rtl_bytes[14] == out_c_bytes[14]);
  assert(out_rtl_bytes[15] == out_c_bytes[15]);

  return;
}

