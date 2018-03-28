#include "arith.h"

#include "../op.h"
#include "../cpu.h"

#define UNARY_MEM_ACC(name, f) \
    unsigned int Op::name(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        const uint8_t value = f(cpu, Op::address(cpu, opcode->mode, operands)); \
        Op::setNZFlags(cpu, value); \
        Op::addressWrite(cpu, opcode->mode, operands, value); \
    }

static uint8_t computeASL(CPU *cpu, uint8_t x) {
    cpu->setFlag(CPUFlag::CARRY, ((x >> 7) & 0b1) == 1);
    return x << 1;
}

UNARY_MEM_ACC(asl, computeASL);

static uint8_t computeLSR(CPU *cpu, uint8_t x) {
    cpu->setFlag(CPUFlag::CARRY, (x & 0b1) == 1);
    return x >> 1;
}

UNARY_MEM_ACC(lsr, computeLSR);

static uint8_t computeROL(CPU *cpu, uint8_t x) {
    const auto bit0 = (uint8_t)(cpu->isFlagSet(CPUFlag::CARRY) ? 1 : 0);
    cpu->setFlag(CPUFlag::CARRY, ((x >> 7) & 0b1) == 1);
    return (x << 1) | bit0;
}

UNARY_MEM_ACC(rol, computeROL);

static uint8_t computeROR(CPU *cpu, uint8_t x) {
    const auto bit7 = (uint8_t)(cpu->isFlagSet(CPUFlag::CARRY) ? (1 << 7) : 0);
    cpu->setFlag(CPUFlag::CARRY, (x & 0b1) == 1);
    return (x >> 1) | bit7;
}

UNARY_MEM_ACC(ror, computeROR);

static uint8_t computeINC(CPU *cpu, uint8_t x) {
    return x + (uint8_t)1;
}

UNARY_MEM_ACC(inc, computeINC);

static uint8_t computeDEC(CPU *cpu, uint8_t x) {
    return x - (uint8_t)1;
}

UNARY_MEM_ACC(dec, computeDEC);

#define UNARY_REG(name, reg, f) \
    unsigned int Op::name(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        RegisterFile *r = cpu->getRegs(); \
        r->reg = f(cpu, r->reg); \
        Op::setNZFlags(cpu, r->reg); \
    }

UNARY_REG(inx, x, computeINC);
UNARY_REG(iny, y, computeINC);

UNARY_REG(dex, x, computeDEC);
UNARY_REG(dey, y, computeDEC)

#define BINARY(name, reg, f) \
    unsigned int Op::name(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        RegisterFile *r = cpu->getRegs(); \
        const uint8_t v = Op::address(cpu, opcode->mode, operands); \
        r->reg = f(cpu, r->reg, v); \
        Op::setNZFlags(cpu, r->reg); \
        return 0; \
    }

static uint8_t computeAND(CPU *cpu, uint8_t a, uint8_t b) {
    return a & b;
}

BINARY(_and, a, computeAND);

static uint8_t computeORA(CPU *cpu, uint8_t a, uint8_t b) {
    return a | b;
}

BINARY(ora, a, computeORA);

static uint8_t computeEOR(CPU *cpu, uint8_t a, uint8_t b) {
    return a ^ b;
}

BINARY(eor, a, computeEOR)

unsigned int Op::bit(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    RegisterFile *r = cpu->getRegs();
    const uint8_t v = Op::address(cpu, opcode->mode, operands);
    uint8_t a = r->a & v;
    Op::setZeroFlag(cpu, a);
    Op::setNegativeFlag(cpu, v);
    cpu->setFlag(CPUFlag::OVERFLOW, ((v >> 6) & 0b1) == 1);
    return 0;
};