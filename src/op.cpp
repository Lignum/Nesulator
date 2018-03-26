#include "op.h"

#include "cpu.h"
#include "op/irq.h"

#include <vector>
#include <cstdio>

static unsigned int invalid_op(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    fprintf(stderr, "Invalid opcode 0x%02X!\n", opcode->code);
    return 0;
}

static unsigned int unimplemented(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    fprintf(stderr, "Opcode 0x%02X is not supported!\n", opcode->code);
    return 0;
}

using Opcode = Op::Opcode;
using AM = Op::AddressingMode;

#define OP(code, name, addressingMode, handler) Opcode { code, name, addressingMode, handler }
#define INVALID_OP(code) Opcode { code, "N/A", AM::IMPLICIT, invalid_op }

static const Opcode OPCODES[] = {
    OP(0x00, "BRK", AM::IMPLICIT, Op::brk),
    OP(0x01, "ORA", AM::INDEXED_INDIRECT, unimplemented),
    INVALID_OP(0x02),
    INVALID_OP(0x03),
    INVALID_OP(0x04),
    OP(0x05, "ORA", AM::ZERO_PAGE, unimplemented),
    OP(0x06, "ASL", AM::ZERO_PAGE, unimplemented)
};

const Opcode *Op::decode(uint8_t op) {
    return op < sizeof(OPCODES) / sizeof(Opcode) ? &OPCODES[op] : nullptr;
}

size_t Op::getAddressingModeOperandCount(Op::AddressingMode mode) {
    switch (mode) {
        case AM::IMPLICIT:
        case AM::ACCUMULATOR:
            return 0;

        case AM::IMMEDIATE:
        case AM::ZERO_PAGE:
        case AM::ZERO_PAGE_X:
        case AM::ZERO_PAGE_Y:
        case AM::INDIRECT_INDEXED:
        case AM::INDEXED_INDIRECT:
        case AM::RELATIVE:
            return 1;

        case AM::ABSOLUTE:
        case AM::ABSOLUTE_X:
        case AM::ABSOLUTE_Y:
        case AM::INDIRECT:
            return 2;
    }
}