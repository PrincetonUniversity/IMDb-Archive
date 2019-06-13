#!/usr/bin/python3

import argparse
from enum import Enum
from string import Template


class SpecState(Enum):
    ADDR = 1
    NAME = 2
    DESP = 3


def ParseMmioText(in_file):
    state = SpecState.ADDR
    result = []

    with open(in_file, 'r') as fr:
        for line in fr:
            if state is SpecState.ADDR:
                terms = (line.strip('\n')).split('_')
                if len(terms) == 2:
                    addr = terms[1]
                    try:
                        int(addr, 16)
                    except Exception:
                        print('Fail handling address value', addr)
                    state = SpecState.NAME

                else:
                    state = SpecState.ADDR

            elif state is SpecState.NAME:
                name = line.strip('\n')

                if name == 'Reserved':
                    state = SpecState.ADDR
                else:
                    state = SpecState.DESP

            else:
                desp = line.strip('\n')
                state = SpecState.ADDR

                pair = {}
                pair['name'] = name
                pair['addr'] = addr
                pair['desp'] = desp
                result.append(pair)

    return result


def GenMacro(pairs, out_file):
    with open(out_file, 'w') as fw:
        for p in pairs:
            desp = p['desp']
            name = p['name']
            addr = p['addr']

            fw.write('// {0}\n'.format(desp))
            fw.write('#define {0} "{1}"\n'.format(name.upper(), name))
            fw.write('#define {0}_BWID 32\n'.format(name.upper()))
            fw.write('#define {0}_ADDR 0x{1}\n'.format(name.upper(), addr))
            fw.write('\n')


def GenSetup(pairs, out_file):
    with open(out_file, 'w') as fw:
        for p in pairs:
            desp = p['desp']
            name = p['name']

            fw.write('// {0}\n'.format(desp))
            fw.write('NewState(m, {0}, {0}_BWID);\n'.format(name.upper()))
            fw.write('\n')


def GenReadInstr(pairs, out_file):
    with open('misc/reg_read_instr.in', 'r') as fr:
        template_raw = fr.read()
        template = Template(template_raw)

    with open(out_file, 'w') as fw:
        for p in pairs:
            name = p['name']
            addr = p['addr']

            fw.write('\n')

            d = {}
            d['comment'] = 'read register {0}'.format(name.upper())
            d['instr_name'] = 'REG_RD_{0}'.format(name.upper())
            d['mmio_addr'] = '{0}_ADDR'.format(name.upper())
            d['reg_name'] = name.upper()

            instr_reg_rd = template.safe_substitute(d)
            fw.write(instr_reg_rd)

            fw.write('\n')


def GenVarMap(pairs, out_file):
    with open(out_file, 'w') as fw:
        for p in pairs:
            name = p['name']
            fw.write('"{0}" : "m1.{0}",\n'.format(name.upper()))

def GenInstrCond(pairs, out_file):
    with open('misc/reg_read_cond.in', 'r') as fr:
        template_raw = fr.read()
        template = Template(template_raw)

    with open(out_file, 'w') as fw:
        for p in pairs:
            name = p['name']

            fw.write('\n')

            d = {}
            d['instr_name'] = 'REG_RD_{0}'.format(name.upper())

            instr_reg_rd = template.safe_substitute(d)
            fw.write(instr_reg_rd)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Generate macro from MMIO list text')
    parser.add_argument('mmio_file', type=str, help='mmio mapping file')
    parser.add_argument('out_file', type=str, help='output file name')
    parser.add_argument('--macro', action='store_true', help='generate macro')
    parser.add_argument('--setup', action='store_true', help='geenrate setup')
    parser.add_argument('--instr', action='store_true', help='generate instr')
    parser.add_argument('--var', action='store_true', help='generate var map')
    parser.add_argument('--cond', action='store_true', help='generate instr cond')
    args = parser.parse_args()

    pairs = ParseMmioText(args.mmio_file)

    if args.macro:
        GenMacro(pairs, args.out_file)

    if args.setup:
        GenSetup(pairs, args.out_file)

    if args.instr:
        GenReadInstr(pairs, args.out_file)

    if args.var:
        GenVarMap(pairs, args.out_file)

    if args.cond:
        GenInstrCond(pairs, args.out_file)
