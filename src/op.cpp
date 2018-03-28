#include "op.h"

#include "cpu.h"
#include "nes.h"
#include "memory.h"
#include "op/irq.h"
#include "op/loads.h"
#include "op/stores.h"
#include "op/transfers.h"
#include "op/flags.h"
#include "op/control.h"
#include "op/stack.h"
#include "op/arith.h"
#include "utils.h"

#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

static unsigned int unsupported(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    std::cerr << "Opcode $";
    Utils::writeHexToStream(std::cerr, opcode->code);
    std::cerr << " (" << opcode->name << ") is not supported!\n";
    return 0;
}

static unsigned int nop(CPU *cpu, Op::Operands &operands, const Op::Opcode *opcode) {
    return 0;
}

using Opcode = Op::Opcode;
using AM = Op::AddressingMode;

static const Opcode OPCODES[] = {
    /*
     * 0x00 - 0x0F
     */
    Opcode { 0x00, 7, "BRK", AM::IMPLICIT, Op::brk },
    Opcode { 0x01, 6, "ORA", AM::INDEXED_INDIRECT, Op::ora },
    Opcode { 0x02, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0x03, 8, "SLO", AM::INDEXED_INDIRECT, unsupported },
    Opcode { 0x04, 3, "NOP", AM::ZERO_PAGE, unsupported },
    Opcode { 0x05, 3, "ORA", AM::ZERO_PAGE, Op::ora },
    Opcode { 0x06, 5, "ASL", AM::ZERO_PAGE, Op::asl },
    Opcode { 0x07, 5, "SLO", AM::ZERO_PAGE, unsupported },
    Opcode { 0x08, 3, "PHP", AM::IMPLICIT, Op::php },
    Opcode { 0x09, 2, "ORA", AM::IMMEDIATE, Op::ora },
    Opcode { 0x0A, 2, "ASL", AM::ACCUMULATOR, Op::asl },
    Opcode { 0x0B, 2, "ANC", AM::IMMEDIATE, unsupported },
    Opcode { 0x0C, 4, "NOP", AM::ABSOLUTE, unsupported },
    Opcode { 0x0D, 4, "ORA", AM::ABSOLUTE, Op::ora },
    Opcode { 0x0E, 6, "ASL", AM::ABSOLUTE, Op::asl },
    Opcode { 0x0F, 6, "SLO", AM::ABSOLUTE, unsupported },

    /*
     * 0x10 - 0x1F
     */
    Opcode { 0x10, 2, "BPL", AM::RELATIVE, Op::bpl },
    Opcode { 0x11, 5, "ORA", AM::INDIRECT_INDEXED, Op::ora },
    Opcode { 0x12, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0x13, 8, "SLO", AM::INDIRECT_INDEXED, unsupported },
    Opcode { 0x14, 4, "NOP", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0x15, 4, "ORA", AM::ZERO_PAGE_X, Op::ora },
    Opcode { 0x16, 6, "ASL", AM::ZERO_PAGE_X, Op::asl },
    Opcode { 0x17, 6, "SLO", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0x18, 2, "CLC", AM::IMPLICIT, Op::clc },
    Opcode { 0x19, 4, "ORA", AM::ABSOLUTE_Y, Op::ora },
    Opcode { 0x1A, 2, "NOP", AM::IMPLICIT, unsupported },
    Opcode { 0x1B, 7, "SLO", AM::ABSOLUTE_Y, unsupported },
    Opcode { 0x1C, 4, "NOP", AM::ABSOLUTE_X, unsupported },
    Opcode { 0x1D, 4, "ORA", AM::ABSOLUTE_X, Op::ora },
    Opcode { 0x1E, 7, "ASL", AM::ABSOLUTE_X, Op::asl },
    Opcode { 0x1F, 7, "SLO", AM::ABSOLUTE_X, unsupported },

    /*
     * 0x20 - 0x2F
     */
    Opcode { 0x20, 6, "JSR", AM::ABSOLUTE, Op::jsr },
    Opcode { 0x21, 6, "AND", AM::INDEXED_INDIRECT, Op::_and },
    Opcode { 0x22, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0x23, 8, "RLA", AM::INDEXED_INDIRECT, unsupported },
    Opcode { 0x24, 3, "BIT", AM::ZERO_PAGE, Op::bit },
    Opcode { 0x25, 3, "AND", AM::ZERO_PAGE, Op::_and },
    Opcode { 0x26, 5, "ROL", AM::ZERO_PAGE, Op::rol },
    Opcode { 0x27, 5, "RLA", AM::ZERO_PAGE, unsupported },
    Opcode { 0x28, 4, "PLP", AM::IMPLICIT, Op::plp },
    Opcode { 0x29, 2, "AND", AM::IMMEDIATE, Op::_and },
    Opcode { 0x2A, 2, "ROL", AM::ACCUMULATOR, Op::rol },
    Opcode { 0x2B, 2, "ANC", AM::IMMEDIATE, unsupported },
    Opcode { 0x2C, 4, "BIT", AM::ABSOLUTE, Op::bit },
    Opcode { 0x2D, 4, "AND", AM::ABSOLUTE, Op::_and },
    Opcode { 0x2E, 6, "ROL", AM::ABSOLUTE, Op::rol },
    Opcode { 0x2F, 6, "RLA", AM::ABSOLUTE, unsupported },

    /*
     * 0x30 - 0x3F
     */
    Opcode { 0x30, 2, "BMI", AM::RELATIVE, Op::bmi },
    Opcode { 0x31, 5, "AND", AM::INDIRECT_INDEXED, Op::_and },
    Opcode { 0x32, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0x33, 8, "RLA", AM::INDIRECT_INDEXED, unsupported },
    Opcode { 0x34, 4, "NOP", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0x35, 4, "AND", AM::ZERO_PAGE_X, Op::_and },
    Opcode { 0x36, 6, "ROL", AM::ZERO_PAGE_X, Op::rol },
    Opcode { 0x37, 6, "RLA", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0x38, 2, "SEC", AM::IMPLICIT, Op::sec },
    Opcode { 0x39, 4, "AND", AM::ABSOLUTE_Y, Op::_and },
    Opcode { 0x3A, 2, "NOP", AM::IMPLICIT, unsupported },
    Opcode { 0x3B, 7, "RLA", AM::ABSOLUTE_Y, unsupported },
    Opcode { 0x3C, 4, "NOP", AM::ABSOLUTE_X, unsupported },
    Opcode { 0x3D, 4, "AND", AM::ABSOLUTE, Op::_and },
    Opcode { 0x3E, 7, "ROL", AM::ABSOLUTE_X, Op::rol },
    Opcode { 0x3F, 7, "RLA", AM::ABSOLUTE_X, unsupported },

    /*
     * 0x40 - 0x4F
     */
    Opcode { 0x40, 6, "RTI", AM::IMPLICIT, Op::rti },
    Opcode { 0x41, 6, "EOR", AM::INDEXED_INDIRECT, Op::eor },
    Opcode { 0x42, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0x43, 8, "SRE", AM::INDEXED_INDIRECT, unsupported },
    Opcode { 0x44, 3, "NOP", AM::ZERO_PAGE, unsupported },
    Opcode { 0x45, 3, "EOR", AM::ZERO_PAGE, Op::eor },
    Opcode { 0x46, 5, "LSR", AM::ZERO_PAGE, Op::lsr },
    Opcode { 0x47, 5, "SRE", AM::ZERO_PAGE, unsupported },
    Opcode { 0x48, 3, "PHA", AM::IMPLICIT, Op::pha },
    Opcode { 0x49, 2, "EOR", AM::IMMEDIATE, Op::eor },
    Opcode { 0x4A, 2, "LSR", AM::ACCUMULATOR, Op::lsr },
    Opcode { 0x4B, 2, "ALR", AM::IMMEDIATE, unsupported },
    Opcode { 0x4C, 3, "JMP", AM::ABSOLUTE, Op::jmp },
    Opcode { 0x4D, 4, "EOR", AM::ABSOLUTE, Op::eor },
    Opcode { 0x4E, 6, "LSR", AM::ABSOLUTE, Op::lsr },
    Opcode { 0x4F, 6, "SRE", AM::ABSOLUTE, unsupported },

    /*
     * 0x50 - 0x5F
     */
    Opcode { 0x50, 2, "BVC", AM::RELATIVE, Op::bvc },
    Opcode { 0x51, 5, "EOR", AM::INDIRECT_INDEXED, Op::eor },
    Opcode { 0x52, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0x53, 0, "SRE", AM::INDEXED_INDIRECT, unsupported },
    Opcode { 0x54, 4, "NOP", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0x55, 4, "EOR", AM::ZERO_PAGE_X, Op::eor },
    Opcode { 0x56, 6, "LSR", AM::ZERO_PAGE_X, Op::lsr },
    Opcode { 0x57, 6, "SRE", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0x58, 2, "CLI", AM::IMPLICIT, Op::cli },
    Opcode { 0x59, 4, "EOR", AM::ABSOLUTE_Y, Op::eor },
    Opcode { 0x5A, 2, "NOP", AM::IMPLICIT, unsupported },
    Opcode { 0x5B, 7, "SRE", AM::ABSOLUTE_Y, unsupported },
    Opcode { 0x5C, 4, "NOP", AM::ABSOLUTE_X, unsupported },
    Opcode { 0x5D, 4, "EOR", AM::ABSOLUTE_X, Op::eor },
    Opcode { 0x5E, 7, "LSR", AM::ABSOLUTE_X, Op::lsr },
    Opcode { 0x5F, 7, "SRE", AM::ABSOLUTE_X, unsupported },

    /*
     * 0x60 - 0x6F
     */
    Opcode { 0x60, 6, "RTS", AM::IMPLICIT, Op::rts },
    Opcode { 0x61, 6, "ADC", AM::INDIRECT_INDEXED, Op::adc },
    Opcode { 0x62, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0x63, 8, "RRA", AM::INDEXED_INDIRECT, unsupported },
    Opcode { 0x64, 3, "NOP", AM::ZERO_PAGE, unsupported },
    Opcode { 0x65, 3, "ADC", AM::ZERO_PAGE, Op::adc },
    Opcode { 0x66, 5, "ROR", AM::ZERO_PAGE, Op::ror },
    Opcode { 0x67, 5, "RRA", AM::ZERO_PAGE, unsupported },
    Opcode { 0x68, 4, "PLA", AM::IMPLICIT, Op::pla },
    Opcode { 0x69, 2, "ADC", AM::IMMEDIATE, Op::adc },
    Opcode { 0x6A, 2, "ROR", AM::ACCUMULATOR, Op::ror },
    Opcode { 0x6B, 2, "ARR", AM::IMMEDIATE, unsupported },
    Opcode { 0x6C, 5, "JMP", AM::INDIRECT, Op::jmp },
    Opcode { 0x6D, 4, "ADC", AM::ABSOLUTE, Op::adc },
    Opcode { 0x6E, 6, "ROR", AM::ABSOLUTE, Op::ror },
    Opcode { 0x6F, 6, "RRA", AM::ABSOLUTE, unsupported },

    /*
     * 0x70 - 0x7F
     */
    Opcode { 0x70, 2, "BVS", AM::RELATIVE, Op::bvs },
    Opcode { 0x71, 5, "ADC", AM::INDIRECT_INDEXED, Op::adc },
    Opcode { 0x72, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0x73, 8, "RRA", AM::INDIRECT_INDEXED, unsupported },
    Opcode { 0x74, 4, "NOP", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0x75, 4, "ADC", AM::ZERO_PAGE_X, Op::adc },
    Opcode { 0x76, 6, "ROR", AM::ZERO_PAGE_X, Op::ror },
    Opcode { 0x77, 6, "RRA", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0x78, 2, "SEI", AM::IMPLICIT, Op::sei },
    Opcode { 0x79, 4, "ADC", AM::ABSOLUTE_Y, Op::adc },
    Opcode { 0x7A, 2, "NOP", AM::IMPLICIT, unsupported },
    Opcode { 0x7B, 7, "RRA", AM::ABSOLUTE_Y, unsupported },
    Opcode { 0x7C, 4, "NOP", AM::ABSOLUTE_X, unsupported },
    Opcode { 0x7D, 4, "ADC", AM::ABSOLUTE_X, Op::adc },
    Opcode { 0x7E, 7, "ROR", AM::ABSOLUTE_X, Op::ror },
    Opcode { 0x7F, 7, "RRA", AM::ABSOLUTE_X, unsupported },

    /*
     * 0x80 - 0x8F
     */
    Opcode { 0x80, 2, "NOP", AM::IMMEDIATE, unsupported },
    Opcode { 0x81, 6, "STA", AM::INDEXED_INDIRECT, Op::sta },
    Opcode { 0x82, 2, "NOP", AM::IMMEDIATE, unsupported },
    Opcode { 0x83, 6, "SAX", AM::INDEXED_INDIRECT, unsupported },
    Opcode { 0x84, 3, "STY", AM::ZERO_PAGE, Op::sty },
    Opcode { 0x85, 3, "STA", AM::ZERO_PAGE, Op::sta },
    Opcode { 0x86, 3, "STX", AM::ZERO_PAGE, Op::stx },
    Opcode { 0x87, 3, "SAX", AM::ZERO_PAGE, unsupported },
    Opcode { 0x88, 2, "DEY", AM::IMPLICIT, Op::dey },
    Opcode { 0x89, 2, "NOP", AM::IMMEDIATE, unsupported },
    Opcode { 0x8A, 2, "TXA", AM::IMPLICIT, Op::txa },
    Opcode { 0x8B, 2, "XAA", AM::IMMEDIATE, unsupported },
    Opcode { 0x8C, 4, "STY", AM::ABSOLUTE, Op::sty },
    Opcode { 0x8D, 4, "STA", AM::ABSOLUTE, Op::sta },
    Opcode { 0x8E, 4, "STX", AM::ABSOLUTE, Op::stx },
    Opcode { 0x8F, 4, "SAX", AM::ABSOLUTE, unsupported },

    /*
     * 0x90 - 0x9F
     */
    Opcode { 0x90, 2, "BCC", AM::RELATIVE, Op::bcc },
    Opcode { 0x91, 6, "STA", AM::INDIRECT_INDEXED, Op::sta },
    Opcode { 0x92, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0x93, 6, "AHX", AM::INDIRECT_INDEXED, unsupported },
    Opcode { 0x94, 4, "STY", AM::ZERO_PAGE_X, Op::sty },
    Opcode { 0x95, 4, "STA", AM::ZERO_PAGE_X, Op::sta },
    Opcode { 0x96, 4, "STX", AM::ZERO_PAGE_Y, Op::stx },
    Opcode { 0x97, 4, "SAX", AM::ZERO_PAGE_Y, unsupported },
    Opcode { 0x98, 2, "TYA", AM::IMPLICIT, Op::tya },
    Opcode { 0x99, 5, "STA", AM::ABSOLUTE_Y, Op::sta },
    Opcode { 0x9A, 2, "TXS", AM::IMPLICIT, Op::txs },
    Opcode { 0x9B, 5, "TAS", AM::ABSOLUTE_Y, unsupported },
    Opcode { 0x9C, 5, "SHY", AM::ABSOLUTE_X, unsupported },
    Opcode { 0x9D, 5, "STA", AM::ABSOLUTE_X, Op::sta },
    Opcode { 0x9E, 5, "SHX", AM::ABSOLUTE_Y, unsupported },
    Opcode { 0x9F, 5, "AHX", AM::ABSOLUTE_Y, unsupported },

    /*
     * 0xA0 - 0xAF
     */
    Opcode { 0xA0, 2, "LDY", AM::IMMEDIATE, Op::ldy },
    Opcode { 0xA1, 6, "LDA", AM::INDEXED_INDIRECT, Op::lda },
    Opcode { 0xA2, 2, "LDX", AM::IMMEDIATE, Op::ldx },
    Opcode { 0xA3, 6, "LAX", AM::INDEXED_INDIRECT, unsupported },
    Opcode { 0xA4, 3, "LDY", AM::ZERO_PAGE, Op::ldy },
    Opcode { 0xA5, 3, "LDA", AM::ZERO_PAGE, Op::lda },
    Opcode { 0xA6, 3, "LDX", AM::ZERO_PAGE, Op::ldx },
    Opcode { 0xA7, 3, "LAX", AM::ZERO_PAGE, unsupported },
    Opcode { 0xA8, 2, "TAY", AM::IMPLICIT, Op::tay },
    Opcode { 0xA9, 2, "LDA", AM::IMMEDIATE, Op::lda },
    Opcode { 0xAA, 2, "TAX", AM::IMPLICIT, Op::tax },
    Opcode { 0xAB, 2, "LAX", AM::IMMEDIATE, unsupported },
    Opcode { 0xAC, 4, "LDY", AM::ABSOLUTE, Op::ldy },
    Opcode { 0xAD, 4, "LDA", AM::ABSOLUTE, Op::lda },
    Opcode { 0xAE, 4, "LDX", AM::ABSOLUTE, Op::ldx },
    Opcode { 0xAF, 4, "LAX", AM::ABSOLUTE, unsupported },

    /*
     * 0xB0 - 0xBF
     */
    Opcode { 0xB0, 2, "BCS", AM::RELATIVE, Op::bcs },
    Opcode { 0xB1, 5, "LDA", AM::INDIRECT_INDEXED, Op::lda },
    Opcode { 0xB2, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0xB3, 5, "LAX", AM::INDIRECT_INDEXED, unsupported },
    Opcode { 0xB4, 4, "LDY", AM::ZERO_PAGE_X, Op::ldy },
    Opcode { 0xB5, 4, "LDA", AM::ZERO_PAGE_X, Op::lda },
    Opcode { 0xB6, 4, "LDX", AM::ZERO_PAGE_Y, Op::ldx },
    Opcode { 0xB7, 4, "LAX", AM::ZERO_PAGE_Y, unsupported },
    Opcode { 0xB8, 2, "CLV", AM::IMPLICIT, Op::clv },
    Opcode { 0xB9, 4, "LDA", AM::ABSOLUTE_Y, Op::lda },
    Opcode { 0xBA, 2, "TSX", AM::IMPLICIT, Op::tsx },
    Opcode { 0xBB, 4, "LAS", AM::ABSOLUTE_Y, unsupported },
    Opcode { 0xBC, 4, "LDY", AM::ABSOLUTE_X, Op::ldy },
    Opcode { 0xBD, 4, "LDA", AM::ABSOLUTE_X, Op::lda },
    Opcode { 0xBE, 4, "LDX", AM::ABSOLUTE_Y, Op::ldx },
    Opcode { 0xBF, 4, "LAX", AM::ABSOLUTE_Y, unsupported },

    /*
     * 0xC0 - 0xCF
     */
    Opcode { 0xC0, 2, "CPY", AM::IMMEDIATE, Op::cpy },
    Opcode { 0xC1, 6, "CMP", AM::INDEXED_INDIRECT, Op::cmp },
    Opcode { 0xC2, 2, "NOP", AM::IMMEDIATE, unsupported },
    Opcode { 0xC3, 8, "DCP", AM::INDEXED_INDIRECT, unsupported },
    Opcode { 0xC4, 3, "CPY", AM::ZERO_PAGE, Op::cpy },
    Opcode { 0xC5, 3, "CMP", AM::ZERO_PAGE, Op::cmp },
    Opcode { 0xC6, 5, "DEC", AM::ZERO_PAGE, Op::dec },
    Opcode { 0xC7, 5, "DCP", AM::ZERO_PAGE, unsupported },
    Opcode { 0xC8, 2, "INY", AM::IMPLICIT, Op::iny },
    Opcode { 0xC9, 2, "CMP", AM::IMMEDIATE, Op::cmp },
    Opcode { 0xCA, 2, "DEX", AM::IMPLICIT, Op::dex },
    Opcode { 0xCB, 2, "AXS", AM::IMMEDIATE, unsupported },
    Opcode { 0xCC, 4, "CPY", AM::ABSOLUTE, Op::cpy },
    Opcode { 0xCD, 4, "CMP", AM::ABSOLUTE, Op::cmp },
    Opcode { 0xCE, 6, "DEC", AM::ABSOLUTE, Op::dec },
    Opcode { 0xCF, 6, "DCP", AM::ABSOLUTE, unsupported },

    /*
     * 0xD0 - 0xDF
     */
    Opcode { 0xD0, 2, "BNE", AM::RELATIVE, Op::bne },
    Opcode { 0xD1, 5, "CMP", AM::INDIRECT_INDEXED, Op::cmp },
    Opcode { 0xD2, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0xD3, 8, "DCP", AM::INDIRECT_INDEXED, unsupported },
    Opcode { 0xD4, 4, "NOP", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0xD5, 4, "CMP", AM::ZERO_PAGE_X, Op::cmp },
    Opcode { 0xD6, 6, "DEC", AM::ZERO_PAGE_X, Op::dec },
    Opcode { 0xD7, 6, "DCP", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0xD8, 2, "CLD", AM::IMPLICIT, Op::cld },
    Opcode { 0xD9, 4, "CMP", AM::ABSOLUTE_Y, Op::cmp },
    Opcode { 0xDA, 2, "NOP", AM::IMPLICIT, unsupported },
    Opcode { 0xDB, 7, "DCP", AM::ABSOLUTE_Y, unsupported },
    Opcode { 0xDC, 4, "NOP", AM::ABSOLUTE_X, unsupported },
    Opcode { 0xDD, 4, "CMP", AM::ABSOLUTE_X, Op::cmp },
    Opcode { 0xDE, 7, "DEC", AM::ABSOLUTE_X, Op::dec },
    Opcode { 0xDF, 7, "DCP", AM::ABSOLUTE_X, unsupported },

    /*
     * 0xE0 - 0xEF
     */
    Opcode { 0xE0, 2, "CPX", AM::IMMEDIATE, Op::cpx },
    Opcode { 0xE1, 6, "SBC", AM::INDEXED_INDIRECT, Op::sbc },
    Opcode { 0xE2, 2, "NOP", AM::IMMEDIATE, unsupported },
    Opcode { 0xE3, 8, "ISC", AM::INDEXED_INDIRECT, unsupported },
    Opcode { 0xE4, 3, "CPX", AM::ZERO_PAGE, Op::cpx },
    Opcode { 0xE5, 3, "SBC", AM::ZERO_PAGE, Op::sbc },
    Opcode { 0xE6, 5, "INC", AM::ZERO_PAGE, Op::inc },
    Opcode { 0xE7, 5, "ISC", AM::ZERO_PAGE, unsupported },
    Opcode { 0xE8, 2, "INX", AM::IMPLICIT, Op::inx },
    Opcode { 0xE9, 2, "SBC", AM::IMMEDIATE, Op::sbc },
    Opcode { 0xEA, 2, "NOP", AM::IMPLICIT, nop },
    Opcode { 0xEB, 2, "SBC", AM::IMMEDIATE, unsupported },
    Opcode { 0xEC, 4, "CPX", AM::ABSOLUTE, Op::cpx },
    Opcode { 0xED, 4, "SBC", AM::ABSOLUTE, Op::sbc },
    Opcode { 0xEE, 6, "INC", AM::ABSOLUTE, Op::inc },
    Opcode { 0xEF, 6, "ISC", AM::ABSOLUTE, unsupported },

    /*
     * 0xF0 - 0xFF
     */
    Opcode { 0xF0, 2, "BEQ", AM::RELATIVE, Op::beq },
    Opcode { 0xF1, 5, "SBC", AM::INDIRECT_INDEXED, Op::sbc },
    Opcode { 0xF2, 0, "KIL", AM::IMPLICIT, unsupported },
    Opcode { 0xF3, 8, "ISC", AM::INDIRECT_INDEXED, unsupported },
    Opcode { 0xF4, 4, "NOP", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0xF5, 4, "SBC", AM::ZERO_PAGE_X, Op::sbc },
    Opcode { 0xF6, 6, "INC", AM::ZERO_PAGE_X, Op::inc },
    Opcode { 0xF7, 6, "ISC", AM::ZERO_PAGE_X, unsupported },
    Opcode { 0xF8, 2, "SED", AM::IMPLICIT, Op::sed },
    Opcode { 0xF9, 4, "SBC", AM::ABSOLUTE_Y, Op::sbc },
    Opcode { 0xFA, 2, "NOP", AM::IMPLICIT, unsupported },
    Opcode { 0xFB, 7, "ISC", AM::ABSOLUTE_X, unsupported },
    Opcode { 0xFC, 4, "NOP", AM::ABSOLUTE_X, unsupported },
    Opcode { 0xFD, 4, "SBC", AM::ABSOLUTE_X, Op::sbc },
    Opcode { 0xFE, 7, "INC", AM::ABSOLUTE_X, Op::inc },
    Opcode { 0xFF, 7, "ISC", AM::ABSOLUTE_X, unsupported }
};

