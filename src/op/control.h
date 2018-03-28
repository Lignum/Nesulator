#pragma once

#include "../op.h"

namespace Op {
    unsigned int jmp(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int bcc(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int bcs(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int beq(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int bmi(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int bne(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int bpl(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int bvc(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int bvs(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);
}