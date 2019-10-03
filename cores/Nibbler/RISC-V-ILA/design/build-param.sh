echo "Building param for P_NBITS=" ${P_NBITS}

export NIBBLER_ROOT=$PWD

export PYTHONPATH=${NIBBLER_ROOT}/py:${PYTHONPATH}

mkdir -p ${NIBBLER_ROOT}/build/${TIMESTAMP}-${P_NBITS}
export SRC_OUT_DIR=${NIBBLER_ROOT}/build/${TIMESTAMP}-${P_NBITS}
echo "SRC_OUT_DIR=${SRC_OUT_DIR}"

#Generate correct param_Defines.py
echo "import math
# // DECLARE CONSTANTS
P_NBITS= "$P_NBITS"                            #// Bit width of datapath
C_N_OFF = 32/P_NBITS                  #// Number of total n-bit chunks per word. 
C_OFFBITS= int( math.log(C_N_OFF,2) ) #// # of bits needed to index the n-bit chunks.
C_OFFBITS_STR= str(C_OFFBITS)
N_CTRL_SIGNALS = 49" > param_Defines.py

make rtl/param-Core.v
make rtl/param-CoreDpathAlu.v
make rtl/param-CoreDpathRegfile.v
make rtl/param-Ctrl.v
make rtl/param-DeserializedReg.v
make rtl/param-Dpath.v
make rtl/param-PCComputation.v
make rtl/param-ShiftDemux.v
make rtl/param-SIMDLaneDpath.v

# move generated files to new build directory
mv rtl/param-Core.v             $SRC_OUT_DIR
mv rtl/param-CoreDpathAlu.v     $SRC_OUT_DIR
mv rtl/param-CoreDpathRegfile.v $SRC_OUT_DIR
mv rtl/param-Ctrl.v             $SRC_OUT_DIR
mv rtl/param-DeserializedReg.v  $SRC_OUT_DIR
mv rtl/param-Dpath.v            $SRC_OUT_DIR
mv rtl/param-PCComputation.v    $SRC_OUT_DIR
mv rtl/param-ShiftDemux.v       $SRC_OUT_DIR
mv rtl/param-SIMDLaneDpath.v    $SRC_OUT_DIR

# copy parameterized files to new build dir
cp rtl/param-ClkEnBuf.v         $SRC_OUT_DIR

