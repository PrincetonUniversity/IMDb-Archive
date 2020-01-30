SRCS=FIFO20.v \
FIFO2.v \
mkCPU.v \
mkCSR_MIE.v \
mkCSR_MIP.v \
mkCSR_RegFile.v \
mkGPR_RegFile.v \
BRAM2.v \
mkMMU_Cache.v \
mkNear_Mem.v \
mkRISCV_MBox.v \
mkSoC_Map.v \
RegFile.v \

test: $(SRCS)
	iverilog $(SRCS) -o test
