#pragma once

#include <cstdint>

namespace Utils {
    template<typename T>
    constexpr inline bool inRange(T x, T a, T b) {
        return x >= a && x <= b;
    }

    constexpr inline uint16_t combineUint8sBE(uint8_t a, uint8_t b) {
        return ((uint16_t)a << 8) | (uint16_t)b;
    }

    constexpr inline uint16_t combineUint8sLE(uint8_t a, uint8_t b) {
        return ((uint16_t)b << 8) | (uint16_t)a;
    }
}