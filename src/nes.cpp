#include "nes.h"

#include "cpu.h"

NES::NES()
    : cpu(this)
{

}

CPU *NES::getCPU() {
    return &cpu;
}
