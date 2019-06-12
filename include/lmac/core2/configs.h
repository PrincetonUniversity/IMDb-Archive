// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: configs.h

// modeling choices
// non-hardware parameters

#ifndef LMAC_CORE2_CONFIGS_H__
#define LMAC_CORE2_CONFIGS_H__

#include <lmac/core2/configs_impl.h>
#include <lmac/core2/configs_spec.h>
#include <lmac/share.h>

namespace ilang {

// minimum packet size (byte count)
#define MIN_PKT_SIZE 64
// CRC byte count
#define CRC_BYTE_CNT 4

}; // namespace ilang

#endif // LMAC_CORE2_CONFIGS_H__

