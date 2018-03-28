#include "op.h"

#include "cpu.h"
#include "nes.h"
#include "memory.h"
#include "op/irq.h"
#include "op/loads.h"
#include "utils.h"

#include <vector>
#include <cstdio>
#include <sstream>
#include <iomanip>

static unsigned int unimplemented(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    fprintf(stderr, "Opcode 0x%02X (%s) has not been implemented yet!\n", opcode->code, opcode->name);
    return 0;
}

static unsigned int unsupported(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    fprintf(stderr, "Opcode 0x%02X (%s) is not supported!\n", opcode->code, opcode->name);
    return 0;
}

using Opcode = Op::Opcode;
using AM = Op::AddressingMode;

#define OP(code, name, addressingMode, handler, baseCycles) Opcode { code, baseCycles, name, addressingMode, handler }
#define UNSUPPORTED_OP(code, name, addressingMode, baseCycles) Opcode { code, baseCycles, name, addressingMode, unsupported }

static const Opcode OPCODES[] = {
    /*
     * 0x00 - 0x0F
     */
    OP(0x00, "BRK", AM::IMPLICIT, Op::brk, 7),
    OP(0x01, "ORA", AM::INDEXED_INDIRECT, unimplemented, 6),
    UNSUPPORTED_OP(0x02, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0x03, "SLO", AM::INDEXED_INDIRECT, 8),
    UNSUPPORTED_OP(0x04, "NOP", AM::ZERO_PAGE, 3),
    OP(0x05, "ORA", AM::ZERO_PAGE, unimplemented, 3),
    OP(0x06, "ASL", AM::ZERO_PAGE, unimplemented, 5),
    UNSUPPORTED_OP(0x07, "SLO", AM::ZERO_PAGE, 5),
    OP(0x08, "PHP", AM::IMPLICIT, unimplemented, 3),
    OP(0x09, "ORA", AM::IMMEDIATE, unimplemented, 2),
    OP(0x0A, "ASL", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0x0B, "ANC", AM::IMMEDIATE, 2),
    UNSUPPORTED_OP(0x0C, "NOP", AM::ABSOLUTE, 4),
    OP(0x0D, "ORA", AM::ABSOLUTE, unimplemented, 4),
    OP(0x0E, "ASL", AM::ABSOLUTE, unimplemented, 6),
    UNSUPPORTED_OP(0x0F, "SLO", AM::ABSOLUTE, 6),

    /*
     * 0x10 - 0x1F
     */
    OP(0x10, "BPL", AM::RELATIVE, unimplemented, 2),
    OP(0x11, "ORA", AM::INDIRECT_INDEXED, unimplemented, 5),
    UNSUPPORTED_OP(0x12, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0x13, "SLO", AM::INDIRECT_INDEXED, 8),
    UNSUPPORTED_OP(0x14, "NOP", AM::ZERO_PAGE_X, 4),
    OP(0x15, "ORA", AM::ZERO_PAGE_X, unimplemented, 4),
    OP(0x16, "ASL", AM::ZERO_PAGE_X, unimplemented, 6),
    UNSUPPORTED_OP(0x17, "SLO", AM::ZERO_PAGE_X, 6),
    OP(0x18, "CLC", AM::IMPLICIT, unimplemented, 2),
    OP(0x19, "ORA", AM::ABSOLUTE_Y, unimplemented, 4),
    UNSUPPORTED_OP(0x1A, "NOP", AM::IMPLICIT, 2),
    UNSUPPORTED_OP(0x1B, "SLO", AM::ABSOLUTE_Y, 7),
    UNSUPPORTED_OP(0x1C, "NOP", AM::ABSOLUTE_X, 4),
    OP(0x1D, "ORA", AM::ABSOLUTE_X, unimplemented, 4),
    OP(0x1E, "ASL", AM::ABSOLUTE_X, unimplemented, 7),
    UNSUPPORTED_OP(0x1F, "SLO", AM::ABSOLUTE_X, 7),

    /*
     * 0x20 - 0x2F
     */
    OP(0x20, "JSR", AM::ABSOLUTE, unimplemented, 6),
    OP(0x21, "AND", AM::INDEXED_INDIRECT, unimplemented, 6),
    UNSUPPORTED_OP(0x22, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0x23, "RLA", AM::INDEXED_INDIRECT, 8),
    OP(0x24, "BIT", AM::ZERO_PAGE, unimplemented, 3),
    OP(0x25, "AND", AM::ZERO_PAGE, unimplemented, 3),
    OP(0x26, "ROL", AM::ZERO_PAGE, unimplemented, 5),
    UNSUPPORTED_OP(0x27, "RLA", AM::ZERO_PAGE, 5),
    OP(0x28, "PLP", AM::IMPLICIT, unimplemented, 4),
    OP(0x29, "AND", AM::IMMEDIATE, unimplemented, 2),
    OP(0x2A, "ROL", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0x2B, "ANC", AM::IMMEDIATE, 2),
    OP(0x2C, "BIT", AM::ABSOLUTE, unimplemented, 4),
    OP(0x2D, "AND", AM::ABSOLUTE, unimplemented, 4),
    OP(0x2E, "ROL", AM::ABSOLUTE, unimplemented, 6),
    UNSUPPORTED_OP(0x2F, "RLA", AM::ABSOLUTE, 6),

    /*
     * 0x30 - 0x3F
     */
    OP(0x30, "BMI", AM::RELATIVE, unimplemented, 2),
    OP(0x31, "AND", AM::INDIRECT_INDEXED, unimplemented, 5),
    UNSUPPORTED_OP(0x32, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0x33, "RLA", AM::INDIRECT_INDEXED, 8),
    UNSUPPORTED_OP(0x34, "NOP", AM::ZERO_PAGE_X, 4),
    OP(0x35, "AND", AM::ZERO_PAGE_X, unimplemented, 4),
    OP(0x36, "ROL", AM::ZERO_PAGE_X, unimplemented, 6),
    UNSUPPORTED_OP(0x37, "RLA", AM::ZERO_PAGE_X, 6),
    OP(0x38, "SEC", AM::IMPLICIT, unimplemented, 2),
    OP(0x39, "AND", AM::ABSOLUTE_Y, unimplemented, 4),
    UNSUPPORTED_OP(0x3A, "NOP", AM::IMPLICIT, 2),
    UNSUPPORTED_OP(0x3B, "RLA", AM::ABSOLUTE_Y, 7),
    UNSUPPORTED_OP(0x3C, "NOP", AM::ABSOLUTE_X, 4),
    OP(0x3D, "AND", AM::ABSOLUTE, unimplemented, 4),
    OP(0x3E, "ROL", AM::ABSOLUTE_X, unimplemented, 7),
    UNSUPPORTED_OP(0x3F, "RLA", AM::ABSOLUTE_X, 7),

    /*
     * 0x40 - 0x4F
     */
    OP(0x40, "RTI", AM::IMPLICIT, unimplemented, 6),
    OP(0x41, "EOR", AM::INDEXED_INDIRECT, unimplemented, 6),
    UNSUPPORTED_OP(0x42, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0x43, "SRE", AM::INDEXED_INDIRECT, 8),
    UNSUPPORTED_OP(0x44, "NOP", AM::ZERO_PAGE, 3),
    OP(0x45, "EOR", AM::ZERO_PAGE, unimplemented, 3),
    OP(0x46, "LSR", AM::ZERO_PAGE, unimplemented, 5),
    UNSUPPORTED_OP(0x47, "SRE", AM::ZERO_PAGE, 5),
    OP(0x48, "PHA", AM::IMPLICIT, unimplemented, 3),
    OP(0x49, "EOR", AM::IMMEDIATE, unimplemented, 2),
    OP(0x4A, "LSR", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0x4B, "ALR", AM::IMMEDIATE, 2),
    OP(0x4C, "JMP", AM::ABSOLUTE, unimplemented, 3),
    OP(0x4D, "EOR", AM::ABSOLUTE, unimplemented, 4),
    OP(0x4E, "LSR", AM::ABSOLUTE, unimplemented, 6),
    UNSUPPORTED_OP(0x4F, "SRE", AM::ABSOLUTE, 6),

    /*
     * 0x50 - 0x5F
     */
    OP(0x50, "BVC", AM::RELATIVE, unimplemented, 2),
    OP(0x51, "EOR", AM::INDIRECT_INDEXED, unimplemented, 5),
    UNSUPPORTED_OP(0x52, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0x53, "SRE", AM::INDEXED_INDIRECT, 0),
    UNSUPPORTED_OP(0x54, "NOP", AM::ZERO_PAGE_X, 4),
    OP(0x55, "EOR", AM::ZERO_PAGE_X, unimplemented, 4),
    OP(0x56, "LSR", AM::ZERO_PAGE_X, unimplemented, 6),
    UNSUPPORTED_OP(0x57, "SRE", AM::ZERO_PAGE_X, 6),
    OP(0x58, "CLI", AM::IMPLICIT, unimplemented, 2),
    OP(0x59, "EOR", AM::ABSOLUTE_Y, unimplemented, 4),
    UNSUPPORTED_OP(0x5A, "NOP", AM::IMPLICIT, 2),
    UNSUPPORTED_OP(0x5B, "SRE", AM::ABSOLUTE_Y, 7),
    UNSUPPORTED_OP(0x5C, "NOP", AM::ABSOLUTE_X, 4),
    OP(0x5D, "EOR", AM::ABSOLUTE_X, unimplemented, 4),
    OP(0x5E, "LSR", AM::ABSOLUTE_X, unimplemented, 7),
    UNSUPPORTED_OP(0x5F, "SRE", AM::ABSOLUTE_X, 7),

    /*
     * 0x60 - 0x6F
     */
    OP(0x60, "RTS", AM::IMPLICIT, unimplemented, 6),
    OP(0x61, "ADC", AM::INDIRECT_INDEXED, unimplemented, 6),
    UNSUPPORTED_OP(0x62, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0x63, "RRA", AM::INDEXED_INDIRECT, 8),
    UNSUPPORTED_OP(0x64, "NOP", AM::ZERO_PAGE, 3),
    OP(0x65, "ADC", AM::ZERO_PAGE, unimplemented, 3),
    OP(0x66, "ROR", AM::ZERO_PAGE, unimplemented, 5),
    UNSUPPORTED_OP(0x67, "RRA", AM::ZERO_PAGE, 5),
    OP(0x68, "PLA", AM::IMPLICIT, unimplemented, 4),
    OP(0x69, "ADC", AM::IMMEDIATE, unimplemented, 2),
    OP(0x6A, "ROR", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0x6B, "ARR", AM::IMMEDIATE, 2),
    OP(0x6C, "JMP", AM::INDIRECT, unimplemented, 5),
    OP(0x6D, "ADC", AM::ABSOLUTE, unimplemented, 4),
    OP(0x6E, "ROR", AM::ABSOLUTE, unimplemented, 6),
    UNSUPPORTED_OP(0x6F, "RRA", AM::ABSOLUTE, 6),

    /*
     * 0x70 - 0x7F
     */
    OP(0x70, "BVS", AM::RELATIVE, unimplemented, 2),
    OP(0x71, "ADC", AM::INDIRECT_INDEXED, unimplemented, 5),
    UNSUPPORTED_OP(0x72, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0x73, "RRA", AM::INDIRECT_INDEXED, 8),
    UNSUPPORTED_OP(0x74, "NOP", AM::ZERO_PAGE_X, 4),
    OP(0x75, "ADC", AM::ZERO_PAGE_X, unimplemented, 4),
    OP(0x76, "ROR", AM::ZERO_PAGE_X, unimplemented, 6),
    UNSUPPORTED_OP(0x77, "RRA", AM::ZERO_PAGE_X, 6),
    OP(0x78, "SEI", AM::IMPLICIT, unimplemented, 2),
    OP(0x79, "ADC", AM::ABSOLUTE_Y, unimplemented, 4),
    UNSUPPORTED_OP(0x7A, "NOP", AM::IMPLICIT, 2),
    UNSUPPORTED_OP(0x7B, "RRA", AM::ABSOLUTE_Y, 7),
    UNSUPPORTED_OP(0x7C, "NOP", AM::ABSOLUTE_X, 4),
    OP(0x7D, "ADC", AM::ABSOLUTE_X, unimplemented, 4),
    OP(0x7E, "ROR", AM::ABSOLUTE_X, unimplemented, 7),
    UNSUPPORTED_OP(0x7F, "RRA", AM::ABSOLUTE_X, 7),

    /*
     * 0x80 - 0x8F
     */
    UNSUPPORTED_OP(0x80, "NOP", AM::IMMEDIATE, 2),
    OP(0x81, "STA", AM::INDEXED_INDIRECT, unimplemented, 6),
    UNSUPPORTED_OP(0x82, "NOP", AM::IMMEDIATE, 2),
    UNSUPPORTED_OP(0x83, "SAX", AM::INDEXED_INDIRECT, 6),
    OP(0x84, "STY", AM::ZERO_PAGE, unimplemented, 3),
    OP(0x85, "STA", AM::ZERO_PAGE, unimplemented, 3),
    OP(0x86, "STX", AM::ZERO_PAGE, unimplemented, 3),
    UNSUPPORTED_OP(0x87, "SAX", AM::ZERO_PAGE, 3),
    OP(0x88, "DEY", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0x89, "NOP", AM::IMMEDIATE, 2),
    OP(0x8A, "TXA", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0x8B, "XAA", AM::IMMEDIATE, 2),
    OP(0x8C, "STY", AM::ABSOLUTE, unimplemented, 4),
    OP(0x8D, "STA", AM::ABSOLUTE, unimplemented, 4),
    OP(0x8E, "STX", AM::ABSOLUTE, unimplemented, 4),
    UNSUPPORTED_OP(0x8F, "SAX", AM::ABSOLUTE, 4),

    /*
     * 0x90 - 0x9F
     */
    OP(0x90, "BCC", AM::RELATIVE, unimplemented, 2),
    OP(0x91, "STA", AM::INDIRECT_INDEXED, unimplemented, 6),
    UNSUPPORTED_OP(0x92, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0x93, "AHX", AM::INDIRECT_INDEXED, 6),
    OP(0x94, "STY", AM::ZERO_PAGE_X, unimplemented, 4),
    OP(0x95, "STA", AM::ZERO_PAGE_X, unimplemented, 4),
    OP(0x96, "STX", AM::ZERO_PAGE_Y, unimplemented, 4),
    UNSUPPORTED_OP(0x97, "SAX", AM::ZERO_PAGE_Y, 4),
    OP(0x98, "TYA", AM::IMPLICIT, unimplemented, 2),
    OP(0x99, "STA", AM::ABSOLUTE_Y, unimplemented, 5),
    OP(0x9A, "TXS", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0x9B, "TAS", AM::ABSOLUTE_Y, 5),
    UNSUPPORTED_OP(0x9C, "SHY", AM::ABSOLUTE_X, 5),
    OP(0x9D, "STA", AM::ABSOLUTE_X, unimplemented, 5),
    UNSUPPORTED_OP(0x9E, "SHX", AM::ABSOLUTE_Y, 5),
    UNSUPPORTED_OP(0x9F, "AHX", AM::ABSOLUTE_Y, 5),

    /*
     * 0xA0 - 0xAF
     */
    OP(0xA0, "LDY", AM::IMMEDIATE, Op::ldy, 2),
    OP(0xA1, "LDA", AM::INDEXED_INDIRECT, Op::lda, 6),
    OP(0xA2, "LDX", AM::IMMEDIATE, Op::ldx, 2),
    UNSUPPORTED_OP(0xA3, "LAX", AM::INDEXED_INDIRECT, 6),
    OP(0xA4, "LDY", AM::ZERO_PAGE, Op::ldy, 3),
    OP(0xA5, "LDA", AM::ZERO_PAGE, Op::lda, 3),
    OP(0xA6, "LDX", AM::ZERO_PAGE, Op::ldx, 3),
    UNSUPPORTED_OP(0xA7, "LAX", AM::ZERO_PAGE, 3),
    OP(0xA8, "TAY", AM::IMPLICIT, unimplemented, 2),
    OP(0xA9, "LDA", AM::IMMEDIATE, Op::lda, 2),
    OP(0xAA, "TAX", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0xAB, "LAX", AM::IMMEDIATE, 2),
    OP(0xAC, "LDY", AM::ABSOLUTE, Op::ldy, 4),
    OP(0xAD, "LDA", AM::ABSOLUTE, Op::lda, 4),
    OP(0xAE, "LDX", AM::ABSOLUTE, Op::ldx, 4),
    UNSUPPORTED_OP(0xAF, "LAX", AM::ABSOLUTE, 4),

    /*
     * 0xB0 - 0xBF
     */
    OP(0xB0, "BCS", AM::RELATIVE, unimplemented, 2),
    OP(0xB1, "LDA", AM::INDIRECT_INDEXED, Op::lda, 5),
    UNSUPPORTED_OP(0xB2, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0xB3, "LAX", AM::INDIRECT_INDEXED, 5),
    OP(0xB4, "LDY", AM::ZERO_PAGE_X, Op::ldy, 4),
    OP(0xB5, "LDA", AM::ZERO_PAGE_X, Op::lda, 4),
    OP(0xB6, "LDX", AM::ZERO_PAGE_Y, Op::ldx, 4),
    UNSUPPORTED_OP(0xB7, "LAX", AM::ZERO_PAGE_Y, 4),
    OP(0xB8, "CLV", AM::IMPLICIT, unimplemented, 2),
    OP(0xB9, "LDA", AM::ABSOLUTE_Y, Op::lda, 4),
    OP(0xBA, "TSX", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0xBB, "LAS", AM::ABSOLUTE_Y, 4),
    OP(0xBC, "LDY", AM::ABSOLUTE_X, Op::ldy, 4),
    OP(0xBD, "LDA", AM::ABSOLUTE_X, Op::lda, 4),
    OP(0xBE, "LDX", AM::ABSOLUTE_Y, Op::ldx, 4),
    UNSUPPORTED_OP(0xBF, "LAX", AM::ABSOLUTE_Y, 4),

    /*
     * 0xC0 - 0xCF
     */
    OP(0xC0, "CPY", AM::IMMEDIATE, unimplemented, 2),
    OP(0xC1, "CMP", AM::INDEXED_INDIRECT, unimplemented, 6),
    UNSUPPORTED_OP(0xC2, "NOP", AM::IMMEDIATE, 2),
    UNSUPPORTED_OP(0xC3, "DCP", AM::INDEXED_INDIRECT, 8),
    OP(0xC4, "CPY", AM::ZERO_PAGE, unimplemented, 3),
    OP(0xC5, "CMP", AM::ZERO_PAGE, unimplemented, 3),
    OP(0xC6, "DEC", AM::ZERO_PAGE, unimplemented, 5),
    UNSUPPORTED_OP(0xC7, "DCP", AM::ZERO_PAGE, 5),
    OP(0xC8, "INY", AM::IMPLICIT, unimplemented, 2),
    OP(0xC9, "CMP", AM::IMMEDIATE, unimplemented, 2),
    OP(0xCA, "DEX", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0xCB, "AXS", AM::IMMEDIATE, 2),
    OP(0xCC, "CPY", AM::ABSOLUTE, unimplemented, 4),
    OP(0xCD, "CMP", AM::ABSOLUTE, unimplemented, 4),
    OP(0xCE, "DEC", AM::ABSOLUTE, unimplemented, 6),
    UNSUPPORTED_OP(0xCF, "DCP", AM::ABSOLUTE, 6),

    /*
     * 0xD0 - 0xDF
     */
    OP(0xD0, "BNE", AM::RELATIVE, unimplemented, 2),
    OP(0xD1, "CMP", AM::INDIRECT_INDEXED, unimplemented, 5),
    UNSUPPORTED_OP(0xD2, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0xD3, "DCP", AM::INDIRECT_INDEXED, 8),
    UNSUPPORTED_OP(0xD4, "NOP", AM::ZERO_PAGE_X, 4),
    OP(0xD5, "CMP", AM::ZERO_PAGE_X, unimplemented, 4),
    OP(0xD6, "DEC", AM::ZERO_PAGE_X, unimplemented, 6),
    UNSUPPORTED_OP(0xD7, "DCP", AM::ZERO_PAGE_X, 6),
    OP(0xD8, "CLD", AM::IMPLICIT, unimplemented, 2),
    OP(0xD9, "CMP", AM::ABSOLUTE_Y, unimplemented, 4),
    UNSUPPORTED_OP(0xDA, "NOP", AM::IMPLICIT, 2),
    UNSUPPORTED_OP(0xDB, "DCP", AM::ABSOLUTE_Y, 7),
    UNSUPPORTED_OP(0xDC, "NOP", AM::ABSOLUTE_X, 4),
    OP(0xDD, "CMP", AM::ABSOLUTE_X, unimplemented, 4),
    OP(0xDE, "DEC", AM::ABSOLUTE_X, unimplemented, 7),
    UNSUPPORTED_OP(0xDF, "DCP", AM::ABSOLUTE_X, 7),

    /*
     * 0xE0 - 0xEF
     */
    OP(0xE0, "CPX", AM::IMMEDIATE, unimplemented, 2),
    OP(0xE1, "SBC", AM::INDEXED_INDIRECT, unimplemented, 6),
    UNSUPPORTED_OP(0xE2, "NOP", AM::IMMEDIATE, 2),
    UNSUPPORTED_OP(0xE3, "ISC", AM::INDEXED_INDIRECT, 8),
    OP(0xE4, "CPX", AM::ZERO_PAGE, unimplemented, 3),
    OP(0xE5, "SBC", AM::ZERO_PAGE, unimplemented, 3),
    OP(0xE6, "INC", AM::ZERO_PAGE, unimplemented, 5),
    UNSUPPORTED_OP(0xE7, "ISC", AM::ZERO_PAGE, 5),
    OP(0xE8, "INX", AM::IMPLICIT, unimplemented, 2),
    OP(0xE9, "SBC", AM::IMMEDIATE, unimplemented, 2),
    OP(0xEA, "NOP", AM::IMPLICIT, unimplemented, 2),
    UNSUPPORTED_OP(0xEB, "SBC", AM::IMMEDIATE, 2),
    OP(0xEC, "CPX", AM::ABSOLUTE, unimplemented, 4),
    OP(0xED, "SBC", AM::ABSOLUTE, unimplemented, 4),
    OP(0xEE, "INC", AM::ABSOLUTE, unimplemented, 6),
    UNSUPPORTED_OP(0xEF, "ISC", AM::ABSOLUTE, 6),

    /*
     * 0xF0 - 0xFF
     */
    OP(0xF0, "BEQ", AM::RELATIVE, unimplemented, 2),
    OP(0xF1, "SBC", AM::INDIRECT_INDEXED, unimplemented, 5),
    UNSUPPORTED_OP(0xF2, "KIL", AM::IMPLICIT, 0),
    UNSUPPORTED_OP(0xF3, "ISC", AM::INDIRECT_INDEXED, 8),
    UNSUPPORTED_OP(0xF4, "NOP", AM::ZERO_PAGE_X, 4),
    OP(0xF5, "SBC", AM::ZERO_PAGE_X, unimplemented, 4),
    OP(0xF6, "INC", AM::ZERO_PAGE_X, unimplemented, 6),
    UNSUPPORTED_OP(0xF7, "ISC", AM::ZERO_PAGE_X, 6),
    OP(0xF8, "SED", AM::IMPLICIT, unimplemented, 2),
    OP(0xF9, "SBC", AM::ABSOLUTE_Y, unimplemented, 4),
    UNSUPPORTED_OP(0xFA, "NOP", AM::IMPLICIT, 2),
    UNSUPPORTED_OP(0xFB, "ISC", AM::ABSOLUTE_X, 7),
    UNSUPPORTED_OP(0xFC, "NOP", AM::ABSOLUTE_X, 4),
    OP(0xFD, "SBC", AM::ABSOLUTE_X, unimplemented, 4),
    OP(0xFE, "INC", AM::ABSOLUTE_X, unimplemented, 7),
    UNSUPPORTED_OP(0xFF, "ISC", AM::ABSOLUTE_X, 7)
};

