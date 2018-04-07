#include "memory.h"

#include "nes.h"
#include "utils.h"

#include <iostream>

const size_t NES_INTERNAL_MEMORY_SIZE = 2048;
const size_t NES_INTERNAL_VIDEO_MEMORY_SIZE = 2048;
const size_t NES_PALETTE_RAM_SIZE = 32;
const size_t NES_PAGE_SIZE = 256;
const Address NES_STACK_ADDRESS = NES_PAGE_SIZE * 1;

Memory::Memory(NES *nes)
    : nes(nes),
      internalMem(NES_INTERNAL_MEMORY_SIZE, 0),
      internalVideoMem(NES_INTERNAL_VIDEO_MEMORY_SIZE, 0),
      paletteRAM(NES_PALETTE_RAM_SIZE, 0)
{
}

uint8_t Memory::read(Address address) const {
    if (Utils::inRange(address, 0x0000, 0x1FFF)) {
        return internalMem[address % 0x0800];
    } else if (Utils::inRange(address, 0x6000, 0xFFFF)) {
        Mapper *mapper = nes->getCartridge()->getMapper();

        if (mapper != nullptr) {
            return mapper->read(address);
        } else {
            std::cout << "Warning: Could not read from address $";
            Utils::writeHexToStream(std::cout, address);
            std::cout << ", because the cartridge has no mapper! Assuming $00.\n";
            return 0x00;
        }
    } else if (Utils::inRange(address, 0x3F00, 0x3FFF)) {
        return paletteRAM[(address - 0x3F00) % NES_PALETTE_RAM_SIZE];
    } else {
        std::cerr << "Could not read from unmapped address $";
        Utils::writeHexToStream(std::cerr, address);
        std::cerr << "!!! Assuming $00.\n";
        return 0x00;
    }
}

void Memory::write(Address address, uint8_t value) {
    if (Utils::inRange(address, 0x0000, 0x1FFF)) {
        internalMem[address % 0x0800] = value;
    } else if (Utils::inRange(address, 0x6000, 0xFFFF)) {
        Mapper *mapper = nes->getCartridge()->getMapper();

        if (mapper != nullptr) {
            mapper->write(address, value);
        } else {
            std::cout << "Warning: Could not write to address $";
            Utils::writeHexToStream(std::cout, address);
            std::cout << ", because the cartridge has no mapper!\n";
        }
    } else if (Utils::inRange(address, 0x3F00, 0x3FFF)) {
        paletteRAM[(address - 0x3F00) % NES_PALETTE_RAM_SIZE] = value;
    } else {
        std::cerr << "Could not write to unmapped address $";
        Utils::writeHexToStream(std::cerr, address);
        std::cerr << "!!!\n";
    }
}

Address Memory::getResetVector() const {
    return Utils::combineUint8sLE(read(0xFFFC), read(0xFFFD));
}

Address Memory::getIRQVector() const {
    return Utils::combineUint8sLE(read(0xFFFE), read(0xFFFF));
}

std::vector<uint8_t> *Memory::getInternalMemory() {
    return &internalMem;
}

std::vector<uint8_t> *Memory::getInternalVideoMemory() {
    return &internalVideoMem;
}

std::vector<uint8_t> *Memory::getPaletteRAM() {
    return &paletteRAM;
}
