#pragma once

#include "../op.h"

namespace Op {
    unsigned int sta(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int stx(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int sty(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);
}