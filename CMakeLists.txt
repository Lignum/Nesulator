cmake_minimum_required(VERSION 3.9)
project(Nesulator)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DNESULATOR_DEBUG")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNESULATOR_DEBUG")
endif()

add_executable(Nesulator src/main.cpp src/nes.cpp src/nes.h src/cpu.h src/cpu.cpp src/memory.cpp src/memory.h src/utils.h src/op.h src/op.cpp src/op/irq.h src/op/irq.cpp src/op/loads.cpp src/op/loads.h src/op/stores.cpp src/op/stores.h src/address.h src/op/transfers.cpp src/op/transfers.h src/op/flags.cpp src/op/flags.h src/op/control.cpp src/op/control.h src/op/stack.cpp src/op/stack.h src/op/arith.cpp src/op/arith.h src/ines.cpp src/ines.h src/cartridge.cpp src/cartridge.h src/mapper.cpp src/mapper.h src/mappers.cpp src/mappers.h src/mappers/nrom.cpp src/mappers/nrom.h src/ppu.cpp src/ppu.h)