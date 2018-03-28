#include "stack.h"

#include "../cpu.h"
#include "../op.h"

unsigned int Op::pha(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    cpu->push(cpu->getRegs()->a);
    return 0;
}

unsigned int Op::php(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    cpu->push((uint8_t)cpu->getRegs()->p);
    return 0;
}

unsigned int Op::pla(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    uint8_t a = cpu->pull();
    cpu->getRegs()->a = a;
    Op::setNZFlags(cpu, a);
    return 0;
}

unsigned int Op::plp(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    cpu->getRegs()->p = (CPUFlag)cpu->pull();
    return 0;
}
