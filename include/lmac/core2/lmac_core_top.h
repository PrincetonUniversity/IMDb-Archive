// ============================================================================
// Instruction-Level Abstraction of LeWiz Communications Ethernet MAC
//
// This Instruction-Level Abstraction (ILA) description is derived based on the
// LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL.
// Check "LICENSE" which comes with this distribution for more information.
// ============================================================================
//
// File Name: lmac_core_top.h

// LmacCore2 top level entry points
//  - TX FIFO
//  - RX FIFO
//  - Register interface and configuration
//  - PHY

#ifndef LMAC_CORE2_LMAC_CORE_TOP_H__
#define LMAC_CORE2_LMAC_CORE_TOP_H__

#include <string>

#include <ilang/ilang++.h>

namespace ilang {

// Top level model of LMAC CORE 2
class LmacCore2 {
public:
  // Default constructor
  LmacCore2();
  // Default destructor
  ~LmacCore2();

  // return a new ILA with the given name
  static Ila New(const std::string& name = "LMAC_CORE2");

protected:
  //

private:
  // set architectural state variable (spec./doc.)
  static void SetArchStateVar(Ila& m);
  // set implementation state variable (internal)
  static void SetImplStateVar(Ila& m);
  // set child ila
  static void SetChild(Ila& m);
  // set instruction
  static void SetInstr(Ila& m);
  // set initial conditions
  static void SetInit(Ila& m);

  // TX FIFO
  static void SetupTxInterface(Ila& m);
  static void SetupTxInternal(Ila& m);
  static void SetupTxInstr(Ila& m);

  // RX FIFO
  static void SetupRxInterface(Ila& m);
  static void SetupRxInternal(Ila& m);
  static void SetupRxInstr(Ila& m);

  // set state variables of register interface and configuration
  static void SetupMmioRegs(Ila& m);
  static void SetupRegInterface(Ila& m);
  static void SetupRegInternal(Ila& m);
  static void SetupRegInstr(Ila& m);

  // set state variables of PHY
  static void SetupPhyInterface(Ila& m);
  static void SetupPhyInternal(Ila& m);
  static void SetupPhyInstr(Ila& m);

}; // class LmacCore2

}; // namespace ilang

#endif // LMAC_CORE2_LMAC_CORE_TOP_H__
