ILA-Based Sound Co-Simulation Model Generation Demo
==================================================================================

Introduction
---------------------------------------------------------------------------------
A co-simulation model for an AES device is generated using ILAng for HW/SW co-simulation. The generation process is fully automated using the sim\_gen API of the ILAng framework. This demo also integrates the generated co-simulation model with a complete SoC hardware model, and presents a software running on the integrated model.

Prerequisites
---------------------------------------------------------------------------------

1.  ILAng is the platform to model hardware components. An up-to-date version needs to be installed, available [here](https://github.com/Bo-Yuan-Huang/ILAng).
2.  IMDb is github repo that holds this demo.
3.  [SystemCTLM](https://github.com/Xilinx/systemctlm-cosim-demo) contains the SystemC model of a small SoC. Our generated AES co-simulation model is integrated with it.
4.  SystemC library is needed to compile the AES co-simulation model and also the small SoC.

Steps to use
---------------------------------------------------------------------------------

0.  Get all related repositories:
```
git clone https://github.com/Bo-Yuan-Huang/ILAng.git
git clone https://github.com/PrincetonUniversity/IMDb.git
git clone https://github.com/Xilinx/systemctlm-cosim-demo.git
```

1.  Generate the co-simulation model from the AES device's ILA model.
```
(SYSTEMC_DIR=path_to_systemc_dir ILANG_PATH=path_to_ilang_repo IMDB_PATH=path_to_imdb_repo ./ilasim_demo.sh)
```

2.  Integrated the AES co-simulation model with the small SoC model
```
cp demo_files_copy_to_xilinx/* path_to_systemctlm-cosim-demo
cd path_to_systemctlm-cosim-demo
make zynqmp_demo
```

3.  Run the co-simulation
There'll be a Qemu session (with a linux OS) to emulate the core, and the zynqmp\_demo (compiled in 2) simulates the rest of the system.  On the running linux OS, we need to get the software script and run it.
```
wget https://raw.githubusercontent.com/PrincetonUniversity/IMDb/ilasim-demo/accls/AES/AES-ILASIM/demo_files_copy_to_xilinx/demo_script_on_linux.sh
bash ./demo_script_on_linux.sh
```
You'll see the result of running the software on the simulated SoC. The software calls the AES device to encrypt and decrypt a text and checks the encrypted and decrypted result.

(An intro to initiate and run the integrated SoC model can be found in the SystemCTLM repo.)

Docker Image
---------------------------------------------------------------------------------

A docker image with pre-built ILAng, SystemC, AES device's co-simulation model and integrated SoC model is provided:.
With this you only need to run the Step 3.
