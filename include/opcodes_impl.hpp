#ifndef OPCODES_IMPL_HPP

#define OPCODES_IMPL_HPP
// Contains the implementation of all the opcodes. This must be done in a header file due
// to use of templates.

#include "opcodes.hpp"
#include "util.hpp"

// Implement each function.
inline void opcodes::do_jump(const u8& offset)
{
  i16 jump_value = offset<128 ? offset : (i16(offset)-256);
  core->PC += jump_value;
}

inline void opcodes::set_flags(const u8& result){ // Set zero and sign flag.
  core->P.Z = (result==0);    // Zero flag
  core->P.S = (result>=0x80); // Sign flag
}

inline u8 opcodes::add(const u8& a, const u8& b){ //Add a with b
  u8 result = a+b;
  core->P.C = ((UINT8_MAX-a) < b); // Detect overflow.
  set_flags(result);
  return result;
}

inline u8 opcodes::subtract(const u8& a, const u8& b){ // Subtract b from a
  core->P.C = (a>=b);
  u8 result = a-b; // C++ standard ensure correct result.
  set_flags(result);
  return result;
}

template <mem_mode mode>
void opcodes::ADC(){   // Add memory to accumulator. Add with carry.
  u8 operand = core->operand(mode);
  u8 op1 = core->A;
  u8 op2 = operand;
  if (core->P.C.get()==1) op2++;

  u8 result = add(op1,op2); // This set Z,S and C flags.
  core->A = result;
  //Detect signed overflow, and set V accordingly. http://archive.is/VAxtz
  core->P.V = (op1^result) & (op2^result) & 0x80;
}

template <mem_mode mode> // "AND" memory with accumulator
void opcodes::AND(){
  u8 operand = core->operand(mode); // This has side effect of incrementing the PC.
  core->A = core->A & operand;
  set_flags(core->A);
}

template <mem_mode mode> // Shift Left One Bit (Memory or Accumulator)
void opcodes::ASL(){
  u8& operand = core->operand(mode);

  u8 high_bit = (operand & 0x80) >> 7;
  core->P.C = high_bit;

  operand = operand << 1;
  set_flags(operand);
}

template <mem_mode mode>
void opcodes::BIT(){ // Test bits in memory with accumulator
  u8 operand = core->operand(mode);

  core->P.Z = (core->A & operand);
  core->P.S = (operand & 0b10000000);
  core->P.V = (operand & 0b01000000);
}

template <mem_mode mode>
void opcodes::CMP() { // Compare memory and accumulator.
  u8& operand = core->operand(mode);
  subtract(core->A, operand);
}

template <mem_mode mode>
void opcodes::CPX() { // Compare memory and X.
  u8& operand = core->operand(mode);
  subtract(core->X, operand);
}

template <mem_mode mode>
void opcodes::CPY() { // Compare memory and Y.
  u8& operand = core->operand(mode);
  subtract(core->Y, operand);
}

template <mem_mode mode>
void opcodes::DEC() { // Decrement memory. Carry ignored.
  u8& operand = core->operand(mode);
  set_flags(--operand);
}

template <mem_mode mode>
void opcodes::EOR() { // XOR Accumulator with memory.
  u8 operand = core->operand(mode);
  set_flags(core->A^operand);
}

template <mem_mode mode>
void opcodes::INC() { // Increment memory. Carry ignored.
  u8& operand = core->operand(mode);
  set_flags(++operand);
}

template <mem_mode mode>
void opcodes::LDA(){ // Load memory into accumulator.
  u8 operand = core->operand(mode);
  core->A = operand;
  set_flags(core->A);
}

template <mem_mode mode>
void opcodes::LDX(){ // Load memory into X.
  u8 operand = core->operand(mode);
  core->X = operand;
  set_flags(core->X);
}

template <mem_mode mode>
void opcodes::LDY(){ // Load memory with Y.
  u8 operand = core->operand(mode);
  core->Y = operand;
  set_flags(core->Y);
}

template <mem_mode mode>
void opcodes::LSR() { // Logical shift right.
  u8& operand = core->operand(mode);
  core->P.C = (operand%2);
  operand = operand >> 1;
  set_flags(operand);
}

template <mem_mode mode>
void opcodes::ORA() { // "OR" memory with accumulator.
  u8 operand = core->operand(mode);
  set_flags(core->A | operand);
}

template <mem_mode mode>
void opcodes::ROL() { // Rotate one bit left.
  //Old carry bit becomes the LSB and the old MSB becomes the carry bit.
  u8& operand = core->operand(mode);
  u8 old_carry = core->P.C.get();
  u8 new_carry = (operand & 0x80);

  operand = (operand << 1); // Left shift.
  operand = operand | old_carry; // Put old carry on LSB.
  core->P.C = new_carry; // old MSB becomes new carry bit.

  set_flags(operand);
}

template <mem_mode mode>
void opcodes::ROR() { // Rotate one bit right.
  //Old carry bit becomes the MSB and the old LSB becomes the carry bit.
  u8& operand  = core->operand(mode);
  u8 old_carry = core->P.C.get();
  u8 new_carry = (operand & 0x01);

  operand = (operand >> 1); // Right shift.
  operand = operand | (old_carry<<7); // Put old carry on MSB.
  core->P.C = new_carry; // old LSB becomes new carry bit.

  set_flags(operand);
}

template <mem_mode mode>
void opcodes::SBC() { // Subtract operand from accumulator with borrow.
  // SBC does A -> A - M - (1-C) and then sets relevant flags. Flags are set as if
  // (M+(1-C)) was subtracted from A.
  u8 operand = core->operand(mode);
  u8 op1 = core->A;
  u8 op2 = operand;
  if (core->P.C.get()==0) op2++;

  u8 result = subtract(op1,op2);
  core->A = result;
  // Set the overflow flag if needed.
  core->P.V = (op1^result) & ((~op2)^result) & 0x80;
}

template <mem_mode mode>
void opcodes::STA() { // // Store Accumulator in Memory
  u8& operand = core->operand(mode);
  operand = core->A;
}

template <mem_mode mode>
void opcodes::STX(){   // Store Index X in Memory
  u8& operand = core->operand(mode);
  operand = core->X;
}

template <mem_mode mode>
void opcodes::STY(){   // Store Index Y in Memory
  u8& operand = core->operand(mode);
  operand = core->Y;
}

#endif
