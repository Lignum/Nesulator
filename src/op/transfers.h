#pragma once

#include "../op.h"

namespace Op {
    unsigned int tax(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int tay(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int tsx(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int txa(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int txs(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int tya(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);
}