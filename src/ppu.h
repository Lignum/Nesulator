#pragma once

#include "address.h"

#include <vector>
#include <cstdint>

enum class PPURegister : uint8_t {
    PPUCTRL = 0,
    PPUMASK = 1,
    PPUSTATUS = 2,
    OAMADDR = 3,
    OAMDATA = 4,
    PPUSCROLL = 5,
    PPUADDR = 6,
    PPUDATA = 7,
    OAMDMA = 8
};

class PPU {
public:
    PPU();

    void writeRegister(PPURegister reg, uint8_t value);

    uint8_t readRegister(PPURegister reg) const;

    static Address getPPURegisterAddress(PPURegister reg);

    static bool getRegisterFromAddress(Address address, PPURegister *outReg);

    static const size_t SPRITE_SIZE;
    static const size_t OBJECT_ATTRIBUTE_MEMORY_SIZE;

private:
    std::vector<uint8_t> oam;
};