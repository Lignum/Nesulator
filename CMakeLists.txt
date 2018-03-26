cmake_minimum_required(VERSION 3.9)
project(Nesulator)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/DNESULATOR_DEBUG")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-DNESULATOR_DEBUG")
endif()

add_executable(Nesulator src/main.cpp src/nes.cpp src/nes.h src/cpu.h src/cpu.cpp src/memory.cpp src/memory.h src/utils.h src/ops.h src/ops.cpp src/ops/irq.h src/ops/irq.cpp)