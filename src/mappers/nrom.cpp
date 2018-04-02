#include "nrom.h"

#include "../mapper.h"

NROM::NROM(NES *nes) : Mapper(nes, 0, "NROM") {

}

uint8_t NROM::read(Address address) {
    return 0;
}

void NROM::write(Address address, uint8_t value) {

}

void NROM::step() {

}
