#pragma once

#include <vector>
#include <string>
#include <cstdint>

class CPU;

namespace Op {
    struct Opcode;

    typedef const std::vector<uint8_t> Operands;
    typedef unsigned int (*Handler)(CPU *cpu, Operands &operands, const Opcode *opcode);

    enum class AddressingMode: uint8_t {
        IMPLICIT,
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

    uint8_t address(CPU *cpu, AddressingMode mode, const Operands &operands);

    void setNegativeFlag(CPU *cpu, uint8_t value);

    void setZeroFlag(CPU *cpu, uint8_t value);

    void setNZFlags(CPU *cpu, uint8_t value);

    void formatInstruction(const Opcode *opcode, const Operands &operands, std::string *outFormatted);

    struct Opcode {
        uint8_t code;
        unsigned int baseCycles;
        const char name[4];
        AddressingMode mode;
        Handler handler;
    };

    const Opcode *decode(uint8_t op);
}