#include "transfers.h"

#include "../cpu.h"
#include "../op.h"

#define T(x, y) \
    unsigned int Op::t##x##y(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        RegisterFile *r = cpu->getRegs(); \
        r->y = r->x; \
        Op::setNZFlags(cpu, r->y); \
        return 0; \
    }

T(a, x);
T(a, y);
T(s, x);
T(x, a);
T(x, s);
T(y, a);