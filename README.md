[![Codacy Badge](https://api.codacy.com/project/badge/Grade/129d02949d13460c910acda8d5408cc8)](https://app.codacy.com/app/Bo-Yuan-Huang/IMDb?utm_source=github.com&utm_medium=referral&utm_content=PrincetonUniversity/IMDb&utm_campaign=Badge_Grade_Dashboard)
[![Build Status](https://travis-ci.org/PrincetonUniversity/IMDb.svg?branch=master)](https://travis-ci.org/PrincetonUniversity/IMDb)

This is the ILA model database, archiving the ILA models synthesized or manually defined using [ILAng](https://github.com/Bo-Yuan-Huang/ILAng).

## Content

### Tutorials

-   [AES](tutorials/aes) is a documented tutorial for ILA-based behavioral equivalence checking. 

### Examples

-   [FIFO-BMC](examples/FIFO-BMC) demonstrates the bounded model checking (BMC) capability of ILAng using a FIFO example. 

### Cores

-   [RISC-V](cores/RISC-V)
-   [PTX](cores/PTX)

### Accelerators

### Uncores

-   [LMAC](uncores/lmac): LeWiz Communications Ethernet MAC. 

### Others

## Contribute

1.  Please ensure all commited files follow the [MIT License](LICENSE) requirements with the help of references/submodules.
2.  Please properly categorize the design and provide scripts for setting up/reproducing the case study in `scripts/ci`.
3.  Please test and make sure your model can be built under the below environment:

-   Ubuntu 18.04 LTS (Bionic)
-   gcc 7.4.0 
-   Python 2.7
-   boost 1.65.1
-   z3 4.4.1
-   bison 3.0.4
-   flex 2.6.0
-   ILAng (0.9.1 or above)
