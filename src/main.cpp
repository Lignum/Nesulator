#include "nes.h"

int main() {
    NES nes;
    nes.getMemory()->write(0x00, 0x06);
    nes.getCPU()->step();
    return 0;
}