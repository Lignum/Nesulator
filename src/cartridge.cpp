#include "cartridge.h"

#include "nes.h"
#include "mappers.h"
#include "ines.h"
#include "utils.h"

#include <utility>

Cartridge::Cartridge(iNES::File &file)
    : prg(std::move(file.prgROM)),
      chr(std::move(file.chrROM)),
      prgram(file.header.prgRAMCount == 0 ? iNES::PRG_RAM_SIZE : file.header.prgRAMCount * iNES::PRG_RAM_SIZE),
      chrram(file.header.chrROMCount == 0),
      mapperNumber((uint8_t)((file.header.flags6 >> 4) | (file.header.flags7 & 0xF0))),
      mapper(nullptr),
      mirroring(Utils::isBitSet(file.header.flags6, 3) ? Mirroring::FOUR_SCREEN :
                Utils::isBitSet(file.header.flags6, 0) ? Mirroring::VERTICAL : Mirroring::HORIZONTAL)
{
}

size_t Cartridge::getPRGROMCount() const {
    return prg.size() / iNES::PRG_ROM_SIZE;
}

const std::vector<uint8_t> *Cartridge::getPRGROM() const {
    return &prg;
}

size_t Cartridge::getPRGRAMCount() const {
    return prgram.size() / iNES::PRG_RAM_SIZE;
}

std::vector<uint8_t> *Cartridge::getPRGRAM() {
    return &prgram;
}

size_t Cartridge::getCHRCount() const {
    return chr.size() / iNES::CHR_ROM_SIZE;
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

Mirroring Cartridge::getMirroring() const {
    return mirroring;
}
