#pragma once

#include "cartridge.h"
#include "cpu.h"
#include "memory.h"

class NES {
public:
    /**
     * Constructs a NES, initialised with the given Cartridge. The Cartridge object is _moved_!! This means you cannot
     * use the object afterwards, you must make a copy.
     * @param cartridge The cartridge to 'insert' into the NES.
     */
    NES(Cartridge &cartridge);

    Cartridge *getCartridge();

    CPU *getCPU();

    Memory *getMemory();

private:
    Cartridge cartridge;
    CPU cpu;
    Memory mem;
};
