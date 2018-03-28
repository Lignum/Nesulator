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

    template<typename T>
    void writeHexToStream(std::ostream &stream, T x) {
        stream << std::uppercase << std::setfill('0') << std::setw((int)(sizeof(T) * 2)) << std::hex << (uint64_t)x;
    }
}