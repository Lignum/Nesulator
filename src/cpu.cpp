#include "cpu.h"

#include "nes.h"

CPU::CPU(NES *nes)
    : nes(nes),
      r(RegisterFile {
              0,                // a
              0,                // x
              0,                // y
              CPUFlag::UNUSED,  // p, unused flag must always be set.
              0x00,             // pc
              0xFF              // sp
      })
{
}

NES *CPU::getNES() {
    return nes;
}

RegisterFile *CPU::getRegisterFile() {
    return &r;
}

bool CPU::isFlagSet(CPUFlag flag) const {
    return ((uint8_t)r.p & (uint8_t)flag) == (uint8_t)flag;
}

void CPU::setFlag(CPUFlag flag, bool set) {
    if (set) {
        r.p = (CPUFlag)((uint8_t)r.p | (uint8_t)flag);
    } else {
        r.p = (CPUFlag)((uint8_t)r.p & ~(uint8_t)flag);
    }
}
