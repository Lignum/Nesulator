#pragma once

#include "ines.h"
#include "mapper.h"

#include <vector>
#include <memory>
#include <cstdint>

class NES;

class Cartridge {
    friend class NES;
public:
    /**
     * Creates a Cartridge from an `iNES::File`. Note that the contents of the `iNES::File` are _moved_,
     * rendering the `iNES::File`'s state undefined after passing it to this constructor! Hence, you should make a
     * copy of it beforehand, if you wish to use it further.
     *
     * @param file The file to create the cartridge from.
     */
    explicit Cartridge(iNES::File &file);

    size_t getPRGROMCount() const;

    const std::vector<uint8_t> *getPRGROM() const;

    size_t getCHRCount() const;

    std::vector<uint8_t> *getCHR();

    size_t getPRGRAMCount() const;

    std::vector<uint8_t> *getPRGRAM();

    /**
     * @return true if getCHR returns CHR-RAM, false if getCHR returns CHR-ROM.
     */
    bool isCHRRAM() const;

    Mapper *getMapper();

    uint8_t getMapperNumber() const;

private:
    void initMapper(NES *nes);

    bool chrram;
    std::vector<uint8_t> prg;
    std::vector<uint8_t> chr;
    std::vector<uint8_t> prgram;
    uint8_t mapperNumber;
    std::unique_ptr<Mapper> mapper;
};