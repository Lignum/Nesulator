#pragma once

#include "../op.h"

namespace Op {
    unsigned int asl(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int lsr(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int rol(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int ror(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int inc(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int inx(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int iny(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int dec(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int dex(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);

    unsigned int dey(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);
}