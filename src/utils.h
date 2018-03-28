#pragma once

#include <iosfwd>
#include <iomanip>
#include <cstdint>

namespace Utils {
    template<typename T>
    constexpr bool inRange(T x, T a, T b) {
        return x >= a && x <= b;
    }

    constexpr uint16_t combineUint8sBE(uint8_t a, uint8_t b) {
        return ((uint16_t)a << 8) | (uint16_t)b;
    }

    constexpr uint16_t combineUint8sLE(uint8_t a, uint8_t b) {
        return ((uint16_t)b << 8) | (uint16_t)a;
    }

    constexpr void splitUint16LE(uint16_t x, uint8_t *outA, uint8_t *outB) {
        if (outA) *outA = (uint8_t)(x & 0xFF);
        if (outB) *outB = (uint8_t)(x >> 8);
    }

    constexpr void splitUint16BE(uint16_t x, uint8_t *outA, uint8_t *outB) {
        if (outA) *outA = (uint8_t)(x >> 8);
        if (outB) *outB = (uint8_t)(x & 0xFF);
    }

    template<typename T>
    void writeHexToStream(std::ostream &stream, T x) {
        stream << std::uppercase << std::setfill('0') << std::setw((int)(sizeof(T) * 2)) << std::hex << (uint64_t)x;
    }

    template<typename T>
    void writeBinaryToStream(std::ostream &stream, T x) {
        size_t size = sizeof(T) * 8;

        for (size_t i = 0; i < size; i++) {
            stream << (unsigned long)((x >> (size - i - 1)) & 0b1);
        }
    }
}