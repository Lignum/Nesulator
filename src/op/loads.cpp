#include "loads.h"

#include "../cpu.h"
#include "../op.h"

unsigned int Op::lda(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    cpu->getRegs()->a = Op::address(cpu, opcode->mode, operands);
    Op::setNZFlags(cpu, cpu->getRegs()->a);
    return 0;
}

unsigned int Op::ldx(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    cpu->getRegs()->x = Op::address(cpu, opcode->mode, operands);
    Op::setNZFlags(cpu, cpu->getRegs()->x);
    return 0;
}

unsigned int Op::ldy(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    cpu->getRegs()->y = Op::address(cpu, opcode->mode, operands);
    Op::setNZFlags(cpu, cpu->getRegs()->y);
    return 0;
}
