#pragma once

#include "address.h"

#include <vector>
#include <cstdint>

extern const size_t NES_INTERNAL_MEMORY_SIZE;

class NES;

class Memory {
public:
    explicit Memory(NES *nes);

    uint8_t read(Address address) const;

    void write(Address address, uint8_t value);

private:
    NES *nes;
    std::vector<uint8_t> internalMem;
};