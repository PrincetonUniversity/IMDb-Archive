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

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <inttypes.h>

#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "aes_ila_tlm.h"

using namespace sc_core;
using namespace std;

aes_ila_tlm::aes_ila_tlm(sc_module_name name, sc_time latency, int size_)
	: sc_module(name), socket("socket"), LATENCY(latency)
{
	socket.register_b_transport(this, &aes_ila_tlm::b_transport);
	aes_core = new AES("top");
        aes_core->AES_cmd_in(AES_cmd_in);
        aes_core->AES_cmdaddr_in(AES_cmd_addr_in);
        aes_core->AES_cmddata_in(AES_cmd_data_in);
        aes_core->AES_aes_status_out(AES_aes_status_out);
        aes_core->AES_aes_address_out(AES_aes_address_out);
        aes_core->AES_aes_length_out(AES_aes_length_out);
        aes_core->AES_aes_counter_out(AES_aes_counter_out);
        aes_core->AES_aes_key_out(AES_aes_key_out);
        aes_core->AES_outdata_out(AES_outdata_out);

	enc_time = 0;
	size = size_;
	memory = new uint8_t[size];
	memset(&memory[0], 0, size);
}

void aes_ila_tlm::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay)
{
	tlm::tlm_command cmd = trans.get_command();
	sc_dt::uint64    addr = trans.get_address();
	unsigned char*   ptr = trans.get_data_ptr();
	unsigned int     len = trans.get_data_length();
	unsigned char*   byt = trans.get_byte_enable_ptr();
	
	//char* key = "encrypt";
	char* dec_text = "Hardware-software (HW/SW) co-simulation is a critical part of Systems-on-Chip (SoC) design flow.";
        uint32_t enc_text[] = {141,178,233,50,176,43,150,63,202,146,52,12,223,241,54,91,9,202,5,32,165,249,108,164,115,188,196,146,46,219,25,160,171,36,188,157,61,179,9,220,72,18,29,211,54,96,240,71,143,181,11,188,220,158,161,242,128,103,193,229,94,19,123,254,160,134,104,255,69,130,87,111,43,235,60,147,199,62,101,102,0,148,186,107,120,156,161,182,176,167,221,44,121,57,3,181}; 

	if (addr > sc_dt::uint64(size)) {
		trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
		SC_REPORT_FATAL("Aes_axi_tlm", "Unsupported access\n");
		return;
	}
	if (byt != 0) {
		trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
		SC_REPORT_FATAL("Aes_axi_tlm", "Unsupported access\n");
		return;
	}
	unsigned int START_ADDR = 0;
	unsigned int LENGTH_ADDR = 1 * 4;
	unsigned int TEXT_ADDR = 2;
	unsigned int COUNTER_ADDR = 8 * 4;

	if (trans.get_command() == tlm::TLM_READ_COMMAND) {
		memcpy(ptr, &memory[addr], len);
	}
	else if (cmd == tlm::TLM_WRITE_COMMAND) {
		if (addr != START_ADDR)
		  memcpy(&memory[addr], ptr, len);
		if ((addr > START_ADDR) && (addr <= (COUNTER_ADDR + 15))) {
			AES_cmd_in.write(2);
			AES_cmd_data_in.write(memory[addr]);
			AES_cmd_addr_in.write(addr + 0xff00);
			wait(SC_ZERO_TIME);
		} else if (addr != START_ADDR) {
			aes_core->AES_XRAM[addr] = memory[addr];	
		} 
		if ((addr == START_ADDR) && (ptr[0] == 1)) {
			int encrypt_length = (memory[LENGTH_ADDR + 1] << 8) + memory[LENGTH_ADDR]; 
			int text_addr = (memory[TEXT_ADDR + 1] << 8) + memory[TEXT_ADDR];

			AES_cmd_in.write(2);
			AES_cmd_data_in.write(1);
			AES_cmd_addr_in.write(0xff00);
			wait(SC_ZERO_TIME);
			AES_cmd_in.write(0);
			AES_cmd_data_in.write(0);
	
			while(AES_aes_status_out.read().to_uint() != 0) {
				wait(SC_ZERO_TIME);
			}
		/// Inspection code start:
		/// Below is the inspection code to check whether the aes encryption 
		/// works correctly, by checking the encryption result vesus
		/// the golden result.
		/// This inspection should been done by the simulated software but 
		/// here it is part of the tlm model. This is because the current 
		/// simulated software is already a complex bash script, putting the
		/// inspection code here can relieve the complexity of the software.

			if (enc_time % 2 == 0) {
			cout << "---------- AES SIM START ----------" << endl;
			  bool error_flag = false;
			  for (int i = 0; i < 96; i++) {
			      if (aes_core->AES_XRAM[text_addr + i] != enc_text[i]) {
				error_flag = true;
				cout << hex << "[Error] sim: " << aes_core->AES_XRAM[text_addr + i] << "  enc_text: " << hex << enc_text[i] << endl;
			      }
			  }
			  if (!error_flag)
			    cout << "No Encryption Error!" << endl;
			} else {
			  bool error_flag = false;
			  for (int i = 0; i < 96; i++) {
			    if (aes_core->AES_XRAM[text_addr + i] != dec_text[i]) {
			      error_flag = true; 
			      cout << "[Error] sim: " << (char)(aes_core->AES_XRAM[text_addr+i].to_uint()) << "  plain_text: " << (char) dec_text[i] << endl;
			    }
			  }
			  if (!error_flag)
			    cout << "No Decryption Error!" << endl;
		 	  cout << endl << "decrypted result:" << endl;
			  for (int i = 0; i < encrypt_length; i++) {
			    cout << (char)(aes_core->AES_XRAM[text_addr + i].to_uint());
			  }
			  cout << endl;
			}
			enc_time++;
		}
	}

	delay += LATENCY;

	trans.set_dmi_allowed(true);
	trans.set_response_status(tlm::TLM_OK_RESPONSE);
}


unsigned int aes_ila_tlm::transport_dbg(tlm::tlm_generic_payload& trans)
{
	tlm::tlm_command cmd = trans.get_command();
	sc_dt::uint64    addr = trans.get_address();
	unsigned char*   ptr = trans.get_data_ptr();
	unsigned int     len = trans.get_data_length();
	unsigned int num_bytes = (len < (size - addr)) ? len : (size - addr);

	if (cmd == tlm::TLM_READ_COMMAND)
		memcpy(ptr, &memory[addr], num_bytes);
	else if ( cmd == tlm::TLM_WRITE_COMMAND )
		memcpy(&memory[addr], ptr, num_bytes);

	return num_bytes;
}
