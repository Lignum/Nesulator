#pragma once

#include "../mapper.h"

class NROM : public Mapper {
public:
    NROM(NES *nes);

    uint8_t read(Address address) override;

    void write(Address address, uint8_t value) override;

    void step();

private:

};
