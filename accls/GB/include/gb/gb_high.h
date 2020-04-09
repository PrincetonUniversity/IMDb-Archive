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

// File: gb_high.h

#ifndef GB_HIGH_H__
#define GB_HIGH_H__

#include <ilang/ilang++.h>

#include <gb/gb_config.h>

namespace ilang {

/// \brief High-level Gaussian Blur
class GbHigh {
public:
  // ------------------------- CONSTRUCTOR/DESTRUCTOR ----------------------- //
  /// Default constructor
  GbHigh();

  // ------------------------- ACCESSORS/MUTATORS --------------------------- //
  /// Return the ILA model
  const Ila& get() const { return model_; }

private:
  // ------------------------- METHODS -------------------------------------- //
  /// Define top-level write instruction
  InstrRef DefineWrInstr();
  /// Define top-level read instruction
  InstrRef DefineRdInstr();
  /// Define the child-ILA that propagates the pixels
  Ila DefineChildPropagate();

  // ------------------------- MEMBERS -------------------------------------- //
  /// The ILA model
  Ila model_;

  //
  // Input
  //
  /// GB -> host ready
  ExprRef arg_0_TREADY;
  /// host -> GB valid
  ExprRef arg_1_TVALID;
  /// host -> GB data
  ExprRef arg_1_TDATA;

  //
  // Output
  //
  /// host -> GB ready
  ExprRef arg_1_TREADY;
  /// GB -> host valid
  ExprRef arg_0_TVALID;
  /// GB -> host data
  ExprRef arg_0_TDATA;

  //
  // Internal memory state
  //
  /// 488 x 1 bytes buffers in the 2-D RAM
  std::vector<ExprRef> ram;

  //
  // Internal arch. state
  //
  /// stencil ready
  ExprRef st_ready;
  /// most recent pixel
  ExprRef cur_pix;
  /// x index (column) in the 2-D RAM
  ExprRef ram_x;
  /// y index (row) in the 2-D RAM
  ExprRef ram_y;
  /// w index (write) in the 2-D RAM
  ExprRef ram_w;
  /// holding iterator for gb kernel computation
  ExprRef gbit;
  /// 1 x 9 bytes slices in the stencil
  std::vector<ExprRef> stencil;

  //
  // Helper, constant, shared, etc.
  //
  /// constant for ready true
  ExprRef ready_t = BvConst(1, 1);
  /// constant for ready false
  ExprRef ready_f = BvConst(0, 1);
  /// constant for valid true
  ExprRef valid_t = BvConst(1, 1);
  /// constant for valid false
  ExprRef valid_f = BvConst(0, 1);
  /// constant 0 for x index
  ExprRef ram_x_0 = BvConst(0, GB_COL_ADDR_SIZE);
  /// constant 1 for x index
  ExprRef ram_x_1 = BvConst(1, GB_COL_ADDR_SIZE);
  /// constant max for x index
  ExprRef ram_x_max = BvConst(GB_IMG_X_SIZE, GB_COL_ADDR_SIZE);
  /// constant 0 for y index
  ExprRef ram_y_0 = BvConst(0, GB_ROW_ADDR_SIZE);
  /// constant 1 for y index
  ExprRef ram_y_1 = BvConst(1, GB_ROW_ADDR_SIZE);
  /// constant max for y index
  ExprRef ram_y_max = BvConst(GB_IMG_Y_SIZE, GB_ROW_ADDR_SIZE);
  /// constant 0 for w index
  ExprRef ram_w_0 = BvConst(0, GB_WR_ADDR_SIZE);
  /// constant 1 for w index
  ExprRef ram_w_1 = BvConst(1, GB_WR_ADDR_SIZE);
  /// constant max for w index
  ExprRef ram_w_max = BvConst(GB_RAM_SIZE - 1, GB_WR_ADDR_SIZE);
  /// uninterpreted func for the kernel GB
  FuncRef gb_func =
      FuncRef("kernel", SortRef::BV(GB_DATA_SIZE),
              SortRef::BV(GB_X_EXTEND* GB_Y_EXTEND* GB_DATA_SIZE));

  // ------------------------- HELPERS -------------------------------------- //
  std::vector<int> GenSliceSeqs(const int& start);
  ExprRef SliceSelect(const ExprRef& start, std::vector<int> seqs);
  ExprRef SliceSelectOne(const int& mod_case, const ExprRef& start,
                         std::vector<int> seqs);
  ExprRef GenRows(const int& idx);

}; // class GbHigh

}; // namespace ilang

#endif // GB_HIGH_H__
