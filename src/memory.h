#pragma once

#include "address.h"

#include <vector>
#include <cstdint>

extern const size_t NES_INTERNAL_MEMORY_SIZE;
extern const size_t NES_INTERNAL_VIDEO_MEMORY_SIZE;
extern const size_t NES_PALETTE_RAM_SIZE;
extern const size_t NES_PAGE_SIZE;
extern const Address NES_STACK_ADDRESS;

class NES;

class Memory {
public:
    explicit Memory(NES *nes);

    uint8_t read(Address address) const;

    void write(Address address, uint8_t value);

    Address getResetVector() const;

    Address getIRQVector() const;

    std::vector<uint8_t> *getInternalMemory();

    std::vector<uint8_t> *getInternalVideoMemory();

    std::vector<uint8_t> *getPaletteRAM();

private:
    NES *nes;
    std::vector<uint8_t> internalMem;
    std::vector<uint8_t> internalVideoMem;
    std::vector<uint8_t> paletteRAM;
};