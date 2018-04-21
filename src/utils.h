#pragma once

#include <iosfwd>
#include <iomanip>
#include <bitset>
#include <cstdint>

namespace Utils {
    template<typename A, typename B>
    constexpr bool inRange(A x, B a, B b) {
        return x >= (A)a && x <= (A)b;
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
    constexpr bool isBitSet(T bits, unsigned int bit) {
        return (bits & (1 << bit)) != 0;
    }

    template<typename T>
    void writeHexToStream(std::ostream &stream, T x) {
        stream << std::uppercase << std::setfill('0') << std::setw((int)(sizeof(T) * 2)) << std::hex << (uint64_t)x;
    }

    template<typename T>
    void writeBinaryToStream(std::ostream &stream, T x) {
        stream << std::bitset<sizeof(T) * 8>(x);
    }

    template<typename T, typename I>
    constexpr bool isFlagSet(T flags, T flag) {
        return (T)((I)flags & (I)flag) == flag;
    }

    template<typename T>
    constexpr bool isFlagSet8(T flags, T flag) {
        return isFlagSet<T, uint8_t>(flags, flag);
    }

    template<typename T, typename I>
    constexpr T setFlag(T flags, T flag, bool set) {
        return (T)(set ? (I)flags | (I)flag : (I)flags & ~(I)flag);
    }

    template<typename T>
    constexpr T setFlag8(T flags, T flag, bool set) {
        return setFlag<T, uint8_t>(flags, flag, set);
    }
}