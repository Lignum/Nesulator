#include "mappers.h"
#include "mapper.h"
#include "nes.h"

#include "mappers/nrom.h"

#include <memory>
#include <iostream>

#define FACTORY(name) ([](NES *nes) { return std::unique_ptr<Mapper>(new name(nes)); })

static const Mappers::MapperFactory MAPPER_FACTORIES[] = {
    FACTORY(NROM)
};

std::unique_ptr<Mapper> Mappers::create(NES *nes, uint8_t id) {
    if (id < sizeof(MAPPER_FACTORIES) / sizeof(Mappers::MapperFactory)) {
        auto factory = MAPPER_FACTORIES[id];
        return factory != nullptr ? factory(nes) : nullptr;
    } else {
        return nullptr;
    }
}
