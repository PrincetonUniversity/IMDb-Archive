# Modeling and Verification Case Studies
ILA models and synthesis templates for processor/accelerator case studies.
Note that the models and templates are constructed using the [Python API](https://github.com/PrincetonILA/ILA-Synthesis).
The ILA formal model and details of the modeling/verification case studies are shown in our [paper](https://arxiv.org/abs/1801.01114).

[8051](https://github.com/PrincetonILA/Modeling-and-Verification/8051): 
* ILA synthesis templates for the 8051 micro-processor, 
* Verification set-up using ABC,
* Generated instruction-level simulator.

[AES-RTL-C](https://github.com/PrincetonILA/Modeling-and-Verification/AES-RTL-C): 
* Two implementations of the AES,
* ILA synthesis templates for the two AES implementation, 
* Verification set-up using CBMC, JasperGold, and Z3,
* A tutorial on ILA synthesis and equivalence checking.

[AES](https://github.com/PrincetonILA/Modeling-and-Verification/AES): 
* ILA synthesis templates for the Python Crypto library,
* Different hierarchy of the AES ILA.

[ALU](https://github.com/PrincetonILA/Modeling-and-Verification/ALU): 
* ILA synthesis templates for a small ALU,
* Generated instruction-level simulator.

[GB-Halide](https://github.com/PrincetonILA/Modeling-and-Verification/GB-Halide): 
* High-level ILA of the Gaussian Blur accelerator,
* Low-level ILA of the Gaussian Blur accelerator,
* Verification set-up, e.g. Verilog wrapper, JasperGold tcl commands, SVA assumptions/assertions.

[NyILA](https://github.com/PrincetonILA/Modeling-and-Verification/NyILA):
* The ILA model for NyuziGPU.

[RBM](https://github.com/PrincetonILA/Modeling-and-Verification/RBM)
* ILA synthesis templates for the Restructed Boltzmann machine (RBM) accelerator,
* The SystemC and Verilog reference models,
* Verification set-up for ILA v.s. FSM equivalence checking.

[RISC-V](https://github.com/PrincetonILA/Modeling-and-Verification/RISC-V):
* ILA synthesis templates for RISC-V Rocket Core,
* Verification set-up for equivalence checking between the ILA and the Verilog RTL implementation.

[SHA](https://github.com/PrincetonILA/Modeling-and-Verification/SHA): 
* ILA synthesis templates for the Secure Hash Algorithm (SHA) accelerator, with hierarchy.
* Generated instruction-level simulator.

[South-Island-GPU](https://github.com/PrincetonILA/Modeling-and-Verification/South-Island-GPU):
* The ILA model of the AMD south island GPU.




