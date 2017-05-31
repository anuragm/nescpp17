#ifndef OPCODES_IMPL_HPP

#define OPCODES_IMPL_HPP
// Contains the implementation of all the opcodes. This must be done in a header file due
// to use of templates.

#include "opcodes.hpp"

// Implement each function.
inline void opcodes::do_jump(u8 offset)
{
  i8 jump_value = offset<128 ? offset : (offset-256);
  cpu->PC += jump_value;
}

inline void opcodes::set_flags(u8 result){ // Set zero and sign flag.
  (result==0) ? cpu->P.Z.set() : cpu->P.Z.clear(); // Zero flag
  (result>=0x80) ? cpu->P.S.set() : cpu->P.S.clear(); // Sign flag
}

inline u8 opcodes::subtract(u8 a, u8 b){ // Subtract b from a
  (a>b) ? cpu->P.C.set() : cpu->P.C.clear();
  u8 result = a-b; // C++ standard ensure correct result.
  set_flags(result);
  return result;
}

template <mem_mode mode> // "AND" memory with accumulator
void opcodes::AND(){
  u8 operand = cpu->operand(mode); // This has side effect of incrementing the PC.
  cpu->A = cpu->A & operand;

  if (~cpu->A)
    cpu->P.Z.set();

  if (cpu->A & 0x10)
    cpu->P.S.set();
}

template <mem_mode mode> // Shift Left One Bit (Memory or Accumulator)
void opcodes::ASL(){
  u8& operand = cpu->operand(mode);

  u8 high_bit = (operand & 0x80) >> 7;
  cpu->P.C = high_bit;

  operand = operand << 1;
  set_flags(operand);
}

template <mem_mode mode>
void opcodes::BIT(){ // Test bits in memory with accumulator
  u8 operand = cpu->operand(mode);
  if (cpu->A & operand)
    cpu->P.Z.set();

  (operand & 0b10000000) ? cpu->P.S.set() : cpu->P.S.clear();
  (operand & 0b01000000) ? cpu->P.V.set() : cpu->P.V.clear();
}

template <mem_mode mode>
void opcodes::CMP() { // Compare memory and accumulator.
  u8& operand = cpu->operand(mode);
  subtract(cpu->A, operand);
}

template <mem_mode mode>
void opcodes::CPX() { // Compare memory and X.
  u8& operand = cpu->operand(mode);
  subtract(cpu->X, operand);
}

template <mem_mode mode>
void opcodes::CPY() { // Compare memory and Y.
  u8& operand = cpu->operand(mode);
  subtract(cpu->Y, operand);
}

template <mem_mode mode>
void opcodes::DEC() { // Decrement memory. Carry ignored.
  u8& operand = cpu->operand(mode);
  set_flags(--operand);
}

template <mem_mode mode>
void opcodes::EOR() { // XOR Accumulator with memory.
  u8 operand = cpu->operand(mode);
  set_flags(cpu->A^operand);
}

template <mem_mode mode>
void opcodes::INC() { // Increment memory. Carry ignored.
  u8& operand = cpu->operand(mode);
  set_flags(++operand);
}

template <mem_mode mode>
void opcodes::LDA(){ // Load memory into accumulator.
  u8 operand = cpu->operand(mode);
  cpu->A = operand;
  set_flags(cpu->A);
}

template <mem_mode mode>
void opcodes::LDX(){ // Load memory into X.
  u8 operand = cpu->operand(mode);
  cpu->X = operand;
  set_flags(cpu->X);
}

template <mem_mode mode>
void opcodes::LDY(){ // Load memory with Y.
  u8 operand = cpu->operand(mode);
  cpu->Y = operand;
  set_flags(cpu->Y);
}

template <mem_mode mode>
void opcodes::LSR() { // Logical shift right.
  u8& operand = cpu->operand(mode);
  cpu->P.C = (operand%2);
  operand = operand >> 1;
  set_flags(operand);
}

template <mem_mode mode>
void opcodes::ORA() { // "OR" memory with accumulator.
  u8 operand = cpu->operand(mode);
  set_flags(cpu->A | operand);
}

template <mem_mode mode>
void opcodes::ROL() { // Rotate one bit left.
  //Old carry bit becomes the LSB and the old MSB becomes the carry bit.
  u8& operand = cpu->operand(mode);
  u8 old_carry = cpu->P.C.get();
  u8 new_carry = (operand & 0x80);

  operand = (operand << 1); // Left shift.
  operand = operand | old_carry; // Put old carry on LSB.
  cpu->P.C = new_carry; // old MSB becomes new carry bit.

  set_flags(operand);
}

template <mem_mode mode>
void opcodes::ROR() { // Rotate one bit right.
  //Old carry bit becomes the MSB and the old LSB becomes the carry bit.
  u8& operand  = cpu->operand(mode);
  u8 old_carry = cpu->P.C.get();
  u8 new_carry = (operand & 0x01);

  operand = (operand >> 1); // Right shift.
  operand = operand | (old_carry<<7); // Put old carry on MSB.
  cpu->P.C = new_carry; // old LSB becomes new carry bit.

  set_flags(operand);
}

template <mem_mode mode>
void opcodes::SBC() { // Subtract operand from accumulator with borrow.
  // SBC does A -> A - M - (1-C) and then sets relevant flags. Flags are set as if
  // (M+(1-C)) was subtracted from A.
  u8 operand = cpu->operand(mode);
  u8 op1 = cpu->A;
  u8 op2 = operand;
  if (cpu->P.C.get()==0) op2++;

  cpu->A = subtract(op1,op2);
}

template <mem_mode mode>
void opcodes::STA() { // // Store Accumulator in Memory
  u8& operand = cpu->operand(mode);
  operand = cpu->A;
}

template <mem_mode mode>
void opcodes::STX(){   // Store Index X in Memory
  u8& operand = cpu->operand(mode);
  operand = cpu->X;
}

template <mem_mode mode>
void opcodes::STY(){   // Store Index Y in Memory
  u8& operand = cpu->operand(mode);
  operand = cpu->Y;
}

#endif
