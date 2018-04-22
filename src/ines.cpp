#include "ines.h"
#include "utils.h"

#include <vector>
#include <string>
#include <cstring>
#include <fstream>

const size_t iNES::PRG_ROM_SIZE = 16384;
const size_t iNES::PRG_RAM_SIZE = 8192;
const size_t iNES::CHR_ROM_SIZE = 8192;
const char iNES::HEADER_MAGIC_BYTES[4] = { 'N', 'E', 'S', '\x1A' };
const size_t iNES::TRAINER_SIZE = 512;

iNES::LoadError iNES::loadFromFile(const std::string &file, iNES::File &outFile) {
	std::ifstream in(file, std::ios::in | std::ios::binary);

    if (!in) {
        return iNES::LoadError::OPEN_FAILED;
    }

#define READ_MEMBER(mem) in.read((char*)&(mem), sizeof(mem))
	
	bool headerSuccessfullyRead = 
		READ_MEMBER(outFile.header.magicBytes) &&
		READ_MEMBER(outFile.header.prgROMCount) &&
		READ_MEMBER(outFile.header.chrROMCount) &&
		READ_MEMBER(outFile.header.flags6) &&
		READ_MEMBER(outFile.header.flags7) &&
		READ_MEMBER(outFile.header.prgRAMCount) &&
		READ_MEMBER(outFile.header.padding);

    if (!headerSuccessfullyRead) {
        return iNES::LoadError::READ_ERROR;
    }

    if (std::memcmp(outFile.header.magicBytes, iNES::HEADER_MAGIC_BYTES, sizeof(iNES::HEADER_MAGIC_BYTES)) != 0) {
        // This is not an iNES file!!
        return iNES::LoadError::MAGIC_BYTES_MISMATCH;
    }

    const bool hasTrainer = Utils::isBitSet(outFile.header.flags6, 2);

    if (hasTrainer) {
        outFile.trainer.resize(TRAINER_SIZE);

		if (!in.read((char*)outFile.trainer.data(), outFile.trainer.size())) {
			return iNES::LoadError::READ_ERROR;
        }
    }

    const size_t prgSize = outFile.header.prgROMCount * iNES::PRG_ROM_SIZE;
    outFile.prgROM.resize(prgSize);

    if (!in.read((char*)outFile.prgROM.data(), outFile.prgROM.size())) {
		return iNES::LoadError::READ_ERROR;
    }

    if (outFile.header.chrROMCount > 0) {
        const size_t chrSize = outFile.header.chrROMCount * iNES::CHR_ROM_SIZE;
        outFile.chrROM.resize(chrSize);

        if (!in.read((char*)outFile.chrROM.data(), outFile.chrROM.size())) {
			return iNES::LoadError::READ_ERROR;
        }
    } else {
        // We're dealing with CHR-RAM.
        outFile.chrROM.resize(iNES::CHR_ROM_SIZE, 0x00);
    }

    return iNES::LoadError::NO_ERROR;
}

std::string iNES::getLoadErrorMessage(iNES::LoadError error) {
    switch (error) {
        case iNES::LoadError::NO_ERROR:
            return "No error.";

        case iNES::LoadError::OPEN_FAILED:
            return "Could not open file!";

        case iNES::LoadError::MAGIC_BYTES_MISMATCH:
            return "Magic bytes do not match. This is not an iNES file!";

        case iNES::LoadError::READ_ERROR:
            return "Failed to read from file!";
    }

	return "N/A";
}

