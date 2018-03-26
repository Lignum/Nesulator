#pragma once

namespace Utils {
    template<typename T>
    constexpr inline bool inRange(T x, T a, T b) {
        return x >= a && x <= b;
    }
}