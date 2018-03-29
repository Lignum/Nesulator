#include "ines.h"

#include <cstdio>
#include <vector>
#include <string>
#include <cstring>

const size_t iNES::PRG_ROM_SIZE = 16384;
const size_t iNES::CHR_ROM_SIZE = 8192;
const char iNES::HEADER_MAGIC_BYTES[4] = { 'N', 'E', 'S', '\x1A' };

static iNES::LoadError getFileError(FILE *fp) {
    if (feof(fp)) {
        return iNES::LoadError::EARLY_END_OF_FILE;
    } else if (ferror(fp)) {
        return iNES::LoadError::READ_ERROR;
    }

    return iNES::LoadError::UNKNOWN_ERROR;
}

iNES::LoadError iNES::loadFromFile(const std::string &file, File &outFile) {
    FILE *fp = fopen(file.c_str(), "rb");

    if (!fp) {
        return iNES::LoadError::OPEN_FAILED;
    }

    size_t headerBytesRead =
        fread(&outFile.header.magicBytes, sizeof(char), 4, fp) +
        fread(&outFile.header.prgROMCount, sizeof(uint8_t), 1, fp) +
        fread(&outFile.header.chrROMCount, sizeof(uint8_t), 1, fp) +
        fread(&outFile.header.flags6, sizeof(uint8_t), 1, fp) +
        fread(&outFile.header.flags7, sizeof(uint8_t), 1, fp) +
        fread(&outFile.header.prgRAMSize, sizeof(uint8_t), 1, fp) +
        fread(&outFile.header.flags9, sizeof(uint8_t), 1, fp) +
        fread(&outFile.header.padding, sizeof(uint8_t), 6, fp);

    if (headerBytesRead < 16) {
        fclose(fp);
        return getFileError(fp);
    }

    if (memcmp(outFile.header.magicBytes, iNES::HEADER_MAGIC_BYTES, sizeof(iNES::HEADER_MAGIC_BYTES)) != 0) {
        // This is not an iNES file!!
        return iNES::LoadError::MAGIC_BYTES_MISMATCH;
    }

    const size_t prgSize = outFile.header.prgROMCount * iNES::PRG_ROM_SIZE;
    outFile.prgROM.resize(prgSize);

    if (fread(outFile.prgROM.data(), sizeof(uint8_t), prgSize, fp) < prgSize) {
        fclose(fp);
        return getFileError(fp);
    }

    const size_t chrSize = outFile.header.chrROMCount * iNES::CHR_ROM_SIZE;
    outFile.chrROM.resize(chrSize);

    if (fread(outFile.chrROM.data(), sizeof(uint8_t), chrSize, fp) < chrSize) {
        fclose(fp);
        return getFileError(fp);
    }

    fclose(fp);
    return iNES::LoadError::NO_ERROR;
}

std::string iNES::getLoadErrorMessage(iNES::LoadError error) {
    switch (error) {
        case iNES::LoadError::NO_ERROR:
            return "No error";

        case iNES::LoadError::UNKNOWN_ERROR:
            return "Unknown error";

        case iNES::LoadError::OPEN_FAILED:
            return "Could not open file!";

        case iNES::LoadError::MAGIC_BYTES_MISMATCH:
            return "Magic bytes do not match. This is not an iNES file!";

        case iNES::LoadError::READ_ERROR:
            return "Failed to read from file!";

        case iNES::LoadError::EARLY_END_OF_FILE:
            return "File ended abruptly. Is this file corrupted?";
    }
}
