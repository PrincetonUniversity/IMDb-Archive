import os
import ila

from gb_arch import GBArch
from gb_wri import WRI
from gb_rdi import RDI
from gb_u1 import U1
from gb_u2 import U2
from gb_u3 import U3
from gb_u4 import U4

def ExportHierIla():

    archive_dir = './archive'
    if not os.path.exists(archive_dir):
        os.makedirs(archive_dir)

    gb_top = GBArch();
    WRI(gb_top)
    RDI(gb_top)
    gb_top.setNext()
    gb_top.setDecode()

    gb_top.exportModel(archive_dir + '/gb_top.ila')

    gb_child = GBArch();
    U1(gb_child)
    U2(gb_child)
    U3(gb_child)
    U4(gb_child)
    gb_child.setNext()
    gb_child.setDecode()

    gb_child.exportModel(archive_dir + '/gb_child.ila')

if __name__ == '__main__':
    ExportHierIla()
