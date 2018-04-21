#pragma once

#include "address.h"
#include "memory.h"

#include <cstdint>
#include <string>
#include <memory>

class NES;

extern const size_t NES_NAMETABLE_SIZE;

class Mapper {
public:
    Mapper(NES *nes, uint8_t id, const std::string &name);

    uint8_t read(MemoryAccessSource source, Address address);

    void write(MemoryAccessSource source, Address address, uint8_t value);

    virtual uint8_t readCPU(Address address) = 0;

    virtual uint8_t readPPU(Address address) = 0;

    virtual void writeCPU(Address address, uint8_t value) = 0;

    virtual void writePPU(Address address, uint8_t value) = 0;

    virtual void step() = 0;

    NES *getNES();

    uint8_t getID() const;

    const std::string *getName() const;

protected:
    uint8_t basicNametableRead(Address address);

    void basicNametableWrite(Address address, uint8_t value);

    uint8_t basicPatternTableRead(Address address);

    void basicPatternTableWrite(Address address, uint8_t value);

    uint8_t basicPPURead(Address address);

    void basicPPUWrite(Address address, uint8_t value);

    NES *nes;

private:
    const uint8_t id;
    const std::string name;
};