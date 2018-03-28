#pragma once

#include "../op.h"

namespace Op {
    unsigned int clc(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int cld(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int cli(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int clv(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int sec(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int sed(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int sei(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);
}