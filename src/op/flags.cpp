#include "flags.h"

#include "../cpu.h"

#define CL(x, flag) \
    unsigned int Op::cl##x(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        cpu->setFlag(flag, false); \
        return 0; \
    }

#define SE(x, flag) \
    unsigned int Op::se##x(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        cpu->setFlag(flag, true); \
        return 0; \
    }

CL(c, CPUFlag::CARRY);
CL(d, CPUFlag::DECIMAL_MODE);
CL(i, CPUFlag::IRQ_DISABLE);
CL(v, CPUFlag::OVER_FLOW);

SE(c, CPUFlag::CARRY);
SE(d, CPUFlag::DECIMAL_MODE);
SE(i, CPUFlag::IRQ_DISABLE);