#pragma once

#include "../ops.h"

#include <vector>
#include <cstdint>

namespace Op {
    void brk(CPU *cpu, const std::vector<uint8_t> &operands);
}