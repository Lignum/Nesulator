#include "nes.h"

int main() {
    NES nes;
    nes.getCPU()->step();
    return 0;
}