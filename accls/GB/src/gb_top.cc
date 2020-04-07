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

// File: gb_top.cc

#include <gb/gb_top.h>

#include <ilang/util/log.h>

namespace ilang {

Ila GetGbIla(const std::string& model_name) {
  auto m = Ila(model_name);

  // define interface and architectural states
  auto addr = m.NewBvInput("addr", 16);
  auto data = m.NewBvInput("data", 8);
  auto ctrl = m.NewBvState("ctrl", 8);
  auto mode = m.NewBvState("mode", 8);

  // define ILA valid function
  m.SetValid(addr >= 0xFF00 & addr < 0xFF10);

  // define instruction - decode and state update functions
  auto instr_wr_ctrl = m.NewInstr("WrCtrl");
  instr_wr_ctrl.SetDecode(addr == 0xFF00);
  instr_wr_ctrl.SetUpdate(ctrl, Ite(data < 8, data, ctrl));

  // TODO

  return m;
}

}; // namespace ilang
