#include <aes_128.h>
#include <aes_ila.h>
#include <ilang/aqed-out/aqed_out.h>


/// Build the model
int main(int argc, char **argv) {

  SetUnsignedComparison(true); 
  // build the aes model
  AES aes_ila_model;

  AQedInfoGenerator aqed_out(aes_ila_model.model.get());
  aqed_out.ExportInstructionAndDecode("../aqed/decode.v");

  return 0;
}


