import math
# // DECLARE CONSTANTS
P_NBITS= 1                            #// Bit width of datapath
C_N_OFF = 32/P_NBITS                  #// Number of total n-bit chunks per word. 
C_OFFBITS= int( math.log(C_N_OFF,2) ) #// # of bits needed to index the n-bit chunks.
C_OFFBITS_STR= str(C_OFFBITS)
N_CTRL_SIGNALS = 49
