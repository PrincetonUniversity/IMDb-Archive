# Instruction-level abstraction (ILA) of the Gaussian Blur accelerator

This is the ILA model of the [GB](https://github.com/jingpu/Halide-HLS.git) accelerator developed by the Stanford VLSI Research Group led by Prof. Mark Horowitz. 
Check [LICENSE](https://github.com/jingpu/Halide-HLS/blob/HLS/LICENSE.txt) for further details.

## Directory structure
 
-   `include/gb` contains header files for functions and macros (e.g., state name, bit-width, etc.)
    -   `gb_config.h` defines design parameters
    -   `gb_high.h` provides the structure of the high-level ILA

-   `src` contains source files for functions to construct the ILA model
    -   `gb_high.cc` defines the high-level ILA

-   `app` contains the `main.cc` for applications using the ILA model

