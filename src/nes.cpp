#include "nes.h"

#include "cpu.h"
#include "memory.h"

NES::NES()
    : cpu(this),
      mem(this)
{
}

CPU *NES::getCPU() {
    return &cpu;
}

Memory *NES::getMemory() {
    return &mem;
}
