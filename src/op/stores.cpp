#include "stores.h"

#include "../cpu.h"
#include "../memory.h"
#include "../nes.h"
#include "../op.h"

#define ST(x) \
    unsigned int Op::st##x(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        Memory *mem = cpu->getNES()->getMemory(); \
        Address addr = Op::getAddress(cpu, opcode->mode, operands); \
        mem->writeCPU(addr, cpu->getRegs()->x); \
        return 0; \
    }

ST(a);
ST(x);
ST(y);