#include "stores.h"

#include "../cpu.h"
#include "../memory.h"
#include "../nes.h"
#include "../op.h"

#define STx(reg) \
    unsigned int Op::st##reg(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        Memory *mem = cpu->getNES()->getMemory(); \
        Address addr = Op::getAddress(cpu, opcode->mode, operands); \
        mem->write(addr, cpu->getRegs()->reg); \
        return 0; \
    }

STx(a);
STx(x);
STx(y);