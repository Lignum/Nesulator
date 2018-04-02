#pragma once

#include "mapper.h"

#include <memory>

class NES;

namespace Mappers {
    typedef std::unique_ptr<Mapper> (*MapperFactory)(NES *nes);

    std::unique_ptr<Mapper> create(NES *nes, uint8_t id);
}