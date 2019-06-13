// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: share.h

// modeling choices
// non-hardware parameters

#ifndef LMAC_SHARE_H__
#define LMAC_SHARE_H__

namespace ilang {

// bit-width of a byte
#define BYTE 8
// bit-width of a word
#define WORD 16
// bit-width of a double-word
#define DWORD 32
// bit-width of a quad-word
#define QWORD 64

// model Boolean expr as 1-bit bit-vector
// #define ICFG_BOOL_AS_BV

}; // namespace ilang

#endif // LMAC_SHARE_H__

