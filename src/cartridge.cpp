#include "cartridge.h"

#include "nes.h"
#include "mappers.h"

#include <utility>

Cartridge::Cartridge(iNES::File &file)
    : prg(std::move(file.prgROM)),
      chr(std::move(file.chrROM)),
      chrram(file.header.chrROMCount == 0),
      mapperNumber((uint8_t)((file.header.flags6 >> 4) | (file.header.flags7 & 0xF0))),
      mapper(nullptr)
{
}

const std::vector<uint8_t> *Cartridge::getPRGROM() const {
    return &prg;
}

std::vector<uint8_t> *Cartridge::getCHR() {
    return &chr;
}

bool Cartridge::isCHRRAM() const {
    return chrram;
}

Mapper *Cartridge::getMapper() {
    return mapper.get();
}

void Cartridge::initMapper(NES *nes) {
    mapper = std::move(Mappers::create(nes, mapperNumber));
}

uint8_t Cartridge::getMapperNumber() const {
    return mapperNumber;
}
