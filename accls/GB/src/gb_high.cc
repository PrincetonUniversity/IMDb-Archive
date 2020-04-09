// =============================================================================
// MIT License
//
// Copyright (c) 2020 Princeton University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// =============================================================================

// File: gb_high.cc

#include <gb/gb_config.h>
#include <gb/gb_high.h>

#include <ilang/util/log.h>

namespace ilang {

GbHigh::GbHigh()
    : model_("GBHigh"),
      // input
      arg_0_TREADY(model_.NewBvInput("arg_0_TREADY", 1)),
      arg_1_TVALID(model_.NewBvInput("arg_1_TVALID", 1)),
      arg_1_TDATA(model_.NewBvInput("arg_1_TDATA", GB_DATA_SIZE)),
      // output
      arg_1_TREADY(model_.NewBvState("arg_1_TREADY", 1)),
      arg_0_TVALID(model_.NewBvState("arg_0_TVALID", 1)),
      arg_0_TDATA(model_.NewBvState("arg_0_TDATA", GB_DATA_SIZE)),
      // internal arch
      st_ready(model_.NewBvState("st_ready", 1)),
      cur_pix(model_.NewBvState("cur_pic", GB_DATA_SIZE)),
      ram_x(model_.NewBvState("ram_x", GB_COL_ADDR_SIZE)),
      ram_y(model_.NewBvState("ram_y", GB_ROW_ADDR_SIZE)),
      ram_w(model_.NewBvState("ram_w", GB_WR_ADDR_SIZE)),
      gbit(model_.NewBvState("gbit", 4)) {
  //
  // parametrized internal arch
  //
  for (auto i = 0; i < GB_STENCIL_SIZE; i++) {
    stencil.push_back(
        model_.NewBvState("stencil" + std::to_string(i), GB_SLICE_SIZE));
  }
  for (auto i = 0; i < GB_RAM_SIZE; i++) {
    ram.push_back(model_.NewMemState("ram" + std::to_string(i),
                                     GB_COL_ADDR_SIZE, GB_DATA_SIZE));
  }

  //
  // valid
  //
  model_.SetValid(((arg_0_TREADY == ready_t) & (arg_0_TVALID == valid_t)) |
                  ((arg_1_TVALID == valid_t) & (arg_1_TREADY == ready_t)));

  //
  // instruction
  //
  auto instr_wr = DefineWrInstr();
  auto instr_rd = DefineRdInstr();

  //
  // child-ILA
  //
  auto child_prop = DefineChildPropagate();
  instr_wr.SetProgram(child_prop);
  instr_rd.SetProgram(child_prop);

  //
  // valid sequence
  //
  // TODO

} // GbHigh constructor

InstrRef GbHigh::DefineWrInstr() {
  auto instr = model_.NewInstr("Write");

  instr.SetDecode((arg_1_TVALID == valid_t) & (arg_1_TREADY == ready_t) &
                  (arg_0_TVALID == valid_t) & (arg_0_TREADY == ready_f));

  // state updates
  instr.SetUpdate(arg_1_TREADY, ready_f);
  instr.SetUpdate(arg_0_TVALID, valid_f);
  instr.SetUpdate(arg_0_TDATA, arg_0_TDATA);

  instr.SetUpdate(cur_pix, arg_1_TDATA);
  instr.SetUpdate(ram_x, ram_x);
  instr.SetUpdate(ram_y, ram_y);
  instr.SetUpdate(ram_w, ram_w);
  for (auto m : ram) {
    instr.SetUpdate(m, m);
  }
  for (auto s : stencil) {
    instr.SetUpdate(s, s);
  }

  return instr;
} // DefineWrInstr

InstrRef GbHigh::DefineRdInstr() {
  auto instr = model_.NewInstr("Read");

  instr.SetDecode((arg_0_TVALID == valid_t) & (arg_0_TREADY == ready_t) &
                  (arg_1_TVALID == valid_f));

  // state updates
  instr.SetUpdate(arg_1_TREADY, arg_1_TREADY);
  instr.SetUpdate(arg_0_TVALID, !((gbit == 0) | (gbit == 7)));
  instr.SetUpdate(arg_0_TDATA, arg_0_TDATA);

  instr.SetUpdate(cur_pix, cur_pix);
  instr.SetUpdate(ram_x, ram_x);
  instr.SetUpdate(ram_y, ram_y);
  instr.SetUpdate(ram_w, ram_w);
  for (auto m : ram) {
    instr.SetUpdate(m, m);
  }
  for (auto s : stencil) {
    instr.SetUpdate(s, s);
  }
  instr.SetUpdate(
      gbit, Ite((ram_x == ram_x_max) & (ram_y == ram_y_max), gbit + 1, gbit));

  return instr;
} // DefineRdInstr

Ila GbHigh::DefineChildPropagate() {
  auto child = model_.NewChild("Propagate");

  //
  // valid
  //
  child.SetValid((arg_1_TREADY == ready_f) & (arg_0_TREADY == ready_f));

  //
  // micro state
  //
  auto pre_pix = child.NewBvState("pre_pix", GB_DATA_SIZE);
  auto proc_in = child.NewBvState("proc_in", GB_SLICE_SIZE * GB_STENCIL_SIZE);

  //
  // instruction
  //
  { // propagate 0 (RAM)
    auto instr = child.NewInstr("prop0");

    instr.SetDecode((arg_1_TREADY == ready_f) & (arg_0_TREADY == ready_f) &
                    (st_ready == ready_t));

    // state updates
    auto init_cond = (ram_x == 0) & (ram_y == 0);
    auto acc_pixel = (ram_y < GB_RAM_SIZE);

    instr.SetUpdate(arg_1_TREADY,
                    Ite(init_cond, ready_t, Ite(acc_pixel, ready_t, ready_f)));
    instr.SetUpdate(arg_0_TVALID, Ite(init_cond, valid_f, arg_0_TVALID));
    instr.SetUpdate(arg_0_TDATA, arg_0_TDATA);
    instr.SetUpdate(cur_pix, cur_pix);
    instr.SetUpdate(pre_pix, pre_pix);
    instr.SetUpdate(ram_x, Ite(init_cond, ram_x_1,
                               Ite(ram_x == ram_x_max, ram_x_1, ram_x + 1)));
    instr.SetUpdate(ram_y,
                    Ite(ram_x == ram_x_max,
                        Ite(ram_y == ram_y_max, ram_y_0, ram_y + 1), ram_y));
    instr.SetUpdate(ram_w,
                    Ite(ram_x == ram_x_max,
                        Ite(ram_w == ram_w_max, ram_w_0, ram_w + 1), ram_w));
    for (auto i = 0; i < GB_RAM_SIZE; i++) {
      auto m = ram[i];
      instr.SetUpdate(m, Ite(init_cond, m,
                             Ite(ram_w == i, Store(m, ram_x - 1, pre_pix), m)));
    }
    for (auto i = 0; i < GB_STENCIL_SIZE - 1; i++) {
      instr.SetUpdate(stencil[i], stencil[i]);
    }
    { // last stencil
      auto last = stencil.back();

      auto slice_seqs = std::vector<std::vector<int>>();
      for (auto i = 0; i < GB_RAM_SIZE; i++) {
        slice_seqs.push_back(GenSliceSeqs(i));
      }

      auto slice_chunks = std::vector<ExprRef>();
      for (auto i = GB_RAM_SIZE - 1; i >= 0; i--) {
        slice_chunks.push_back(SliceSelect(ram_w, slice_seqs[i]));
      }

      auto last_nxt = pre_pix;
      for (auto i = 0; i < slice_chunks.size(); i++) {
        last_nxt = Concat(slice_chunks[i], last_nxt);
      }

      instr.SetUpdate(last, Ite(ram_y < GB_RAM_SIZE, last, last_nxt));
    }

    instr.SetUpdate(st_ready,
                    Ite(init_cond, ready_t, Ite(acc_pixel, ready_t, ready_f)));
    instr.SetUpdate(proc_in, proc_in);
  }

  { // propagate 1 (stencil)
    auto instr = child.NewInstr("prop1");

    instr.SetDecode((arg_1_TREADY == ready_f) & (arg_0_TREADY == ready_f) &
                    (st_ready == ready_f));

    // state updates
    auto end_pixel = (ram_x == ram_x_max - 1) & (ram_y == ram_y_max - 1);
    auto rel_pixel = (ram_x == 1) & (ram_y == ram_y_max);

    auto stencil_rows = std::vector<ExprRef>();
    for (auto i = GB_STENCIL_SIZE - 1; i >= 0; i--) {
      stencil_rows.push_back(GenRows(i));
    }
    auto concat_stencil_rows = stencil_rows[0];
    for (auto i = 1; i < stencil_rows.size(); i++) {
      concat_stencil_rows = Concat(stencil_rows[i], concat_stencil_rows);
    }

    instr.SetUpdate(proc_in, Ite(rel_pixel, proc_in,
                                 Ite(((ram_x > GB_STENCIL_SIZE - 1) &
                                      (ram_y >= GB_RAM_SIZE)) |
                                         ((ram_x == 1) & (ram_y > GB_RAM_SIZE)),
                                     concat_stencil_rows, proc_in)));

    instr.SetUpdate(arg_1_TREADY, Ite(end_pixel, ready_f, ready_t));

    instr.SetUpdate(
        arg_0_TVALID,
        Ite(rel_pixel, arg_0_TVALID,
            Ite(((ram_x > GB_STENCIL_SIZE - 1) & (ram_y >= GB_RAM_SIZE)) |
                    ((ram_x == 1) & (ram_y > GB_RAM_SIZE)),
                valid_t, valid_f)));

    auto apply_gb = gb_func(instr.GetUpdate(proc_in));
    instr.SetUpdate(arg_0_TDATA, Ite(rel_pixel, arg_0_TDATA, apply_gb));

    instr.SetUpdate(cur_pix, cur_pix);
    instr.SetUpdate(pre_pix, pre_pix);
    instr.SetUpdate(ram_x, ram_x);
    instr.SetUpdate(ram_y, ram_y);
    instr.SetUpdate(ram_w, ram_w);

    for (auto m : ram) {
      instr.SetUpdate(m, m);
    }

    for (auto i = 0; i < GB_STENCIL_SIZE - 1; i++) {
      instr.SetUpdate(stencil[i],
                      Ite(ram_y < GB_RAM_SIZE, stencil[i], stencil[i + 1]));
    }
    instr.SetUpdate(stencil.back(), stencil.back());

    instr.SetUpdate(st_ready, ready_t);
  }

  return child;
}

std::vector<int> GbHigh::GenSliceSeqs(const int& start) {
  auto res = std::vector<int>();
  for (auto i = 0; i < GB_RAM_SIZE; i++) {
    res.push_back((start + i) % GB_RAM_SIZE);
  }
  return res;
}

ExprRef GbHigh::SliceSelect(const ExprRef& start, std::vector<int> seqs) {
  return SliceSelectOne(0, start, seqs);
}

ExprRef GbHigh::SliceSelectOne(const int& mod_case, const ExprRef& start,
                               std::vector<int> seqs) {
  auto idx = seqs[mod_case];
  if (mod_case == GB_RAM_SIZE - 1) {
    return Load(ram[idx], ram_x - 1);
  } else {
    return Ite(start == mod_case, Load(ram[idx], ram_x - 1),
               SliceSelectOne(mod_case + 1, start, seqs));
  }
}

ExprRef GbHigh::GenRows(const int& idx) {
  auto l = GB_DATA_SIZE * idx;
  auto h = l + GB_DATA_SIZE - 1;

  auto res = Extract(stencil.front(), h, l);
  for (auto i = 1; i < GB_STENCIL_SIZE; i++) {
    res = Concat(Extract(stencil[i], h, l), res);
  }
  return res;
}

}; // namespace ilang
