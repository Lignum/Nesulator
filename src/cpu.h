#pragma once

#include "address.h"

#include <cstdint>
#include <cstdlib>
#include <vector>

enum class CPUFlag: uint8_t {
    CARRY           = 1 << 0,
    ZERO            = 1 << 1,
    IRQ_DISABLE     = 1 << 2,
    DECIMAL_MODE    = 1 << 3,
    BREAK           = 1 << 4,
    UNUSED          = 1 << 5,
    OVERFLOW        = 1 << 6,
    NEGATIVE        = 1 << 7
};

struct RegisterFile {
    uint8_t a;      // Accumulator
    uint8_t x;      // X Index Register
    uint8_t y;      // Y Index Register
    CPUFlag p;      // Processor Status (Flags)
    uint8_t s;      // Stack Pointer
    uint16_t pc;    // Program Counter
};

class NES;

class CPU {
public:
    explicit CPU(NES *nes);

    NES *getNES();

    RegisterFile *getRegs();

    bool isFlagSet(CPUFlag flag) const;

    void setFlag(CPUFlag flag, bool set);

    void jump(Address address);

    void push(uint8_t value);

    uint8_t pull();

    unsigned int step();

    void printState() const;

private:
    NES *nes;
    RegisterFile r;

    uint8_t fetch();

    void fetchOperands(size_t count, std::vector<uint8_t> &operands);
};