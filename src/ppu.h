#pragma once

#include "address.h"

#include <vector>
#include <cstdint>

enum class PPURegister : uint8_t {
    PPUCTRL = 0,
    PPUMASK = 1,
    PPUSTATUS = 2,
    OAMADDR = 3,
    OAMDATA = 4,
    PPUSCROLL = 5,
    PPUADDR = 6,
    PPUDATA = 7,
    OAMDMA = 8
};

enum class PPUControlFlag : uint8_t {
    NMI_ENABLE = 1 << 0,
    PPU_MASTER_SLAVE = 1 << 1,
    SPRITE_HEIGHT = 1 << 2,
    BACKGROUND_PATTERN_TABLE = 1 << 3,
    SPRITE_PATTERN_TABLE = 1 << 4,
};

enum class PPUMaskFlag : uint8_t {
    EMPHASISE_BLUE = 1 << 0,
    EMPHASISE_GREEN = 1 << 1,
    EMPHASISE_RED = 1 << 2,
    SHOW_SPRITES = 1 << 3,
    SHOW_BACKGROUND = 1 << 4,
    SHOW_SPRITES_LEFT_COLUMN = 1 << 5,
    SHOW_BACKGROUND_LEFT_COLUMN = 1 << 6,
    GREYSCALE = 1 << 7
};

enum class PPUStatusFlag : uint8_t {
    VERTICAL_BLANK,
    SPRITE_0_HIT,
    SPRITE_OVERFLOW
};

class PPU {
public:
    PPU();

    void writeRegister(PPURegister reg, uint8_t value);

    uint8_t readRegister(PPURegister reg) const;

    bool isControlFlagSet(PPUControlFlag flag) const;

    void setControlFlag(PPUControlFlag flag, bool set);

    bool isMaskFlagSet(PPUMaskFlag flag) const;

    void setMaskFlag(PPUMaskFlag flag, bool set);

    bool isStatusFlagSet(PPUStatusFlag flag) const;

    void setStatusFlag(PPUStatusFlag flag, bool set);

    static Address getPPURegisterAddress(PPURegister reg);

    static bool getRegisterFromAddress(Address address, PPURegister *outReg);

    static const size_t SPRITE_SIZE;
    static const size_t OBJECT_ATTRIBUTE_MEMORY_SIZE;

private:
    PPUControlFlag controlFlags;
    PPUMaskFlag maskFlags;
    PPUStatusFlag statusFlags;

    uint8_t ppuLatch;

    bool addressLatch;

    uint8_t scrollX;
    uint8_t scrollY;

    uint16_t address;

    std::vector<uint8_t> oam;
};