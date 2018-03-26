#include "ops.h"

#include "cpu.h"
#include "ops/irq.h"

#include <vector>
#include <cstdio>

static void invalid_op(CPU *cpu, const std::vector<uint8_t> &operands) {
    fprintf(stderr, "Invalid opcode!\n");
}

using Opcode = Op::Opcode;

static const Opcode INVALID_OPCODE = { "N/A", 0, invalid_op };

static const Opcode OPCODES[] = {
    Opcode { "BRK", 0, Op::brk }, // 0x00
};

const Opcode *Op::decode(uint8_t op) {
    return op < sizeof(OPCODES) / sizeof(Opcode) ? &OPCODES[op] : &INVALID_OPCODE;
}