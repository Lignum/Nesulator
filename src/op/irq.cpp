#include "irq.h"

#include <cstdio>

unsigned int Op::brk(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    printf("Hit a BRK\n");
    return 7;
}
