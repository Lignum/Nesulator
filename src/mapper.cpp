#include "mapper.h"

#include "cartridge.h"
#include "nes.h"
#include "utils.h"

#include <iostream>

const size_t NES_NAMETABLE_SIZE = 1024;

Mapper::Mapper(NES *nes, uint8_t id, const std::string &name)
    : nes(nes),
      id(id),
      name(name)
{
}

NES *Mapper::getNES() {
    return nes;
}

uint8_t Mapper::getID() const {
    return id;
}

const std::string *Mapper::getName() const {
    return &name;
}

uint8_t Mapper::basicNametableRead(Address address) {
    const Cartridge *cartridge = nes->getCartridge();
    const auto *vram = nes->getMemory()->getInternalVideoMemory();

    switch (cartridge->getMirroring()) {
        case Mirroring::HORIZONTAL:
            if (Utils::inRange(address, 0x2000, 0x27FF)) {
                return vram->at((size_t)(address - 0x2000) % NES_NAMETABLE_SIZE);
            } else if (Utils::inRange(address, 0x2800, 0x2FFF)) {
                return vram->at((size_t)(address - 0x2000) % NES_NAMETABLE_SIZE + NES_NAMETABLE_SIZE * 2);
            }

        case Mirroring::VERTICAL:
            if (Utils::inRange(address, 0x2000, 0x23FF) || Utils::inRange(address, 0x2800, 0x2BFF)) {
                return vram->at((size_t)(address - 0x2000) % NES_NAMETABLE_SIZE);
            } else if (Utils::inRange(address, 0x2400, 0x27FF) || Utils::inRange(address, 0x2C00, 0x2FFF)) {
                return vram->at((size_t)(address - 0x2000) % NES_NAMETABLE_SIZE + NES_NAMETABLE_SIZE * 2);
            }

        case Mirroring::FOUR_SCREEN:
            std::cout << "Warning: This mapper does not support four-screen mode, yet the cartridge requires it! Assuming $00.\n";
            return 0;
    }
}

void Mapper::basicNametableWrite(Address address, uint8_t value) {
    Cartridge *cartridge = nes->getCartridge();
    auto *vram = nes->getMemory()->getInternalVideoMemory();

    switch (cartridge->getMirroring()) {
        case Mirroring::HORIZONTAL:
            if (Utils::inRange(address, 0x2000, 0x27FF)) {
                vram->at((size_t)(address - 0x2000) % NES_NAMETABLE_SIZE) = value;
            } else if (Utils::inRange(address, 0x2800, 0x2FFF)) {
                vram->at((size_t)(address - 0x2000) % NES_NAMETABLE_SIZE + NES_NAMETABLE_SIZE * 2) = value;
            }
            break;

        case Mirroring::VERTICAL:
            if (Utils::inRange(address, 0x2000, 0x23FF) || Utils::inRange(address, 0x2800, 0x2BFF)) {
                vram->at((size_t)(address - 0x2000) % NES_NAMETABLE_SIZE) = value;
            } else if (Utils::inRange(address, 0x2400, 0x27FF) || Utils::inRange(address, 0x2C00, 0x2FFF)) {
                vram->at((size_t)(address - 0x2000) % NES_NAMETABLE_SIZE + NES_NAMETABLE_SIZE * 2) = value;
            }
            break;

        case Mirroring::FOUR_SCREEN:
            std::cout << "Warning: This mapper does not support four-screen mode, yet the cartridge requires it!\n";
            break;
    }
}

uint8_t Mapper::basicPatternTableRead(Address address) {
    Cartridge *cartridge = nes->getCartridge();
    auto *chr = cartridge->getCHR();

    if (Utils::inRange(address, 0x0000, 0x1FFF)) {
        return chr->at(address);
    }
}

void Mapper::basicPatternTableWrite(Address address, uint8_t value) {
    Cartridge *cartridge = nes->getCartridge();
    auto *chr = cartridge->getCHR();

    if (Utils::inRange(address, 0x0000, 0x1FFF)) {
        chr->at(address) = value;
    }
}

uint8_t Mapper::basicPPURead(Address address) {
    if (Utils::inRange(address, 0x0000, 0x1FFF)) {
        return basicPatternTableRead(address);
    } else if (Utils::inRange(address, 0x2000, 0x2FFF)) {
        return basicNametableRead(address);
    }
}

void Mapper::basicPPUWrite(Address address, uint8_t value) {
    if (Utils::inRange(address, 0x0000, 0x1FFF)) {
        basicPatternTableWrite(address, value);
    } else if (Utils::inRange(address, 0x2000, 0x2FFF)) {
        basicPatternTableWrite(address, value);
    }
}

uint8_t Mapper::read(MemoryAccessSource source, Address address) {
    switch (source) {
        case MemoryAccessSource::CPU: return readCPU(address);
        case MemoryAccessSource::PPU: return readPPU(address);
    }
}

void Mapper::write(MemoryAccessSource source, Address address, uint8_t value) {
    switch (source) {
        case MemoryAccessSource::CPU: return writeCPU(address, value);
        case MemoryAccessSource::PPU: return writePPU(address, value);
    }
}
