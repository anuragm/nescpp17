#include "cpu_main.hpp"

// Implement the constructor which defines the opcode table.
cpu_main::cpu_main(){
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
  opcode_table[0x00] = &cpu_main::BRK         ; opcode_table[0x10] = &cpu_main::BPL       ;
  opcode_table[0x01] = &cpu_main::ORA<m_INX>  ; opcode_table[0x11] = &cpu_main::ORA<m_INY>;
  opcode_table[0x05] = &cpu_main::ORA<m_ZPG>  ; opcode_table[0x15] = &cpu_main::ORA<m_ZPX>;
  opcode_table[0x06] = &cpu_main::ASL<m_ZPG>  ; opcode_table[0x16] = &cpu_main::ASL<m_ZPX>;
  opcode_table[0x08] = &cpu_main::PHP         ; opcode_table[0x18] = &cpu_main::CLC       ;
  opcode_table[0x09] = &cpu_main::ORA<m_IMM>  ; opcode_table[0x19] = &cpu_main::ORA<m_ABY>;
  opcode_table[0x0A] = &cpu_main::ASL<m_ACCUM>; opcode_table[0x1D] = &cpu_main::ORA<m_ABX>;
  opcode_table[0x0D] = &cpu_main::ORA<m_ABS>  ; opcode_table[0x1E] = &cpu_main::ASL<m_ABX>;
  opcode_table[0x0E] = &cpu_main::ASL<m_ABS>  ;

  opcode_table[0x20] = &cpu_main::JSR         ; opcode_table[0x30] = &cpu_main::BMI       ;
  opcode_table[0x21] = &cpu_main::AND<m_INX>  ; opcode_table[0x31] = &cpu_main::AND<m_INY>;
  opcode_table[0x24] = &cpu_main::BIT<m_ZPG>  ; opcode_table[0x35] = &cpu_main::AND<m_ZPG>;
  opcode_table[0x25] = &cpu_main::AND<m_ZPG>  ; opcode_table[0x36] = &cpu_main::ROL<m_ZPG>;
  opcode_table[0x26] = &cpu_main::ROL<m_ZPG>  ; opcode_table[0x38] = &cpu_main::SEC       ;
  opcode_table[0x28] = &cpu_main::PLP         ; opcode_table[0x39] = &cpu_main::AND<m_ABY>;
  opcode_table[0x29] = &cpu_main::AND<m_IMM>  ; opcode_table[0x3D] = &cpu_main::AND<m_ABX>;
  opcode_table[0x2A] = &cpu_main::ROL<m_ACCUM>; opcode_table[0x3E] = &cpu_main::ROL<m_ABX>;
  opcode_table[0x2C] = &cpu_main::BIT<m_ABS>  ;
  opcode_table[0x2D] = &cpu_main::AND<m_ABS>  ;
  opcode_table[0x2E] = &cpu_main::ROL<m_ABS>  ;

  opcode_table[0x40] = &cpu_main::RTI         ; opcode_table[0x50] = &cpu_main::BVC        ;
  opcode_table[0x41] = &cpu_main::EOR<m_INX>  ; opcode_table[0x51] = &cpu_main::EOR<m_INY> ;
  opcode_table[0x45] = &cpu_main::EOR<m_ZPG>  ; opcode_table[0x55] = &cpu_main::EOR<m_ZPG> ;
  opcode_table[0x46] = &cpu_main::LSR<m_ZPG>  ; opcode_table[0x56] = &cpu_main::LSR<m_ZPG> ;
  opcode_table[0x48] = &cpu_main::PHA         ; opcode_table[0x58] = &cpu_main::CLI        ;
  opcode_table[0x49] = &cpu_main::EOR<m_IMM>  ; opcode_table[0x59] = &cpu_main::EOR<m_ABY> ;
  opcode_table[0x4A] = &cpu_main::LSR<m_ACCUM>; opcode_table[0x5D] = &cpu_main::EOR<m_ABX> ;
  opcode_table[0x4C] = &cpu_main::JMP_ABS     ; opcode_table[0x5E] = &cpu_main::LSR<m_ABX> ;
  opcode_table[0x4D] = &cpu_main::EOR<m_ABS>  ;
  opcode_table[0x4E] = &cpu_main::LSR<m_ABS>  ;

  opcode_table[0x60] = &cpu_main::RTS         ; opcode_table[0x70] = &cpu_main::BVS        ;
  opcode_table[0x61] = &cpu_main::ADC<m_INX>  ; opcode_table[0x71] = &cpu_main::ADC<m_INX> ;
  opcode_table[0x65] = &cpu_main::ADC<m_ZPG>  ; opcode_table[0x75] = &cpu_main::ADC<m_ZPX> ;
  opcode_table[0x66] = &cpu_main::ROR<m_ZPG>  ; opcode_table[0x76] = &cpu_main::ROR<m_ZPX> ;
  opcode_table[0x68] = &cpu_main::PLA         ; opcode_table[0x78] = &cpu_main::SEI        ;
  opcode_table[0x69] = &cpu_main::ADC<m_IMM>  ; opcode_table[0x79] = &cpu_main::ADC<m_ABY> ;
  opcode_table[0x6A] = &cpu_main::ROR<m_ACCUM>; opcode_table[0x7D] = &cpu_main::ADC<m_ABX> ;
  opcode_table[0x6C] = &cpu_main::JMP_IND     ; opcode_table[0x7E] = &cpu_main::ROR<m_ABX> ;
  opcode_table[0x6D] = &cpu_main::ADC<m_ABS>  ;
  opcode_table[0x6E] = &cpu_main::ROR<m_ABS>  ;

  opcode_table[0x81] = &cpu_main::STA<m_INX>  ; opcode_table[0x90] = &cpu_main::BCC        ;
  opcode_table[0x84] = &cpu_main::STY<m_ZPG>  ; opcode_table[0x91] = &cpu_main::STA<m_INY> ;
  opcode_table[0x85] = &cpu_main::STA<m_ZPG>  ; opcode_table[0x94] = &cpu_main::STY<m_ZPG> ;
  opcode_table[0x86] = &cpu_main::STX<m_ZPG>  ; opcode_table[0x95] = &cpu_main::STA<m_ZPG> ;
  opcode_table[0x88] = &cpu_main::DEY         ; opcode_table[0x96] = &cpu_main::STX<m_ZPY> ;
  opcode_table[0x8A] = &cpu_main::TXA         ; opcode_table[0x98] = &cpu_main::TYA        ;
  opcode_table[0x8C] = &cpu_main::STY<m_ABS>  ; opcode_table[0x99] = &cpu_main::STA<m_ABY> ;
  opcode_table[0x8D] = &cpu_main::STA<m_ABS>  ; opcode_table[0x9A] = &cpu_main::TXS        ;
  opcode_table[0x8E] = &cpu_main::STX<m_ABS>  ; opcode_table[0x9D] = &cpu_main::STA<m_ABX> ;

  opcode_table[0xA0] = &cpu_main::LDY<m_IMM>  ; opcode_table[0xB0] = &cpu_main::BCS        ;
  opcode_table[0xA1] = &cpu_main::LDA<m_INX>  ; opcode_table[0xB1] = &cpu_main::LDA<m_INY> ;
  opcode_table[0xA2] = &cpu_main::LDX<m_IMM>  ; opcode_table[0xB4] = &cpu_main::LDY<m_ZPG> ;
  opcode_table[0xA4] = &cpu_main::LDY<m_ZPG>  ; opcode_table[0xB5] = &cpu_main::LDA<m_ZPG> ;
  opcode_table[0xA5] = &cpu_main::LDA<m_ZPG>  ; opcode_table[0xB6] = &cpu_main::LDX<m_ZPY> ;
  opcode_table[0xA6] = &cpu_main::LDX<m_ZPG>  ; opcode_table[0xB8] = &cpu_main::CLV        ;
  opcode_table[0xA8] = &cpu_main::TAY         ; opcode_table[0xB9] = &cpu_main::LDA<m_ABY> ;
  opcode_table[0xA9] = &cpu_main::LDA<m_IMM>  ; opcode_table[0xBA] = &cpu_main::TSX        ;
  opcode_table[0xAA] = &cpu_main::TAX         ; opcode_table[0xBC] = &cpu_main::LDY<m_ABX> ;
  opcode_table[0xAC] = &cpu_main::LDY<m_ABS>  ; opcode_table[0xBD] = &cpu_main::LDA<m_ABX> ;
  opcode_table[0xAD] = &cpu_main::LDA<m_ABS>  ; opcode_table[0xBE] = &cpu_main::LDX<m_ABY> ;
  opcode_table[0xAE] = &cpu_main::LDX<m_ABS>  ;

  opcode_table[0xC0] = &cpu_main::CPY<m_IMM>  ; opcode_table[0xD0] = &cpu_main::BNE        ;
  opcode_table[0xC1] = &cpu_main::CMP<m_INX>  ; opcode_table[0xD1] = &cpu_main::CMP<m_INY> ;
  opcode_table[0xC4] = &cpu_main::CPY<m_ZPG>  ; opcode_table[0xD5] = &cpu_main::CMP<m_ZPG> ;
  opcode_table[0xC5] = &cpu_main::CMP<m_ZPG>  ; opcode_table[0xD6] = &cpu_main::DEC<m_ZPG> ;
  opcode_table[0xC6] = &cpu_main::DEC<m_ZPG>  ; opcode_table[0xD8] = &cpu_main::CLD        ;
  opcode_table[0xC8] = &cpu_main::INY         ; opcode_table[0xD9] = &cpu_main::CMP<m_ABY> ;
  opcode_table[0xC9] = &cpu_main::CMP<m_IMM>  ; opcode_table[0xDD] = &cpu_main::CMP<m_ABX> ;
  opcode_table[0xCA] = &cpu_main::DEX         ; opcode_table[0xDE] = &cpu_main::DEC<m_ABX> ;
  opcode_table[0xCC] = &cpu_main::CPY<m_ABS>  ;
  opcode_table[0xCD] = &cpu_main::CMP<m_ABS>  ;
  opcode_table[0xCE] = &cpu_main::DEC<m_ABS>  ;

  opcode_table[0xE0] = &cpu_main::CPX<m_IMM>  ; opcode_table[0xF0] = &cpu_main::BEQ        ;
  opcode_table[0xE1] = &cpu_main::SBC<m_INX>  ; opcode_table[0xF1] = &cpu_main::SBC<m_INY> ;
  opcode_table[0xE4] = &cpu_main::CPX<m_ZPG>  ; opcode_table[0xF5] = &cpu_main::SBC<m_ZPG> ;
  opcode_table[0xE5] = &cpu_main::SBC<m_ZPG>  ; opcode_table[0xF6] = &cpu_main::INC<m_ZPG> ;
  opcode_table[0xE6] = &cpu_main::INC<m_ZPG>  ; opcode_table[0xF8] = &cpu_main::SED        ;
  opcode_table[0xE8] = &cpu_main::INX         ; opcode_table[0xF9] = &cpu_main::SBC<m_ABY> ;
  opcode_table[0xE9] = &cpu_main::SBC<m_IMM>  ; opcode_table[0xFD] = &cpu_main::SBC<m_ABX> ;
  opcode_table[0xEA] = &cpu_main::NOP         ; opcode_table[0xFE] = &cpu_main::INC<m_ABX> ;
  opcode_table[0xEC] = &cpu_main::CPX<m_ABS>  ;
  opcode_table[0xED] = &cpu_main::SBC<m_ABS>  ;
  opcode_table[0xEE] = &cpu_main::INC<m_ABS>  ;
}
