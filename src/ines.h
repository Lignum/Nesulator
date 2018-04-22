#pragma once

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>

namespace iNES {
    extern const size_t PRG_ROM_SIZE;
    extern const size_t PRG_RAM_SIZE;
    extern const size_t CHR_ROM_SIZE;
    extern const char HEADER_MAGIC_BYTES[4];
    extern const size_t TRAINER_SIZE;

    struct Header {
        char magicBytes[4];
        uint8_t prgROMCount;
        uint8_t chrROMCount;
        uint8_t flags6;
        uint8_t flags7;
        uint8_t prgRAMCount;
        uint8_t padding[7];
    };

    struct File {
        Header header;
        std::vector<uint8_t> trainer;
        std::vector<uint8_t> prgROM;
        std::vector<uint8_t> chrROM;
    };

    enum class LoadError {
        NO_ERROR,
        MAGIC_BYTES_MISMATCH,
        READ_ERROR,
        OPEN_FAILED,
    };

    LoadError loadFromFile(const std::string &file, File &outFile);

    std::string getLoadErrorMessage(LoadError error);
}