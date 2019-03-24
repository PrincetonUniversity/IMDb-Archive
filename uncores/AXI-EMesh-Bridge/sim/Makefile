SRCS= \
../verilog/oh_memory_ram.v \
../verilog/oh_memory_dp.v \
../verilog/oh_fifo_sync.v \
../verilog/packet2emesh.v \
../verilog/emesh2packet.v \
../verilog/oh_dsync.v \
../verilog/emaxi.v \
emaxi.t.v

sim: ${SRCS}
	iverilog ${SRCS} -o sim

run: sim
	./sim
