#include "cpu.h"

#include "nes.h"
#include "ops.h"

#include <vector>
#include <cstdio>

CPU::CPU(NES *nes)
    : nes(nes),
      r(RegisterFile {
              0,                // a
              0,                // x
              0,                // y
              CPUFlag::UNUSED,  // p, unused flag must always be set.
              0x00,             // pc
              0xFF              // sp
      })
{
}

NES *CPU::getNES() {
    return nes;
}

RegisterFile *CPU::getRegisterFile() {
    return &r;
}

bool CPU::isFlagSet(CPUFlag flag) const {
    return ((uint8_t)r.p & (uint8_t)flag) == (uint8_t)flag;
}

void CPU::setFlag(CPUFlag flag, bool set) {
    if (set) {
        r.p = (CPUFlag)((uint8_t)r.p | (uint8_t)flag);
    } else {
        r.p = (CPUFlag)((uint8_t)r.p & ~(uint8_t)flag);
    }
}

void CPU::step() {
    const uint8_t op = fetch();
    const Op::Opcode *opDecoded = Op::decode(op);

#ifdef NESULATOR_DEBUG
    printf("%s\n", opDecoded->name);
#endif

    std::vector<uint8_t> operands;
    operands.reserve(opDecoded->operands);
    fetchOperands(opDecoded->operands, operands);
    opDecoded->handler(this, operands);
}

uint8_t CPU::fetch() {
    Memory *mem = nes->getMemory();
    uint16_t *pc = &nes->getCPU()->getRegisterFile()->pc;
    uint8_t op = mem->read((Address)*pc);
    *pc += 1;
    return op;
}

void CPU::fetchOperands(size_t count, std::vector<uint8_t> &operands) {
    for (size_t i = 0; i < count; i++) {
        const uint8_t op = fetch();
        operands.push_back(op);
    }
}