const Opcode *Op::decode(uint8_t op) {
    return op < sizeof(OPCODES) / sizeof(Opcode) ? &OPCODES[op] : nullptr;
}

size_t Op::getAddressingModeOperandCount(Op::AddressingMode mode) {
    switch (mode) {
        case AM::IMPLICIT:
            return 0;

        case AM::IMMEDIATE:
        case AM::ZERO_PAGE:
        case AM::ZERO_PAGE_X:
        case AM::ZERO_PAGE_Y:
        case AM::INDIRECT_INDEXED:
        case AM::INDEXED_INDIRECT:
        case AM::RELATIVE:
            return 1;

        case AM::ABSOLUTE:
        case AM::ABSOLUTE_X:
        case AM::ABSOLUTE_Y:
        case AM::INDIRECT:
            return 2;
    }
}

uint8_t Op::address(CPU *cpu, Op::AddressingMode mode, const Op::Operands &operands) {
    RegisterFile *r = cpu->getRegs();
    Memory *mem = cpu->getNES()->getMemory();

    switch (mode) {
        case AM::IMPLICIT:
            return 0;

        case AM::IMMEDIATE:
            return operands[0];

        case AM::ZERO_PAGE:
            return mem->read((Address)operands[0]);

        case AM::ZERO_PAGE_X:
            return mem->read((Address)(operands[0] + r->x));

        case AM::ZERO_PAGE_Y:
            return mem->read((Address)(operands[0] + r->y));

        case AM::ABSOLUTE:
            return mem->read(Utils::combineUint8sLE(operands[1], operands[0]));

        case AM::ABSOLUTE_X:
            return mem->read(Utils::combineUint8sLE(operands[1], operands[0]) + r->x);

        case AM::ABSOLUTE_Y:
            return mem->read(Utils::combineUint8sLE(operands[1], operands[0]) + r->y);

        default:
            // TODO: Implement.
            return 0;
    }
}

