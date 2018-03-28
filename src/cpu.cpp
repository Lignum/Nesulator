#include "cpu.h"

#include "nes.h"
#include "op.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>

CPU::CPU(NES *nes)
    : nes(nes),
      r(RegisterFile {
          0,                // a
          0,                // x
          0,                // y
          CPUFlag::UNUSED,  // p, unused flag must always be set.
          0xFF,             // sp
          0x0000            // pc
      })
{
}

NES *CPU::getNES() {
    return nes;
}

RegisterFile *CPU::getRegs() {
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

unsigned int CPU::step() {
    const uint8_t op = fetch();
    const Op::Opcode *opDecoded = Op::decode(op);

    if (opDecoded == nullptr) {
        fprintf(stderr, "0x%02X was not a valid opcode!", op);
        return 0;
    }

    const size_t operandCount = Op::getAddressingModeOperandCount(opDecoded->mode);

    std::vector<uint8_t> operands;
    operands.reserve(operandCount);
    fetchOperands(operandCount, operands);

#ifdef NESULATOR_DEBUG
    std::string inst;
    Op::formatInstruction(opDecoded, operands, &inst);
    std::cout << inst << "\n";
#endif

    return opDecoded->handler(this, operands, opDecoded) + opDecoded->baseCycles;
}

uint8_t CPU::fetch() {
    Memory *mem = nes->getMemory();
    uint8_t op = mem->read((Address)r.pc);
    r.pc++;
    return op;
}

void CPU::fetchOperands(size_t count, std::vector<uint8_t> &operands) {
    for (size_t i = 0; i < count; i++) {
        const uint8_t op = fetch();
        operands.push_back(op);
    }
}

void CPU::printState() const {
    printf("A=$%02X, X=$%02X, Y=$%02X, P=$%02X, SP=$%02X, PC=$%04X\n", r.a, r.x, r.y, r.p, r.sp, r.pc);
}
