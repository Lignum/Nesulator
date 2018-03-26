#include "memory.h"

#include "nes.h"
#include "utils.h"

const size_t NES_INTERNAL_MEMORY_SIZE = 2048;

Memory::Memory(NES *nes)
    : nes(nes),
      internalMem(NES_INTERNAL_MEMORY_SIZE, 0)
{
}

uint8_t Memory::read(Address address) const {
    if (Utils::inRange(address, (Address)0x0000, (Address)0x1FFF)) {
        return internalMem[address % 0x0800];
    } else {
        // Pass call on to mapper.
    }
}

void Memory::write(Address address, uint8_t value) {
    if (Utils::inRange(address, (Address)0x0000, (Address)0x1FFF)) {
        internalMem[address % 0x0800] = value;
    } else {
        // Pass call on to mapper.
    }
}