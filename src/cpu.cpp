#include "cpu.h"

#include "nes.h"
#include "op.h"
#include "utils.h"
#include "memory.h"

#include <vector>
#include <string>
#include <iostream>

CPU::CPU(NES *nes)
    : nes(nes),
      r(RegisterFile {
          0,                // a
          0,                // x
          0,                // y
          CPUFlag::UNUSED,  // p, unused flag must always be set.
          0xFF,             // s
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
    r.p = (CPUFlag)(set ? (uint8_t)r.p | (uint8_t)flag : (uint8_t)r.p & ~(uint8_t)flag);
}

void CPU::push(uint8_t value) {
    nes->getMemory()->write(NES_STACK_ADDRESS + r.s, value);
    r.s--;
}

uint8_t CPU::pull() {
    r.s++;
    return nes->getMemory()->read(NES_STACK_ADDRESS + r.s);
}

unsigned int CPU::step() {
    const uint8_t op = fetch();
    const Op::Opcode *opDecoded = Op::decode(op);

    if (opDecoded == nullptr) {
        std::cerr << "$";
        Utils::writeHexToStream(std::cerr, op);
        std::cerr << " was not a valid opcode!\n";
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
    std::cout << "A=$";
    Utils::writeHexToStream(std::cout, r.a);

    std::cout << ", X=$";
    Utils::writeHexToStream(std::cout, r.x);

    std::cout << ", Y=$";
    Utils::writeHexToStream(std::cout, r.y);

    std::cout << ", P=0b";
    Utils::writeBinaryToStream(std::cout, (uint8_t)r.p);

    std::cout << ", S=$";
    Utils::writeHexToStream(std::cout, r.s);

    std::cout << ", PC=$";
    Utils::writeHexToStream(std::cout, r.pc);

    std::cout << "\n";
}
