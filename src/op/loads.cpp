#include "loads.h"

#include "../cpu.h"
#include "../op.h"

#define LD(x) \
    unsigned int Op::ld##x(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        cpu->getRegs()->x = Op::address(cpu, opcode->mode, operands); \
        Op::setNZFlags(cpu, cpu->getRegs()->x); \
        return 0; \
    }

LD(a);
LD(x);
LD(y);