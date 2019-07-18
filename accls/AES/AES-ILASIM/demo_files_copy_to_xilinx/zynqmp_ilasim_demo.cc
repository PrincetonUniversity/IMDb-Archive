/*
 * Top level of the ZynqMP cosim example.
 *
 * Copyright (c) 2014 Xilinx Inc.
 * Written by Edgar E. Iglesias
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <inttypes.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "systemc.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "trace.h"
#include "iconnect.h"
#include "memory.h"
#include "debugdev.h"
#include "demo-dma.h"
#include "xilinx-zynqmp.h"

#include "checkers/pc-axilite.h"
#include "tlm-bridges/tlm2axilite-bridge.h"
#include "tlm-bridges/tlm2axi-bridge.h"
#include "tlm-bridges/tlm2apb-bridge.h"
#include "aes_ila_tlm.h"
#ifdef HAVE_VERILOG_VCS
#include "apb_slave_timer.h"
#endif

#ifdef HAVE_VERILOG_VERILATOR
#include "Vapb_timer.h"
#include "Vaxilite_dev_v1_0.h"
#include "verilated.h"
#endif

#define NR_DEMODMA      0
#define NR_MASTERS	1 + NR_DEMODMA
#define NR_DEVICES	3 + NR_DEMODMA

SC_MODULE(Top)
{
	SC_HAS_PROCESS(Top);
	iconnect<NR_MASTERS, NR_DEVICES>	*bus;
	xilinx_zynqmp zynq;

	sc_signal<bool> rst, rst_n;

	sc_clock *clk;

	aes_ila_tlm *aes_device;
#define AXIFULL_DATA_WIDTH 128
#define AXIFULL_ID_WIDTH 8
#ifdef HAVE_VERILOG
	cout << "have verilog" << endl;
	tlm2axilite_bridge<4, 32> *tlm2axi_al;
#else
	memory mem_al;
	// memory mem_af;
#endif
#ifdef HAVE_VERILOG
#ifdef HAVE_VERILOG_VCS
#endif
#ifdef HAVE_VERILOG_VERILATOR
	Vaxilite_dev_v1_0 *al;
	// Vaxifull_AES *af;
#endif
#endif

	sc_signal<bool> al_awvalid;
	sc_signal<bool> al_awready;
	sc_signal<sc_bv<4> > al_awaddr;
	sc_signal<sc_bv<3> > al_awprot;

	sc_signal<bool> al_wvalid;
	sc_signal<bool> al_wready;
	sc_signal<sc_bv<32> > al_wdata;
	sc_signal<sc_bv<4> > al_wstrb;

	sc_signal<bool> al_bvalid;
	sc_signal<bool> al_bready;
	sc_signal<sc_bv<2> > al_bresp;

	sc_signal<bool> al_arvalid;
	sc_signal<bool> al_arready;
	sc_signal<sc_bv<4> > al_araddr;
	sc_signal<sc_bv<3> > al_arprot;

	sc_signal<bool> al_rvalid;
	sc_signal<bool> al_rready;
	sc_signal<sc_bv<32> > al_rdata;
	sc_signal<sc_bv<2> > al_rresp;

	void gen_rst_n(void)
	{
		rst_n.write(!rst.read());
	}

	Top(sc_module_name name, const char *sk_descr, sc_time quantum) :
		zynq("zynq", sk_descr),
		rst("rst"),
		rst_n("rst_n"),
#ifdef HAVE_VERILOG
#else
		mem_al("mem_al", sc_time(1, SC_NS), 4 * 4),
		// 128bits x 512 slots.
		// mem_af("mem_af", sc_time(1, SC_NS), 16 * 512),
#endif

		al_awvalid("al_awvalid"),
		al_awready("al_awready"),
		al_awaddr("al_awaddr"),
		al_awprot("al_awprot"),

		al_wvalid("al_wvalid"),
		al_wready("al_wready"),
		al_wdata("al_wdata"),
		al_wstrb("al_wstrb"),

		al_bvalid("al_bvalid"),
		al_bready("al_bready"),
		al_bresp("al_bresp"),

		al_arvalid("al_arvalid"),
		al_arready("al_arready"),
		al_araddr("al_araddr"),
		al_arprot("al_arprot"),

		al_rvalid("al_rvalid"),
		al_rready("al_rready"),
		al_rdata("al_rdata"),
		al_rresp("al_rresp")
	{

		SC_METHOD(gen_rst_n);
		sensitive << rst;

		m_qk.set_global_quantum(quantum);

		zynq.rst(rst);

		bus   = new iconnect<NR_MASTERS, NR_DEVICES> ("bus");


		aes_device = new aes_ila_tlm("aes_ila_tlm", sc_time(1, SC_NS), 16*512);

		bus->memmap(0xa0460000ULL, 0x4000 - 1,
				ADDRMODE_RELATIVE, -1, aes_device->socket);
#ifdef HAVE_VERILOG
		tlm2axi_al = new tlm2axilite_bridge<4, 32> ("tlm2axi-al-bridge");
		// tlm2axi_af = new tlm2axi_bridge<10, AXIFULL_DATA_WIDTH> ("tlm2axi-af-bridge");
		cout << "map axilite" << endl;
		bus->memmap(0xa0450000ULL, 0x10 - 1,
				ADDRMODE_RELATIVE, -1, tlm2axi_al->tgt_socket);
		cout << "map axifull" << endl;
#else
		bus->memmap(0xa0450000ULL, 0x10 - 1,
				ADDRMODE_RELATIVE, -1, mem_al.socket);
		//bus->memmap(0xa0460000ULL, 0x400 - 1,
		//		ADDRMODE_RELATIVE, -1, mem_af.socket);
#endif

		bus->memmap(0x0LL, 0xffffffff - 1,
				ADDRMODE_RELATIVE, -1, *(zynq.s_axi_hpc_fpd[0]));

		zynq.s_axi_hpm_fpd[0]->bind(*(bus->t_sk[0]));


#ifdef HAVE_VERILOG
		/* Slow clock to keep simulation fast.  */
		clk = new sc_clock("clk", sc_time(10, SC_US));
