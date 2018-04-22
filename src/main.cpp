#include "ines.h"
#include "nes.h"
#include "utils.h"

#include <iostream>
#include <thread>
#include <chrono>

static const unsigned int NANOSECONDS_PER_CYCLE = 602;

int main() {
	iNES::File file = {};
    iNES::LoadError error = iNES::loadFromFile("test.nes", file);

    if (error != iNES::LoadError::NO_ERROR) {
        std::cerr << "iNES Load Error: " << iNES::getLoadErrorMessage(error) << "\n";
#ifdef _WIN32
		std::cin.get();
#endif
        return EXIT_FAILURE;
    }

    Cartridge cartridge(file);
    NES nes(cartridge);

    Mapper *mapper = nes.getCartridge()->getMapper();

    if (mapper == nullptr) {
        std::cerr << "The mapper that this game requires (" << (unsigned int)nes.getCartridge()->getMapperNumber() << ") has not been implemented yet!\n";
#ifdef _WIN32
		std::cin.get();
#endif
        return EXIT_FAILURE;
    }

    std::cout << "iNES file has " << *mapper->getName() << " mapper (" << (unsigned int)mapper->getID() << ")\n";

    CPU *cpu = nes.getCPU();

    while (true) {
        cpu->printState();
        unsigned int cycles = cpu->step();
        std::this_thread::sleep_for(std::chrono::nanoseconds(cycles * NANOSECONDS_PER_CYCLE));
    }

#ifdef _WIN32
	std::cin.get();
#endif

    return EXIT_SUCCESS;
}