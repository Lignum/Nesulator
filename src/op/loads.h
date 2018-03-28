#pragma once

#include "../op.h"

namespace Op {
    unsigned int lda(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int ldx(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int ldy(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);
}