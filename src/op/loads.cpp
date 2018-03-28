#include "loads.h"

#include "../cpu.h"
#include "../op.h"

#define LDx(reg) \
    unsigned int Op::ld##reg(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        cpu->getRegs()->a = Op::address(cpu, opcode->mode, operands); \
        Op::setNZFlags(cpu, cpu->getRegs()->a); \
        return 0; \
    }

LDx(a);
LDx(x);
LDx(y);