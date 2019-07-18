import ila

from gb_arch import GBArch
from gb_rdi import RDI
from gb_u1 import U1
from gb_u2 import U2
from gb_u3 import U3
from gb_u4 import U4

def createILA ():
    gb = GBArch ()
    U1 (gb)
    U2 (gb)
    U3 (gb)
    U4 (gb)
    RDI (gb)

    gb.setNext ()
    verilogFile = '../archive/gb_low_vlog_rd.v'
    gb.exportVerilog (verilogFile)

if __name__ == '__main__':
    createILA ()
    
