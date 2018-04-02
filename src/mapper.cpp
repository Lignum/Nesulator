#include "mapper.h"

Mapper::Mapper(NES *nes, uint8_t id, const std::string &name)
    : nes(nes),
      id(id),
      name(name)
{
}

NES *Mapper::getNES() {
    return nes;
}

uint8_t Mapper::getID() const {
    return id;
}

const std::string *Mapper::getName() const {
    return &name;
}

