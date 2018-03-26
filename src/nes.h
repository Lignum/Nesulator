#pragma once

#include "cpu.h"

class NES {
public:
    NES();

    CPU *getCPU();

private:
    CPU cpu;
};
