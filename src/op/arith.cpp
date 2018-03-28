#include "arith.h"

#include "../op.h"
#include "../cpu.h"

#define UNARY(name, f) \
    unsigned int Op::name(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        uint8_t value = f(cpu, Op::address(cpu, opcode->mode, operands)); \
        Op::setNZFlags(cpu, value); \
        Op::addressWrite(cpu, opcode->mode, operands, value); \
    }

uint8_t computeASL(CPU *cpu, uint8_t x) {
    cpu->setFlag(CPUFlag::CARRY, ((x >> 7) & 0b1) == 1);
    return x << 1;
}

UNARY(asl, computeASL);

uint8_t computeLSR(CPU *cpu, uint8_t x) {
    cpu->setFlag(CPUFlag::CARRY, (x & 0b1) == 1);
    return x >> 1;
}

UNARY(lsr, computeLSR);

uint8_t computeROL(CPU *cpu, uint8_t x) {
    auto bit0 = (uint8_t)(cpu->isFlagSet(CPUFlag::CARRY) ? 1 : 0);
    cpu->setFlag(CPUFlag::CARRY, ((x >> 7) & 0b1) == 1);
    return (x << 1) | bit0;
}

UNARY(rol, computeROL);

uint8_t computeROR(CPU *cpu, uint8_t x) {
    auto bit7 = (uint8_t)(cpu->isFlagSet(CPUFlag::CARRY) ? (1 << 7) : 0);
    cpu->setFlag(CPUFlag::CARRY, (x & 0b1) == 1);
    return (x >> 1) | bit7;
}

UNARY(ror, computeROR);