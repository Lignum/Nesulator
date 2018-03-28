#include "nes.h"
#include "utils.h"

#include <iostream>

int main() {
    NES nes;
    CPU *cpu = nes.getCPU();
    Memory *mem = nes.getMemory();

    const size_t programSize = 13;

    // LDA #$6F
    mem->write(0x00, 0xA9);
    mem->write(0x01, 0x80);

    // STA $24
    mem->write(0x02, 0x85);
    mem->write(0x03, 0x24);

    // TAX
    mem->write(0x04, 0xAA);

    // TAY
    mem->write(0x05, 0xA8);

    // SEI
    mem->write(0x06, 0x78);

    // JMP $0767
    mem->write(0x07, 0x4C);
    mem->write(0x08, 0x67);
    mem->write(0x09, 0x07);

    // LDX #$A2
    mem->write(0x0500, 0xA2);
    mem->write(0x0501, 0xA2);

    // RTS
    mem->write(0x0502, 0x60);

    // CLI
    mem->write(0x0767, 0x58);

    // PHA
    mem->write(0x0768, 0x48);

    // LDA #$20
    mem->write(0x0769, 0xA9);
    mem->write(0x076A, 0x20);

    // JSR $0500
    mem->write(0x076B, 0x20);
    mem->write(0x076C, 0x00);
    mem->write(0x076D, 0x05);

    // PLA
    mem->write(0x076E, 0x68);

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