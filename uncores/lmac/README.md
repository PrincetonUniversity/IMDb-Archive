# Instruction-Level Abstraction (ILA) of LMAC
This ILA description is derived based on the LeWiz Communications Ethernet MAC (LMAC), which is licensed under GNU LGPL. 
Check below for further detail:

-   [LMAC_CORE1](https://github.com/lewiz-support/LMAC_CORE1)
-   [LMAC_CORE2](https://github.com/lewiz-support/LMAC_CORE2)

## Build
To build this ILA model, create a build directory and run:

``` bash
mkdir -p build && cd build
cmake ..
make
```

The output is an executable `lmac`, which can be used to generate the LMAC ILA and the verification target for equivalence checking. 

``` bash
./lamc -d <DESIGN_PATH> -o <OUTPUT_PATH> -i <INSTR_COND> -v <VAR_MAP>
```
