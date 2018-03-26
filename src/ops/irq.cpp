#include "irq.h"

#include <cstdio>

void Op::brk(CPU *cpu, const std::vector<uint8_t> &operands) {
    printf("Hit a BRK\n");
}
