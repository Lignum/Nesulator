#include "ppu.h"

#include "utils.h"

const size_t PPU::SPRITE_SIZE = 0x4;
const size_t PPU::OBJECT_ATTRIBUTE_MEMORY_SIZE = 64 * SPRITE_SIZE;

PPU::PPU()
    : oam(OBJECT_ATTRIBUTE_MEMORY_SIZE, 0)
{
}

uint8_t PPU::readRegister(PPURegister reg) const {
    return 0;
}

void PPU::writeRegister(PPURegister reg, uint8_t value) {

}

Address PPU::getPPURegisterAddress(PPURegister reg) {
    if (reg == PPURegister::OAMDMA) {
        return 0x4014;
    } else {
        return (Address)reg + (Address)0x2000;
    }
}

bool PPU::getRegisterFromAddress(Address address, PPURegister *outReg) {
    if (!outReg) {
        return false;
    }

    if (Utils::inRange(address, 0x2000, 0x2007)) {
        *outReg = (PPURegister)(address - 0x2000);
        return true;
    } else if (address == 0x4014) {
        *outReg = PPURegister::OAMDMA;
        return true;
    }

    return false;
}
