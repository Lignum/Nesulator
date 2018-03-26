#pragma once

#include "../op.h"

#include <vector>
#include <cstdint>

namespace Op {
    unsigned int brk(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode);
}