# Modeling and Verification Case Studies
ILA models and synthesis templates for processor/accelerator case studies.
Note that the models and templates are constructed using the [Python API](https://github.com/PrincetonILA/ILA-Synthesis).

[8051](https://github.com/PrincetonILA/Modeling-and-Verification/8051): 
(i) Synthesis templates for the 8051 micro-processor, 
(ii) Verification set-up using ABC,
(iii) Generated instruction-level simulator.

[GB-Halide](https://github.com/PrincetonILA/Modeling-and-Verification/GB-Halide): 
(i) High-level ILA of the Gaussian Blur accelerator,
(ii) Low-level ILA of the Gaussian Blur accelerator,
(iii) Verification set-up, e.g. Verilog wrapper, JasperGold tcl commands, SVA assumptions/assertions.

[ALU](https://github.com/PrincetonILA/Modeling-and-Verification/ALU): 
(i) Synthesis templates for a small ALU,
(ii) Generated instruction-level simulator.

[SHA](https://github.com/PrincetonILA/Modeling-and-Verification/SHA): 
(1) Synthesis templates for the Secure Hash Algorithm (SHA) accelerator, with hierarchy.
(2) Generated instruction-level simulator.

[AES-RTL-C](https://github.com/PrincetonILA/Modeling-and-Verification/AES-RTL-C)
provides a tutorial on synthesizing ILAs for AES from RTL implementation and from C implementation. 
The verification set-up for equivalence checking between two implementations are also included.

[RBM](https://github.com/PrincetonILA/Modeling-and-Verification/RBM)
contains the case study where two ILAs of the Restricted Boltzmann machine accelerator are constructed and verified, w.r.t the SystemC and Verilog reference models, via ILA v.s. FSM equivalence checking.

[RISC-V](https://github.com/PrincetonILA/Modeling-and-Verification/RISC-V)
constains the ILA of the RISC-V ISA and the verification framework that checks the equivalence between the ILA and the Rocket core RTL implementation.


