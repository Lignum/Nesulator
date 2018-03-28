#pragma once

#include "../op.h"

namespace Op {
    unsigned int brk(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int rti(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);
}