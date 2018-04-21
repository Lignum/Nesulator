#pragma once

#include "../mapper.h"

class NROM : public Mapper {
public:
    NROM(NES *nes);

    uint8_t readCPU(Address address) override;

    uint8_t readPPU(Address address) override;

    void writeCPU(Address address, uint8_t value) override;

    void writePPU(Address address, uint8_t value) override;

    void step();

private:

};
