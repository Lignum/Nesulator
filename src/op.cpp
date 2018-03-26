#include "op.h"

#include "cpu.h"
#include "op/irq.h"

#include <vector>
#include <cstdio>

static unsigned int unimplemented(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    fprintf(stderr, "Opcode 0x%02X (%s) has not been implemented yet!\n", opcode->code, opcode->name);
    return 0;
}

static unsigned int unsupported(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    fprintf(stderr, "Opcode 0x%02X (%s) is not supported!\n", opcode->code, opcode->name);
    return 0;
}

using Opcode = Op::Opcode;
using AM = Op::AddressingMode;

#define OP(code, name, addressingMode, handler) Opcode { code, name, addressingMode, handler }
#define UNSUPPORTED_OP(code, name, addressingMode) Opcode { code, name, addressingMode, unsupported }

static const Opcode OPCODES[] = {
    OP(0x00, "BRK", AM::IMPLICIT, Op::brk),
    OP(0x01, "ORA", AM::INDEXED_INDIRECT, unimplemented),
    UNSUPPORTED_OP(0x02, "KIL", AM::IMPLICIT),
    UNSUPPORTED_OP(0x03, "SLO", AM::INDEXED_INDIRECT),
    UNSUPPORTED_OP(0x04, "NOP", AM::ZERO_PAGE),
    OP(0x05, "ORA", AM::ZERO_PAGE, unimplemented),
    OP(0x06, "ASL", AM::ZERO_PAGE, unimplemented),
    UNSUPPORTED_OP(0x07, "SLO", AM::ZERO_PAGE),
    OP(0x08, "PHP", AM::IMPLICIT, unimplemented),
    OP(0x09, "ORA", AM::IMMEDIATE, unimplemented),
    OP(0x0A, "ASL", AM::IMPLICIT, unimplemented),
    UNSUPPORTED_OP(0x0B, "ANC", AM::IMMEDIATE),
    UNSUPPORTED_OP(0x0C, "NOP", AM::ABSOLUTE),
    OP(0x0D, "ORA", AM::ABSOLUTE, unimplemented),
    OP(0x0E, "ASL", AM::ABSOLUTE, unimplemented),
    UNSUPPORTED_OP(0x0F, "SLO", AM::ABSOLUTE)
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