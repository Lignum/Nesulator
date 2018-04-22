#include "control.h"

#include "../cpu.h"
#include "../utils.h"

static void jump(CPU *cpu, Address address) {
    cpu->jump(address);
}

unsigned int Op::jmp(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    // TODO: Implement weird 6502 behaviour at page boundaries with AddressingMode::INDIRECT.
    jump(cpu, Op::getAddress(cpu, opcode->mode, operands));
    return 0;
}

#define BRANCH_IF_SET(name, flag) \
    unsigned int Op::name(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        if (cpu->isFlagSet(flag)) { \
            jump(cpu, Op::getAddress(cpu, opcode->mode, operands)); \
        } \
        return 0; \
    }

#define BRANCH_IF_CLEAR(name, flag) \
    unsigned int Op::name(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) { \
        if (!cpu->isFlagSet(flag)) { \
            jump(cpu, Op::getAddress(cpu, opcode->mode, operands)); \
        } \
        return 0; \
    }

BRANCH_IF_CLEAR(bcc, CPUFlag::CARRY);
BRANCH_IF_SET(bcs, CPUFlag::CARRY);
BRANCH_IF_SET(beq, CPUFlag::ZERO);
BRANCH_IF_SET(bmi, CPUFlag::NEGATIVE);
BRANCH_IF_CLEAR(bne, CPUFlag::ZERO);
BRANCH_IF_CLEAR(bpl, CPUFlag::NEGATIVE);
BRANCH_IF_CLEAR(bvc, CPUFlag::OVER_FLOW);
BRANCH_IF_SET(bvs, CPUFlag::OVER_FLOW)

unsigned int Op::jsr(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    RegisterFile *r = cpu->getRegs();
    Address pc = r->pc - (Address)1;

    uint8_t high, low;
    Utils::splitUint16LE(pc, &low, &high);

    cpu->push(high);
    cpu->push(low);

    r->pc = Op::getAddress(cpu, opcode->mode, operands);
    return 0;
}

unsigned int Op::rts(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    RegisterFile *r = cpu->getRegs();
    uint8_t low = cpu->pull();
    uint8_t high = cpu->pull();
    r->pc = Utils::combineUint8sLE(low, high) + (Address)1;
    return 0;
};
