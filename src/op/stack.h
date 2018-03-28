#pragma once

#include "../op.h"

namespace Op {
    unsigned int pha(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int php(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int pla(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int plp(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);
}