#ifdef HAVE_VERILOG_VCS
#elif defined(HAVE_VERILOG_VERILATOR)
		al = new Vaxilite_dev_v1_0("axilite-dev");
		// af = new Vaxifull_AES("axifull-dev");
#endif

		aes_device->clk(*clk);
		al->s00_axi_aclk(*clk);
		al->s00_axi_aresetn(rst_n);

		al->s00_axi_awvalid(al_awvalid);
		al->s00_axi_awready(al_awready);
		al->s00_axi_awaddr(al_awaddr);
		al->s00_axi_awprot(al_awprot);

		al->s00_axi_wvalid(al_wvalid);
		al->s00_axi_wready(al_wready);
		al->s00_axi_wdata(al_wdata);
		al->s00_axi_wstrb(al_wstrb);

		al->s00_axi_bvalid(al_bvalid);
		al->s00_axi_bready(al_bready);
		al->s00_axi_bresp(al_bresp);

		al->s00_axi_arvalid(al_arvalid);
		al->s00_axi_arready(al_arready);
		al->s00_axi_araddr(al_araddr);
		al->s00_axi_arprot(al_arprot);

		al->s00_axi_rvalid(al_rvalid);
		al->s00_axi_rready(al_rready);
		al->s00_axi_rdata(al_rdata);
		al->s00_axi_rresp(al_rresp);

		tlm2axi_al->clk(*clk);

		tlm2axi_al->resetn(rst_n);

		tlm2axi_al->awvalid(al_awvalid);
		tlm2axi_al->awready(al_awready);
		tlm2axi_al->awaddr(al_awaddr);
		tlm2axi_al->awprot(al_awprot);

		tlm2axi_al->wvalid(al_wvalid);
		tlm2axi_al->wready(al_wready);
		tlm2axi_al->wdata(al_wdata);
		tlm2axi_al->wstrb(al_wstrb);

		tlm2axi_al->bvalid(al_bvalid);
		tlm2axi_al->bready(al_bready);
		tlm2axi_al->bresp(al_bresp);

		tlm2axi_al->arvalid(al_arvalid);
		tlm2axi_al->arready(al_arready);
		tlm2axi_al->araddr(al_araddr);
		tlm2axi_al->arprot(al_arprot);

		tlm2axi_al->rvalid(al_rvalid);
		tlm2axi_al->rready(al_rready);
		tlm2axi_al->rdata(al_rdata);
		tlm2axi_al->rresp(al_rresp);

		/* AXI FULL.  */
#else
		clk = new sc_clock("clk", sc_time(20, SC_US));
#endif

		zynq.tie_off();
	}

private:
	tlm_utils::tlm_quantumkeeper m_qk;
};

void usage(void)
{
	cout << "tlm socket-path sync-quantum-ns" << endl;
}

int sc_main(int argc, char* argv[])
{
	Top *top;
	uint64_t sync_quantum;
	sc_trace_file *trace_fp = NULL;

#if HAVE_VERILOG_VERILATOR
	Verilated::commandArgs(argc, argv);
#endif

	if (argc < 3) {
		sync_quantum = 10000;
	} else {
		sync_quantum = strtoull(argv[2], NULL, 10);
	}

	sc_set_time_resolution(1, SC_PS);

	top = new Top("top", argv[1], sc_time((double) sync_quantum, SC_NS));

	if (argc < 3) {
		sc_start(1, SC_PS);
		sc_stop();
		usage();
		exit(EXIT_FAILURE);
	}

	trace_fp = sc_create_vcd_trace_file("trace");
	trace(trace_fp, *top, top->name());
	/* Pull the reset signal.  */
	top->rst.write(true);
	sc_start(1, SC_US);
	top->rst.write(false);

	sc_start();
	if (trace_fp) {
		sc_close_vcd_trace_file(trace_fp);
	}
	return 0;
}
