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

uint8_t Memory::read(MemoryAccessSource source, Address address) const {
    switch (source) {
        case MemoryAccessSource::CPU: return readCPU(address);
        case MemoryAccessSource::PPU: return readPPU(address);
    }

}

void Memory::write(MemoryAccessSource source, Address address, uint8_t value) {
    switch (source) {
        case MemoryAccessSource::CPU: return writeCPU(address, value);
        case MemoryAccessSource::PPU: return writeCPU(address, value);
    }
}

uint8_t Memory::readCPU(Address address) const {
    if (Utils::inRange(address, 0x0000, 0x1FFF)) {
        return internalMem[address % 0x0800];
    } else if (Utils::inRange(address, 0x4020, 0xFFFF)) {
        Mapper *mapper = nes->getCartridge()->getMapper();

        if (mapper != nullptr) {
            return mapper->readCPU(address);
        } else {
            std::cout << "Warning: Could not read from CPU address $";
            Utils::writeHexToStream(std::cout, address);
            std::cout << ", because the cartridge has no mapper! Assuming $00.\n";
            return 0x00;
        }
    } else if (Utils::inRange(address, 0x3F00, 0x3FFF)) {
        return paletteRAM[(address - 0x3F00) % NES_PALETTE_RAM_SIZE];
    } else {
        std::cerr << "Could not read from unmapped CPU address $";
        Utils::writeHexToStream(std::cerr, address);
        std::cerr << "!!! Assuming $00.\n";
        return 0x00;
    }
}

uint8_t Memory::readPPU(Address address) const {
    if (Utils::inRange(address, 0x3F00, 0x3FFF)) {
        return paletteRAM[(address - 0x3F00) % NES_PALETTE_RAM_SIZE];
    } else {
        Mapper *mapper = nes->getCartridge()->getMapper();

        if (mapper != nullptr) {
            if (Utils::inRange(address, 0x3000, 0x3EFF)) {
                // This is a mirror of $2000-$23FF, it would be a nuisance to implement this in every mapper.
                address %= 0x0F00;
            }

            return mapper->readPPU(address);
        } else {
            std::cerr << "Could not read from unmapped PPU address $";
            Utils::writeHexToStream(std::cerr, address);
            std::cerr << "!!! Assuming $00.\n";
            return 0x00;
        }
    }
}

void Memory::writeCPU(Address address, uint8_t value) {
    if (Utils::inRange(address, 0x0000, 0x1FFF)) {
        internalMem[address % 0x0800] = value;
    } else if (Utils::inRange(address, 0x4020, 0xFFFF)) {
        Mapper *mapper = nes->getCartridge()->getMapper();

        if (mapper != nullptr) {
            mapper->writeCPU(address, value);
        } else {
            std::cout << "Warning: Could not write to CPU address $";
            Utils::writeHexToStream(std::cout, address);
            std::cout << ", because the cartridge has no mapper!\n";
        }
    } else {
        std::cerr << "Could not write to unmapped CPU address $";
        Utils::writeHexToStream(std::cerr, address);
        std::cerr << "!!!\n";
    }
}

void Memory::writePPU(Address address, uint8_t value) {
    if (Utils::inRange(address, 0x3F00, 0x3FFF)) {
        paletteRAM[(address - 0x3F00) % NES_PALETTE_RAM_SIZE] = value;
    } else {
        Mapper *mapper = nes->getCartridge()->getMapper();

        if (mapper != nullptr) {
            if (Utils::inRange(address, 0x3000, 0x3EFF)) {
                // This is a mirror of $2000-$23FF, it would be a nuisance to implement this in every mapper.
                address %= 0x0F00;
            }

            mapper->writePPU(address, value);
        } else {
            std::cout << "Warning: Could not write to CPU address $";
            Utils::writeHexToStream(std::cout, address);
            std::cout << ", because the cartridge has no mapper!\n";
        }
    }
}

Address Memory::getResetVector() const {
    return Utils::combineUint8sLE(readCPU(0xFFFC), readCPU(0xFFFD));
}

Address Memory::getIRQVector() const {
    return Utils::combineUint8sLE(readCPU(0xFFFE), readCPU(0xFFFF));
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
