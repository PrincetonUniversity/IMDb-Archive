## 8051 Core
This ILA model is synthesized based on the [8051 microcontroller](https://opencores.org/projects/8051) from OpenCores. 

### Usage
To run the synthesis process, source the script `synthesize8051.sh`. 

The result of synthesis is archived in the [asts](asts) directory if you do not want to re-run the synthesis. 
It can used in ILAng as shown in `oc8051`. 

``` bash
# bash
mkdir -p oc8051-ila/build && cd oc8051-ila/build
cmake .. 
make
./oc8051
```
