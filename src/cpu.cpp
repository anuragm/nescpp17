#include "cpu.hpp"

cpu::cpu() {
  //Set the CPU core memory and registers to zero.
  cpu_core.mem.zeros();
  cpu_core.P.byte = 0;
  cpu_core.A      = 0;
  cpu_core.X      = 0;
  cpu_core.Y      = 0;
  cpu_core.SP     = 0;
  cpu_core.PC     = 0;

  //Set the member CPU core of opcodes to our CPU core.
  code.core = &cpu_core;

  //Assign the code table.
  opcode_table[0x00] = &opcodes::BRK         ; opcode_table[0x10] = &opcodes::BPL       ;
  opcode_table[0x01] = &opcodes::ORA<m_INX>  ; opcode_table[0x11] = &opcodes::ORA<m_INY>;
  opcode_table[0x05] = &opcodes::ORA<m_ZPG>  ; opcode_table[0x15] = &opcodes::ORA<m_ZPX>;
  opcode_table[0x06] = &opcodes::ASL<m_ZPG>  ; opcode_table[0x16] = &opcodes::ASL<m_ZPX>;
  opcode_table[0x08] = &opcodes::PHP         ; opcode_table[0x18] = &opcodes::CLC       ;
  opcode_table[0x09] = &opcodes::ORA<m_IMM>  ; opcode_table[0x19] = &opcodes::ORA<m_ABY>;
  opcode_table[0x0A] = &opcodes::ASL<m_ACCUM>; opcode_table[0x1D] = &opcodes::ORA<m_ABX>;
  opcode_table[0x0D] = &opcodes::ORA<m_ABS>  ; opcode_table[0x1E] = &opcodes::ASL<m_ABX>;
  opcode_table[0x0E] = &opcodes::ASL<m_ABS>  ;

  opcode_table[0x20] = &opcodes::JSR         ; opcode_table[0x30] = &opcodes::BMI       ;
  opcode_table[0x21] = &opcodes::AND<m_INX>  ; opcode_table[0x31] = &opcodes::AND<m_INY>;
  opcode_table[0x24] = &opcodes::BIT<m_ZPG>  ; opcode_table[0x35] = &opcodes::AND<m_ZPG>;
  opcode_table[0x25] = &opcodes::AND<m_ZPG>  ; opcode_table[0x36] = &opcodes::ROL<m_ZPG>;
  opcode_table[0x26] = &opcodes::ROL<m_ZPG>  ; opcode_table[0x38] = &opcodes::SEC       ;
  opcode_table[0x28] = &opcodes::PLP         ; opcode_table[0x39] = &opcodes::AND<m_ABY>;
  opcode_table[0x29] = &opcodes::AND<m_IMM>  ; opcode_table[0x3D] = &opcodes::AND<m_ABX>;
  opcode_table[0x2A] = &opcodes::ROL<m_ACCUM>; opcode_table[0x3E] = &opcodes::ROL<m_ABX>;
  opcode_table[0x2C] = &opcodes::BIT<m_ABS>  ;
  opcode_table[0x2D] = &opcodes::AND<m_ABS>  ;
  opcode_table[0x2E] = &opcodes::ROL<m_ABS>  ;

  opcode_table[0x40] = &opcodes::RTI         ; opcode_table[0x50] = &opcodes::BVC        ;
  opcode_table[0x41] = &opcodes::EOR<m_INX>  ; opcode_table[0x51] = &opcodes::EOR<m_INY> ;
  opcode_table[0x45] = &opcodes::EOR<m_ZPG>  ; opcode_table[0x55] = &opcodes::EOR<m_ZPG> ;
  opcode_table[0x46] = &opcodes::LSR<m_ZPG>  ; opcode_table[0x56] = &opcodes::LSR<m_ZPG> ;
  opcode_table[0x48] = &opcodes::PHA         ; opcode_table[0x58] = &opcodes::CLI        ;
  opcode_table[0x49] = &opcodes::EOR<m_IMM>  ; opcode_table[0x59] = &opcodes::EOR<m_ABY> ;
  opcode_table[0x4A] = &opcodes::LSR<m_ACCUM>; opcode_table[0x5D] = &opcodes::EOR<m_ABX> ;
  opcode_table[0x4C] = &opcodes::JMP_ABS     ; opcode_table[0x5E] = &opcodes::LSR<m_ABX> ;
  opcode_table[0x4D] = &opcodes::EOR<m_ABS>  ;
  opcode_table[0x4E] = &opcodes::LSR<m_ABS>  ;

  opcode_table[0x60] = &opcodes::RTS         ; opcode_table[0x70] = &opcodes::BVS        ;
  opcode_table[0x61] = &opcodes::ADC<m_INX>  ; opcode_table[0x71] = &opcodes::ADC<m_INX> ;
  opcode_table[0x65] = &opcodes::ADC<m_ZPG>  ; opcode_table[0x75] = &opcodes::ADC<m_ZPX> ;
  opcode_table[0x66] = &opcodes::ROR<m_ZPG>  ; opcode_table[0x76] = &opcodes::ROR<m_ZPX> ;
  opcode_table[0x68] = &opcodes::PLA         ; opcode_table[0x78] = &opcodes::SEI        ;
  opcode_table[0x69] = &opcodes::ADC<m_IMM>  ; opcode_table[0x79] = &opcodes::ADC<m_ABY> ;
  opcode_table[0x6A] = &opcodes::ROR<m_ACCUM>; opcode_table[0x7D] = &opcodes::ADC<m_ABX> ;
  opcode_table[0x6C] = &opcodes::JMP_IND     ; opcode_table[0x7E] = &opcodes::ROR<m_ABX> ;
  opcode_table[0x6D] = &opcodes::ADC<m_ABS>  ;
  opcode_table[0x6E] = &opcodes::ROR<m_ABS>  ;

  opcode_table[0x81] = &opcodes::STA<m_INX>  ; opcode_table[0x90] = &opcodes::BCC        ;
  opcode_table[0x84] = &opcodes::STY<m_ZPG>  ; opcode_table[0x91] = &opcodes::STA<m_INY> ;
  opcode_table[0x85] = &opcodes::STA<m_ZPG>  ; opcode_table[0x94] = &opcodes::STY<m_ZPG> ;
  opcode_table[0x86] = &opcodes::STX<m_ZPG>  ; opcode_table[0x95] = &opcodes::STA<m_ZPG> ;
  opcode_table[0x88] = &opcodes::DEY         ; opcode_table[0x96] = &opcodes::STX<m_ZPY> ;
  opcode_table[0x8A] = &opcodes::TXA         ; opcode_table[0x98] = &opcodes::TYA        ;
  opcode_table[0x8C] = &opcodes::STY<m_ABS>  ; opcode_table[0x99] = &opcodes::STA<m_ABY> ;
  opcode_table[0x8D] = &opcodes::STA<m_ABS>  ; opcode_table[0x9A] = &opcodes::TXS        ;
  opcode_table[0x8E] = &opcodes::STX<m_ABS>  ; opcode_table[0x9D] = &opcodes::STA<m_ABX> ;

  opcode_table[0xA0] = &opcodes::LDY<m_IMM>  ; opcode_table[0xB0] = &opcodes::BCS        ;
  opcode_table[0xA1] = &opcodes::LDA<m_INX>  ; opcode_table[0xB1] = &opcodes::LDA<m_INY> ;
  opcode_table[0xA2] = &opcodes::LDX<m_IMM>  ; opcode_table[0xB4] = &opcodes::LDY<m_ZPG> ;
  opcode_table[0xA4] = &opcodes::LDY<m_ZPG>  ; opcode_table[0xB5] = &opcodes::LDA<m_ZPG> ;
  opcode_table[0xA5] = &opcodes::LDA<m_ZPG>  ; opcode_table[0xB6] = &opcodes::LDX<m_ZPY> ;
  opcode_table[0xA6] = &opcodes::LDX<m_ZPG>  ; opcode_table[0xB8] = &opcodes::CLV        ;
  opcode_table[0xA8] = &opcodes::TAY         ; opcode_table[0xB9] = &opcodes::LDA<m_ABY> ;
  opcode_table[0xA9] = &opcodes::LDA<m_IMM>  ; opcode_table[0xBA] = &opcodes::TSX        ;
  opcode_table[0xAA] = &opcodes::TAX         ; opcode_table[0xBC] = &opcodes::LDY<m_ABX> ;
  opcode_table[0xAC] = &opcodes::LDY<m_ABS>  ; opcode_table[0xBD] = &opcodes::LDA<m_ABX> ;
  opcode_table[0xAD] = &opcodes::LDA<m_ABS>  ; opcode_table[0xBE] = &opcodes::LDX<m_ABY> ;
  opcode_table[0xAE] = &opcodes::LDX<m_ABS>  ;

  opcode_table[0xC0] = &opcodes::CPY<m_IMM>  ; opcode_table[0xD0] = &opcodes::BNE        ;
  opcode_table[0xC1] = &opcodes::CMP<m_INX>  ; opcode_table[0xD1] = &opcodes::CMP<m_INY> ;
  opcode_table[0xC4] = &opcodes::CPY<m_ZPG>  ; opcode_table[0xD5] = &opcodes::CMP<m_ZPG> ;
  opcode_table[0xC5] = &opcodes::CMP<m_ZPG>  ; opcode_table[0xD6] = &opcodes::DEC<m_ZPG> ;
  opcode_table[0xC6] = &opcodes::DEC<m_ZPG>  ; opcode_table[0xD8] = &opcodes::CLD        ;
  opcode_table[0xC8] = &opcodes::INY         ; opcode_table[0xD9] = &opcodes::CMP<m_ABY> ;
  opcode_table[0xC9] = &opcodes::CMP<m_IMM>  ; opcode_table[0xDD] = &opcodes::CMP<m_ABX> ;
  opcode_table[0xCA] = &opcodes::DEX         ; opcode_table[0xDE] = &opcodes::DEC<m_ABX> ;
  opcode_table[0xCC] = &opcodes::CPY<m_ABS>  ;
  opcode_table[0xCD] = &opcodes::CMP<m_ABS>  ;
  opcode_table[0xCE] = &opcodes::DEC<m_ABS>  ;

  opcode_table[0xE0] = &opcodes::CPX<m_IMM>  ; opcode_table[0xF0] = &opcodes::BEQ        ;
  opcode_table[0xE1] = &opcodes::SBC<m_INX>  ; opcode_table[0xF1] = &opcodes::SBC<m_INY> ;
  opcode_table[0xE4] = &opcodes::CPX<m_ZPG>  ; opcode_table[0xF5] = &opcodes::SBC<m_ZPG> ;
  opcode_table[0xE5] = &opcodes::SBC<m_ZPG>  ; opcode_table[0xF6] = &opcodes::INC<m_ZPG> ;
  opcode_table[0xE6] = &opcodes::INC<m_ZPG>  ; opcode_table[0xF8] = &opcodes::SED        ;
  opcode_table[0xE8] = &opcodes::INX         ; opcode_table[0xF9] = &opcodes::SBC<m_ABY> ;
  opcode_table[0xE9] = &opcodes::SBC<m_IMM>  ; opcode_table[0xFD] = &opcodes::SBC<m_ABX> ;
  opcode_table[0xEA] = &opcodes::NOP         ; opcode_table[0xFE] = &opcodes::INC<m_ABX> ;
  opcode_table[0xEC] = &opcodes::CPX<m_ABS>  ;
  opcode_table[0xED] = &opcodes::SBC<m_ABS>  ;
  opcode_table[0xEE] = &opcodes::INC<m_ABS>  ;
}
