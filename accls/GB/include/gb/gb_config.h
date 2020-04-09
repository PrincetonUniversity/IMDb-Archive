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

// File: gb_config.h

#ifndef GB_CONFIG_H__
#define GB_CONFIG_H__

#include <ilang/ilang++.h>

namespace ilang {

#define GB_DATA_SIZE 8
#define GB_COUNT_SIZE 19
#define GB_X_EXTEND 9
#define GB_Y_EXTEND 9
#define GB_IMG_X_SIZE 488
#define GB_IMG_Y_SIZE 648
#define GB_COL_ADDR_SIZE 9
#define GB_ROW_ADDR_SIZE 10
#define GB_WR_ADDR_SIZE 3

#define GB_RAM_SIZE (GB_Y_EXTEND - 1)
#define GB_SLICE_SIZE (GB_Y_EXTEND * GB_DATA_SIZE)
#define GB_STENCIL_SIZE GB_X_EXTEND

}; // namespace ilang

#endif // GB_CONFIG_H__
