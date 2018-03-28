#include "nes.h"

#include <cstdio>

int main() {
    NES nes;
    nes.getMemory()->write(0x00, 0xB5);
    nes.getMemory()->write(0x01, 0x6F);

    nes.getMemory()->write(0x02, 0xA2);
    nes.getMemory()->write(0x03, 0x01);

    nes.getMemory()->write(0x04, 0xB5);
    nes.getMemory()->write(0x05, 0x6F);

    nes.getMemory()->write(0x6F, 0xAB);
    nes.getMemory()->write(0x70, 0xCD);

    nes.getCPU()->printState();
    nes.getCPU()->step();
    nes.getCPU()->printState();
    nes.getCPU()->step();
    nes.getCPU()->printState();
    nes.getCPU()->step();
    nes.getCPU()->printState();

    return 0;
}