const Opcode *Op::decode(uint8_t op) {
    return op < sizeof(OPCODES) / sizeof(Opcode) ? &OPCODES[op] : nullptr;
}

size_t Op::getAddressingModeOperandCount(Op::AddressingMode mode) {
    switch (mode) {
        case AM::IMPLICIT:
        case AM::ACCUMULATOR:
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
    const RegisterFile *r = cpu->getRegs();
    const Memory *mem = cpu->getNES()->getMemory();

    switch (mode) {
        case AM::IMPLICIT:
            return 0;

        case AM::ACCUMULATOR:
            return r->a;

        case AM::IMMEDIATE:
            return operands[0];

        default:
            return mem->read(getAddress(cpu, mode, operands));
    }
}

void Op::addressWrite(CPU *cpu, Op::AddressingMode mode, const Op::Operands &operands, uint8_t value) {
    RegisterFile *r = cpu->getRegs();
    Memory *mem = cpu->getNES()->getMemory();

    switch (mode) {
        case AM::IMPLICIT:
        case AM::IMMEDIATE:
            return;

        case AM::ACCUMULATOR:
            r->a = value;
            break;

        default:
            mem->write(getAddress(cpu, mode, operands), value);
            break;
    }
}


Address Op::getAddress(CPU *cpu, Op::AddressingMode mode, const Op::Operands &operands) {
    const RegisterFile *r = cpu->getRegs();
    const Memory *mem = cpu->getNES()->getMemory();

    switch (mode) {
        case AM::IMPLICIT:
        case AM::ACCUMULATOR:
        case AM::IMMEDIATE:
            return 0;

        case AM::ZERO_PAGE:
            return (Address)operands[0];

        case AM::ZERO_PAGE_X:
            return (Address)(operands[0] + r->x);

        case AM::ZERO_PAGE_Y:
            return (Address)(operands[0] + r->y);

        case AM::ABSOLUTE:
            return Utils::combineUint8sLE(operands[0], operands[1]);

        case AM::ABSOLUTE_X:
            return Utils::combineUint8sLE(operands[0], operands[1]) + r->x;

        case AM::ABSOLUTE_Y:
            return Utils::combineUint8sLE(operands[0], operands[1]) + r->y;

        case AM::RELATIVE:
            return r->pc + (int8_t)operands[0];

        case AM::INDIRECT: {
            Address addrAddr = Utils::combineUint8sLE(operands[0], operands[1]);
            return Utils::combineUint8sLE(mem->read(addrAddr), mem->read(addrAddr + (Address)1));
        }

        case AM::INDEXED_INDIRECT:
            return mem->read((Address)(operands[0] + r->x));

        case AM::INDIRECT_INDEXED: {
            Address addrAddr = mem->read(operands[0]) + r->y;
            return Utils::combineUint8sLE(mem->read(addrAddr), mem->read(addrAddr + (Address)1));
        }
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

void Op::formatInstruction(const Opcode *opcode, const Op::Operands &operands, std::string *outFormatted) {
    if (!outFormatted) {
        return;
    }

    std::ostringstream stream;
    stream << opcode->name;

    switch (opcode->mode) {
        case AM::IMPLICIT:
            break;

        case AM::ACCUMULATOR:
            stream << " A";
            break;

        case AM::IMMEDIATE:
            stream << " #$";
            Utils::writeHexToStream(stream, operands[0]);
            break;

        case AM::ZERO_PAGE:
            stream << " $";
            Utils::writeHexToStream(stream, operands[0]);
            break;

        case AM::ZERO_PAGE_X:
            stream << " $";
            Utils::writeHexToStream(stream, operands[0]);
            stream << ", X";
            break;

        case AM::ZERO_PAGE_Y:
            stream << " $";
            Utils::writeHexToStream(stream, operands[0]);
            stream << ", Y";
            break;

        case AM::INDEXED_INDIRECT:
            stream << " ($";
            Utils::writeHexToStream(stream, operands[0]);
            stream << ", X)";
            break;

        case AM::INDIRECT_INDEXED:
            stream << " ($";
            Utils::writeHexToStream(stream, operands[0]);
            stream << "), Y";
            break;

        case AM::ABSOLUTE:
            stream << " $";
            Utils::writeHexToStream(stream, Utils::combineUint8sLE(operands[0], operands[1]));
            break;

        case AM::ABSOLUTE_X:
            stream << " $";
            Utils::writeHexToStream(stream, Utils::combineUint8sLE(operands[0], operands[1]));
            stream << ", X";
            break;

        case AM::ABSOLUTE_Y:
            stream << " $";
            Utils::writeHexToStream(stream, Utils::combineUint8sLE(operands[0], operands[1]));
            stream << ", Y";
            break;

        case AM::INDIRECT:
            stream << " ($";
            Utils::writeHexToStream(stream, Utils::combineUint8sLE(operands[0], operands[1]));
            stream << ")";
            break;

        case AM::RELATIVE:
            stream << " *" << (int8_t)operands[0];
            break;
    }

    *outFormatted = stream.str();
}