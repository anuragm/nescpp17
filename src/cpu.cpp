#include "cpu.hpp"

// Implement the constructor which defines the opcode table.
cpu::cpu(){
  //Set the initial variables to be zero.
  cycle_count = 0;
  mem.zeros();
  P.byte = 0;
  A      = 0;
  X      = 0;
  Y      = 0;
  SP     = 0;
  PC     = 0;

  // Initialize the opcode table.
  opcode_table[0x00] = &cpu::BRK         ; opcode_table[0x10] = &cpu::BPL       ;
  opcode_table[0x01] = &cpu::ORA<m_INX>  ; opcode_table[0x11] = &cpu::ORA<m_INY>;
  opcode_table[0x05] = &cpu::ORA<m_ZPG>  ; opcode_table[0x15] = &cpu::ORA<m_ZPX>;
  opcode_table[0x06] = &cpu::ASL<m_ZPG>  ; opcode_table[0x16] = &cpu::ASL<m_ZPX>;
  opcode_table[0x08] = &cpu::PHP         ; opcode_table[0x18] = &cpu::CLC       ;
  opcode_table[0x09] = &cpu::ORA<m_IMM>  ; opcode_table[0x19] = &cpu::ORA<m_ABY>;
  opcode_table[0x0A] = &cpu::ASL<m_ACCUM>; opcode_table[0x1D] = &cpu::ORA<m_ABX>;
  opcode_table[0x0D] = &cpu::ORA<m_ABS>  ; opcode_table[0x1E] = &cpu::ASL<m_ABX>;
  opcode_table[0x0E] = &cpu::ASL<m_ABS>  ;

  opcode_table[0x20] = &cpu::JSR         ; opcode_table[0x30] = &cpu::BMI       ;
  opcode_table[0x21] = &cpu::AND<m_INX>  ; opcode_table[0x31] = &cpu::AND<m_INY>;
  opcode_table[0x24] = &cpu::BIT<m_ZPG>  ; opcode_table[0x35] = &cpu::AND<m_ZPG>;
  opcode_table[0x25] = &cpu::AND<m_ZPG>  ; opcode_table[0x36] = &cpu::ROL<m_ZPG>;
  opcode_table[0x26] = &cpu::ROL<m_ZPG>  ; opcode_table[0x38] = &cpu::SEC       ;
  opcode_table[0x28] = &cpu::PLP         ; opcode_table[0x39] = &cpu::AND<m_ABY>;
  opcode_table[0x29] = &cpu::AND<m_IMM>  ; opcode_table[0x3D] = &cpu::AND<m_ABX>;
  opcode_table[0x2A] = &cpu::ROL<m_ACCUM>; opcode_table[0x3E] = &cpu::ROL<m_ABX>;
  opcode_table[0x2C] = &cpu::BIT<m_ABS>  ;
  opcode_table[0x2D] = &cpu::AND<m_ABS>  ;
  opcode_table[0x2E] = &cpu::ROL<m_ABS>  ;

  opcode_table[0x40] = &cpu::RTI         ; opcode_table[0x50] = &cpu::BVC        ;
  opcode_table[0x41] = &cpu::EOR<m_INX>  ; opcode_table[0x51] = &cpu::EOR<m_INY> ;
  opcode_table[0x45] = &cpu::EOR<m_ZPG>  ; opcode_table[0x55] = &cpu::EOR<m_ZPG> ;
  opcode_table[0x46] = &cpu::LSR<m_ZPG>  ; opcode_table[0x56] = &cpu::LSR<m_ZPG> ;
  opcode_table[0x48] = &cpu::PHA         ; opcode_table[0x58] = &cpu::CLI        ;
  opcode_table[0x49] = &cpu::EOR<m_IMM>  ; opcode_table[0x59] = &cpu::EOR<m_ABY> ;
  opcode_table[0x4A] = &cpu::LSR<m_ACCUM>; opcode_table[0x5D] = &cpu::EOR<m_ABX> ;
  opcode_table[0x4C] = &cpu::JMP_ABS     ; opcode_table[0x5E] = &cpu::LSR<m_ABX> ;
  opcode_table[0x4D] = &cpu::EOR<m_ABS>  ;
  opcode_table[0x4E] = &cpu::LSR<m_ABS>  ;

  opcode_table[0x60] = &cpu::RTS         ; opcode_table[0x70] = &cpu::BVS        ;
  opcode_table[0x61] = &cpu::ADC<m_INX>  ; opcode_table[0x71] = &cpu::ADC<m_INX> ;
  opcode_table[0x65] = &cpu::ADC<m_ZPG>  ; opcode_table[0x75] = &cpu::ADC<m_ZPX> ;
  opcode_table[0x66] = &cpu::ROR<m_ZPG>  ; opcode_table[0x76] = &cpu::ROR<m_ZPX> ;
  opcode_table[0x68] = &cpu::PLA         ; opcode_table[0x78] = &cpu::SEI        ;
  opcode_table[0x69] = &cpu::ADC<m_IMM>  ; opcode_table[0x79] = &cpu::ADC<m_ABY> ;
  opcode_table[0x6A] = &cpu::ROR<m_ACCUM>; opcode_table[0x7D] = &cpu::ADC<m_ABX> ;
  opcode_table[0x6C] = &cpu::JMP_IND     ; opcode_table[0x7E] = &cpu::ROR<m_ABX> ;
  opcode_table[0x6D] = &cpu::ADC<m_ABS>  ;
  opcode_table[0x6E] = &cpu::ROR<m_ABS>  ;

  opcode_table[0x81] = &cpu::STA<m_INX>  ; opcode_table[0x90] = &cpu::BCC        ;
  opcode_table[0x84] = &cpu::STY<m_ZPG>  ; opcode_table[0x91] = &cpu::STA<m_INY> ;
  opcode_table[0x85] = &cpu::STA<m_ZPG>  ; opcode_table[0x94] = &cpu::STY<m_ZPG> ;
  opcode_table[0x86] = &cpu::STX<m_ZPG>  ; opcode_table[0x95] = &cpu::STA<m_ZPG> ;
  opcode_table[0x88] = &cpu::DEY         ; opcode_table[0x96] = &cpu::STX<m_ZPY> ;
  opcode_table[0x8A] = &cpu::TXA         ; opcode_table[0x98] = &cpu::TYA        ;
  opcode_table[0x8C] = &cpu::STY<m_ABS>  ; opcode_table[0x99] = &cpu::STA<m_ABY> ;
  opcode_table[0x8D] = &cpu::STA<m_ABS>  ; opcode_table[0x9A] = &cpu::TXS        ;
  opcode_table[0x8E] = &cpu::STX<m_ABS>  ; opcode_table[0x9D] = &cpu::STA<m_ABX> ;

  opcode_table[0xA0] = &cpu::LDY<m_IMM>  ; opcode_table[0xB0] = &cpu::BCS        ;
  opcode_table[0xA1] = &cpu::LDA<m_INX>  ; opcode_table[0xB1] = &cpu::LDA<m_INY> ;
  opcode_table[0xA2] = &cpu::LDX<m_IMM>  ; opcode_table[0xB4] = &cpu::LDY<m_ZPG> ;
  opcode_table[0xA4] = &cpu::LDY<m_ZPG>  ; opcode_table[0xB5] = &cpu::LDA<m_ZPG> ;
  opcode_table[0xA5] = &cpu::LDA<m_ZPG>  ; opcode_table[0xB6] = &cpu::LDX<m_ZPY> ;
  opcode_table[0xA6] = &cpu::LDX<m_ZPG>  ; opcode_table[0xB8] = &cpu::CLV        ;
  opcode_table[0xA8] = &cpu::TAY         ; opcode_table[0xB9] = &cpu::LDA<m_ABY> ;
  opcode_table[0xA9] = &cpu::LDA<m_IMM>  ; opcode_table[0xBA] = &cpu::TSX        ;
  opcode_table[0xAA] = &cpu::TAX         ; opcode_table[0xBC] = &cpu::LDY<m_ABX> ;
  opcode_table[0xAC] = &cpu::LDY<m_ABS>  ; opcode_table[0xBD] = &cpu::LDA<m_ABX> ;
  opcode_table[0xAD] = &cpu::LDA<m_ABS>  ; opcode_table[0xBE] = &cpu::LDX<m_ABY> ;
  opcode_table[0xAE] = &cpu::LDX<m_ABS>  ;

  opcode_table[0xC0] = &cpu::CPY<m_IMM>  ; opcode_table[0xD0] = &cpu::BNE        ;
  opcode_table[0xC1] = &cpu::CMP<m_INX>  ; opcode_table[0xD1] = &cpu::CMP<m_INY> ;
  opcode_table[0xC4] = &cpu::CPY<m_ZPG>  ; opcode_table[0xD5] = &cpu::CMP<m_ZPG> ;
  opcode_table[0xC5] = &cpu::CMP<m_ZPG>  ; opcode_table[0xD6] = &cpu::DEC<m_ZPG> ;
  opcode_table[0xC6] = &cpu::DEC<m_ZPG>  ; opcode_table[0xD8] = &cpu::CLD        ;
  opcode_table[0xC8] = &cpu::INY         ; opcode_table[0xD9] = &cpu::CMP<m_ABY> ;
  opcode_table[0xC9] = &cpu::CMP<m_IMM>  ; opcode_table[0xDD] = &cpu::CMP<m_ABX> ;
  opcode_table[0xCA] = &cpu::DEX         ; opcode_table[0xDE] = &cpu::DEC<m_ABX> ;
  opcode_table[0xCC] = &cpu::CPY<m_ABS>  ;
  opcode_table[0xCD] = &cpu::CMP<m_ABS>  ;
  opcode_table[0xCE] = &cpu::DEC<m_ABS>  ;

  opcode_table[0xE0] = &cpu::CPX<m_IMM>  ; opcode_table[0xF0] = &cpu::BEQ        ;
  opcode_table[0xE1] = &cpu::SBC<m_INX>  ; opcode_table[0xF1] = &cpu::SBC<m_INY> ;
  opcode_table[0xE4] = &cpu::CPX<m_ZPG>  ; opcode_table[0xF5] = &cpu::SBC<m_ZPG> ;
  opcode_table[0xE5] = &cpu::SBC<m_ZPG>  ; opcode_table[0xF6] = &cpu::INC<m_ZPG> ;
  opcode_table[0xE6] = &cpu::INC<m_ZPG>  ; opcode_table[0xF8] = &cpu::SED        ;
  opcode_table[0xE8] = &cpu::INX         ; opcode_table[0xF9] = &cpu::SBC<m_ABY> ;
  opcode_table[0xE9] = &cpu::SBC<m_IMM>  ; opcode_table[0xFD] = &cpu::SBC<m_ABX> ;
  opcode_table[0xEA] = &cpu::NOP         ; opcode_table[0xFE] = &cpu::INC<m_ABX> ;
  opcode_table[0xEC] = &cpu::CPX<m_ABS>  ;
  opcode_table[0xED] = &cpu::SBC<m_ABS>  ;
  opcode_table[0xEE] = &cpu::INC<m_ABS>  ;
}