void Op::setNegativeFlag(CPU *cpu, uint8_t value) {
    cpu->setFlag(CPUFlag::NEGATIVE, ((value >> 7) & 0b1) == 1);
}

void Op::setZeroFlag(CPU *cpu, uint8_t value) {
    cpu->setFlag(CPUFlag::ZERO, value == 0);
}

void Op::setNZFlags(CPU *cpu, uint8_t value) {
    setNegativeFlag(cpu, value);
    setZeroFlag(cpu, value);
}

template<typename T>
static void writeHexToSS(std::ostringstream &stream, T x) {
    stream << std::uppercase << std::setfill('0') << std::setw((int)(sizeof(T) * 2)) << std::hex << (size_t)x;
}

void Op::formatInstruction(const Opcode *opcode, const Op::Operands &operands, std::string *outFormatted) {
    if (!outFormatted) {
        return;
    }

    std::ostringstream stream;
    stream << opcode->name;

    switch (opcode->mode) {
        case AM::IMPLICIT:
            break;

        case AM::IMMEDIATE:
            stream << " #$";
            writeHexToSS(stream, operands[0]);
            break;

        case AM::ZERO_PAGE:
            stream << " $";
            writeHexToSS(stream, operands[0]);
            break;

        case AM::ZERO_PAGE_X:
            stream << " $";
            writeHexToSS(stream, operands[0]);
            stream << ", X";
            break;

        case AM::ZERO_PAGE_Y:
            stream << " $";
            writeHexToSS(stream, operands[0]);
            stream << ", Y";
            break;

        case AM::INDEXED_INDIRECT:
            stream << " ($";
            writeHexToSS(stream, operands[0]);
            stream << ", X)";
            break;

        case AM::INDIRECT_INDEXED:
            stream << " ($";
            writeHexToSS(stream, operands[0]);
            stream << "), Y";
            break;

        case AM::ABSOLUTE:
            stream << " $";
            writeHexToSS(stream, Utils::combineUint8sLE(operands[0], operands[1]));
            break;

        case AM::ABSOLUTE_X:
            stream << " $";
            writeHexToSS(stream, Utils::combineUint8sLE(operands[0], operands[1]));
            stream << ", X";
            break;

        case AM::ABSOLUTE_Y:
            stream << " $";
            writeHexToSS(stream, Utils::combineUint8sLE(operands[0], operands[1]));
            stream << ", Y";
            break;

        case AM::INDIRECT:
            stream << " ($";
            writeHexToSS(stream, Utils::combineUint8sLE(operands[0], operands[1]));
            stream << ")";
            break;

        case AM::RELATIVE:
            stream << " *" << (int8_t)operands[0];
            break;
    }

    *outFormatted = stream.str();
}
