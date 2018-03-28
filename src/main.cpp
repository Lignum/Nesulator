#include "nes.h"
#include "utils.h"

#include <iostream>

int main() {
    NES nes;
    CPU *cpu = nes.getCPU();
    Memory *mem = nes.getMemory();

    const size_t programSize = 2;

    // LDA #$6F
    mem->write(0x00, 0xA9);
    mem->write(0x01, 0x6F);

    // STA $24
    mem->write(0x02, 0x85);
    mem->write(0x03, 0x24);

    for (size_t i = 0; i < programSize; i++) {
        cpu->printState();
        cpu->step();
    }

    cpu->printState();

    std::cout << "$0024: $";
    Utils::writeHexToStream(std::cout, mem->read(0x0024));
    std::cout << "\n";
    return 0;
}