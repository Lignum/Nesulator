#pragma once

#include "address.h"

#include <cstdint>
#include <string>
#include <memory>

class NES;

class Mapper {
public:
    Mapper(NES *nes, uint8_t id, const std::string &name);

    virtual uint8_t read(Address address) = 0;

    virtual void write(Address address, uint8_t value) = 0;

    virtual void step() = 0;

    NES *getNES();

    uint8_t getID() const;

    const std::string *getName() const;

protected:
    NES *nes;

private:
    const uint8_t id;
    const std::string name;
};