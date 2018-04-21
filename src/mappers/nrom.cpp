#include "nrom.h"

#include "../mapper.h"
#include "../utils.h"
#include "../nes.h"
#include "../cartridge.h"

NROM::NROM(NES *nes) : Mapper(nes, 0, "NROM") {

}

uint8_t NROM::readCPU(Address address) {
    Cartridge *cartridge = nes->getCartridge();
    auto prgram = cartridge->getPRGRAM();
    auto prgrom = cartridge->getPRGROM();

    if (Utils::inRange(address, 0x6000, 0x7FFF)) {
        return prgram->at((size_t)(address - 0x6000));
    } else if (Utils::inRange(address, 0x8000, 0xBFFF)) {
        return prgrom->at((size_t)(address - 0x8000));
    } else if (Utils::inRange(address, 0xC000, 0xFFFF)) {
        return prgrom->at((size_t)(address - 0x8000) % prgrom->size());
    }

    return 0x00;
}

uint8_t NROM::readPPU(Address address) {
    if (Utils::inRange(address, 0x0000, 0x1FFF)) {
        return basicPatternTableRead(address);
    } else if (Utils::inRange(address, 0x2000, 0x3EFF)) {
        return basicNametableRead(address);
    }

    return 0x00;
}

void NROM::writeCPU(Address address, uint8_t value) {
    Cartridge *cartridge = nes->getCartridge();
    auto prgram = cartridge->getPRGRAM();

    if (Utils::inRange(address, 0x6000, 0x7FFF)) {
        prgram->at((size_t)(address - 0x6000)) = value;
    }
}

void NROM::writePPU(Address address, uint8_t value) {
    if (Utils::inRange(address, 0x2000, 0x2FFF)) {
        basicNametableWrite(address, value);
    }
}

void NROM::step() {

}