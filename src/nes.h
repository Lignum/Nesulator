#pragma once

#include "cpu.h"
#include "memory.h"

class NES {
public:
    NES();

    CPU *getCPU();

    Memory *getMemory();

private:
    CPU cpu;
    Memory mem;
};
