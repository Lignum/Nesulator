#include "ines.h"
#include "nes.h"
#include "utils.h"

#include <iostream>
#include <thread>
#include <chrono>

static const unsigned int NANOSECONDS_PER_CYCLE = 602;

int main() {
    iNES::File file;
    iNES::LoadError error = iNES::loadFromFile("test.nes", file);

    if (error != iNES::LoadError::NO_ERROR) {
        std::cerr << "iNES Load Error: " << iNES::getLoadErrorMessage(error) << "\n";
        return EXIT_FAILURE;
    }

    Cartridge cartridge(file);
    NES nes(cartridge);

    Mapper *mapper = nes.getCartridge()->getMapper();

    if (mapper == nullptr) {
        std::cerr << "The mapper that this game requires (" << (unsigned int)nes.getCartridge()->getMapperNumber() << ") has not been implemented yet!\n";
        return EXIT_FAILURE;
    }

    std::cout << "iNES file has " << *mapper->getName() << " mapper (" << (unsigned int)mapper->getID() << ")\n";

    CPU *cpu = nes.getCPU();
    Memory *mem = nes.getMemory();

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

    // LSR A
    mem->write(0x076F, 0x4A);

    // INC $24
    mem->write(0x0770, 0xE6);
    mem->write(0x0771, 0x24);

    // DEX
    mem->write(0x0772, 0xCA);

    // ADC #$AB
    mem->write(0x0773, 0x69);
    mem->write(0x0774, 0xAB);

    // SBC #$BA
    mem->write(0x0775, 0xE9);
    mem->write(0x0776, 0xBA);

    while (true) {
        cpu->printState();
        unsigned int cycles = cpu->step();
        std::this_thread::sleep_for(std::chrono::nanoseconds(cycles * NANOSECONDS_PER_CYCLE));
    }

    return EXIT_SUCCESS;
}