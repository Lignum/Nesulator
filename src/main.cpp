#include "nes.h"

#include <cstdio>

int main() {
    NES nes;
    nes.getMemory()->write(0x00, 0xA9);
    nes.getMemory()->write(0x01, 0x7F);
    nes.getCPU()->printState();
    unsigned int cycles = nes.getCPU()->step();
    nes.getCPU()->printState();
    printf("Step took %u cycles", cycles);
    return 0;
}