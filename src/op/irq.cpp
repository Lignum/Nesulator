#include "irq.h"

#include "../cpu.h"
#include "../memory.h"
#include "../nes.h"
#include "../op.h"
#include "../utils.h"

unsigned int Op::brk(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    RegisterFile *r = cpu->getRegs();

    uint8_t high, low;
    Utils::splitUint16LE(r->pc, &low, &high);

    cpu->push(high);
    cpu->push(low);

    cpu->push((uint8_t)r->p);
    cpu->setFlag(CPUFlag::IRQ_DISABLE, true);

    Memory *mem = cpu->getNES()->getMemory();
    r->pc = Utils::combineUint8sLE(mem->read(0xFFFE), mem->read(0xFFFF));
    return 0;
}

unsigned int Op::rti(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    RegisterFile *r = cpu->getRegs();
    r->p = (CPUFlag)cpu->pull();

    uint8_t low = cpu->pull();
    uint8_t high = cpu->pull();
    r->pc = Utils::combineUint8sLE(low, high);
    return 0;
}
