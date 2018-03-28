#include "irq.h"

#include <iostream>

unsigned int Op::brk(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    std::cout << "Hit a BRK\n";
    return 0;
}
