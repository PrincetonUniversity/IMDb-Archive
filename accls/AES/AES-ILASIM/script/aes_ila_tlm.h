/*
 * Copyright (c) 2011 Edgar E. Iglesias.
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
#include "AES.h"
#include "systemc.h"
class aes_ila_tlm
: public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket<aes_ila_tlm> socket;

	const sc_time LATENCY;

	aes_ila_tlm(sc_core::sc_module_name name, sc_time latency, int size_);
	virtual void b_transport(tlm::tlm_generic_payload& trans,
					sc_time& delay);
	virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

private:
        AES *aes_core;

  	sc_signal<sc_biguint<2> > AES_cmd_in;
  	sc_signal<sc_biguint<16> > AES_cmd_addr_in;
  	sc_signal<sc_biguint<8> > AES_cmd_data_in; 
  	sc_signal<sc_biguint<2> > AES_aes_status_out;
  	sc_signal<sc_biguint<16> > AES_aes_address_out;
  	sc_signal<sc_biguint<16> > AES_aes_length_out;
  	sc_signal<sc_biguint<128> > AES_aes_counter_out;
  	sc_signal<sc_biguint<128> > AES_aes_key_out;
  	sc_signal<sc_biguint<8> > AES_outdata_out;
	int enc_time;

	uint8_t *memory;
	int size;
};
