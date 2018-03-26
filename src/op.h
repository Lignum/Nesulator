#pragma once

#include <vector>
#include <cstdint>

class CPU;

namespace Op {
    struct Opcode;

    typedef const std::vector<uint8_t> Operands;
    typedef unsigned int (*Handler)(CPU *cpu, Operands &operands, const Opcode *opcode);

    enum class AddressingMode {
        IMPLICIT,
        ACCUMULATOR,
        IMMEDIATE,
        ZERO_PAGE,
        ZERO_PAGE_X,
        ZERO_PAGE_Y,
        RELATIVE,
        ABSOLUTE,
        ABSOLUTE_X,
        ABSOLUTE_Y,
        INDIRECT,
        INDEXED_INDIRECT,
        INDIRECT_INDEXED
    };

    size_t getAddressingModeOperandCount(AddressingMode mode);

    struct Opcode {
        uint8_t code;
        const char name[4];
        AddressingMode mode;
        Handler handler;
    };

    const Opcode *decode(uint8_t op);
}