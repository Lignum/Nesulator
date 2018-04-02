#include "nes.h"

#include "cartridge.h"
#include "cpu.h"
#include "memory.h"
#include "mappers.h"
#include "utils.h"

#include <utility>
#include <iostream>

NES::NES(Cartridge &cartridge)
    : cartridge(std::move(cartridge)),
      cpu(this),
      mem(this)
{
    this->cartridge.initMapper(this);

    std::cout << "Reset vector is $";
    Utils::writeHexToStream(std::cout, mem.getResetVector());
    std::cout << "\n";
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
