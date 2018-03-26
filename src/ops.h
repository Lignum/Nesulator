#pragma once

#include <vector>
#include <cstdint>

class CPU;

namespace Op {
    typedef void (*Handler)(CPU *cpu, const std::vector<uint8_t> &operands);

    struct Opcode {
        const char name[4];
        size_t operands;
        Handler handler;
    };

    const Opcode *decode(uint8_t op);
}