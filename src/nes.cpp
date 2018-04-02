#include "nes.h"

#include "cartridge.h"
#include "cpu.h"
#include "memory.h"
#include "mappers.h"

#include <utility>

NES::NES(Cartridge &cartridge)
    : cartridge(std::move(cartridge)),
      cpu(this),
      mem(this)
{
    this->cartridge.initMapper(this);
    cpu.jump(mem.getResetVector());
}

CPU *NES::getCPU() {
    return &cpu;
}

Memory *NES::getMemory() {
    return &mem;
}

Cartridge *NES::getCartridge() {
    return &cartridge;
}
