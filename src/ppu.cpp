#include "ppu.h"

#include "utils.h"
#include "nes.h"
#include "memory.h"

#include <iostream>
#include <algorithm>

const size_t PPU::SPRITE_SIZE = 0x4;
const size_t PPU::OBJECT_ATTRIBUTE_MEMORY_SIZE = 64 * SPRITE_SIZE;

PPU::PPU(NES *nes)
    : nes(nes),
      controlFlags((PPUControlFlag)0x00),
      maskFlags((PPUMaskFlag)0x00),
      statusFlags((PPUStatusFlag)0x00),
      ppuLatch(0x00),
      addressLatch(false),
      scrollX(0), scrollY(0),
      address(0x0000),
      oam(OBJECT_ATTRIBUTE_MEMORY_SIZE, 0x00)
{
}

void PPU::writeRegister(PPURegister reg, uint8_t value) {
    ppuLatch = value;

    switch (reg) {
        case PPURegister::PPUCTRL:
            controlFlags = (PPUControlFlag)value;
            break;

        case PPURegister::PPUMASK:
            maskFlags = (PPUMaskFlag)value;
            break;

        case PPURegister::PPUSCROLL:
            if (!addressLatch) {
                // First write
                scrollX = value;
            } else {
                // Second write
                scrollY = value;
            }

            addressLatch = !addressLatch;
            break;

        case PPURegister::PPUADDR:
            if (!addressLatch) {
                // First write
                address = (uint16_t)value << 8;
            } else {
                // Second write
                address |= value;
            }

            addressLatch = !addressLatch;
            break;

        case PPURegister::PPUDATA:
            nes->getMemory()->writePPU(address, value);
            incrementAddress();
            break;

        case PPURegister::OAMADDR:
            oamAddress = value;
            break;

        case PPURegister::OAMDATA:
            oam[oamAddress++] = value;
            break;

        case PPURegister::OAMDMA: {
            const Memory *mem = nes->getMemory();
            const Address start = (Address)value << 8;

            for (Address i = 0; i < oam.size(); i++) {
                oam[i] = mem->readPPU(i + start);
            }

            break;
        }

        case PPURegister::PPUSTATUS:
            break;
    }
}

uint8_t PPU::readRegister(PPURegister reg) {
    switch (reg) {
        case PPURegister::PPUSTATUS:
            return (uint8_t)statusFlags | (ppuLatch & (uint8_t)0b00011111);

        case PPURegister::PPUDATA:
            incrementAddress();
            return nes->getMemory()->readPPU(address);

        case PPURegister::OAMDATA:
            return oam[oamAddress++];

        // "Write-only" registers.
        case PPURegister::PPUCTRL:
        case PPURegister::PPUMASK:
        case PPURegister::OAMADDR:
        case PPURegister::PPUSCROLL:
        case PPURegister::PPUADDR:
        case PPURegister::OAMDMA:
            return ppuLatch;
    }

	return 0;
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

bool PPU::isControlFlagSet(PPUControlFlag flag) const {
    return Utils::isFlagSet8(controlFlags, flag);
}

void PPU::setControlFlag(PPUControlFlag flag, bool set) {
    controlFlags = Utils::setFlag8(controlFlags, flag, set);
}

bool PPU::isMaskFlagSet(PPUMaskFlag flag) const {
    return Utils::isFlagSet8(maskFlags, flag);
}

void PPU::setMaskFlag(PPUMaskFlag flag, bool set) {
    maskFlags = Utils::setFlag8(maskFlags, flag, set);
}

bool PPU::isStatusFlagSet(PPUStatusFlag flag) const {
    return Utils::isFlagSet8(statusFlags, flag);
}

void PPU::setStatusFlag(PPUStatusFlag flag, bool set) {
    statusFlags = Utils::setFlag8(statusFlags, flag, set);
}

void PPU::incrementAddress() {
    if (isControlFlagSet(PPUControlFlag::INCREMENT_MODE)) {
        address += 32;
    } else {
        address++;
    }
}

