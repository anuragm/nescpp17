#ifndef CPU_MAIN_IMPL_HPP
#define CPU_MAIN_IMPL_HPP

#include "cpu_main.hpp"
#include "util.hpp"

// Implement each function.
inline void cpu_main::do_jump(const u8& offset)
{
  i16 jump_value = offset<128 ? offset : (i16(offset)-256);
  this->PC += jump_value;
}

inline void cpu_main::set_flags(const u8& result){ // Set zero and sign flag.
  this->P.Z = (result==0);    // Zero flag
  this->P.S = (result>=0x80); // Sign flag
}

inline u8 cpu_main::add(const u8& a, const u8& b){ //Add a with b
  u8 result = a+b;
  this->P.C = ((UINT8_MAX-a) < b); // Detect overflow.
  set_flags(result);
  return result;
}

inline u8 cpu_main::subtract(const u8& a, const u8& b){ // Subtract b from a
  this->P.C = (a>=b);
  u8 result = a-b; // C++ standard ensure correct result.
  set_flags(result);
  return result;
}

template <mem_mode mode>
void cpu_main::ADC(){   // Add memory to accumulator. Add with carry.
  u8 operand = this->operand(mode);
  u8 op1 = this->A;
  u8 op2 = operand;
  if (this->P.C.get()==1) op2++;

  u8 result = add(op1,op2); // This set Z,S and C flags.
  this->A = result;
  //Detect signed overflow, and set V accordingly. http://archive.is/VAxtz
  this->P.V = (op1^result) & (op2^result) & 0x80;
}

template <mem_mode mode> // "AND" memory with accumulator
void cpu_main::AND(){
  u8 operand = this->operand(mode); // This has side effect of incrementing the PC.
  this->A = this->A & operand;
  set_flags(this->A);
}

template <mem_mode mode> // Shift Left One Bit (Memory or Accumulator)
void cpu_main::ASL(){
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  u8 high_bit = (operand & 0x80) >> 7;
  this->P.C = high_bit;

  operand = operand << 1;
  set_flags(operand);
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::BIT(){ // Test bits in memory with accumulator
  u8 operand = this->operand(mode);

  this->P.Z = (this->A & operand);
  this->P.S = (operand & 0b10000000);
  this->P.V = (operand & 0b01000000);
}

template <mem_mode mode>
void cpu_main::CMP() { // Compare memory and accumulator.
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);
  subtract(this->A, operand);
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::CPX() { // Compare memory and X.
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  subtract(this->X, operand);
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::CPY() { // Compare memory and Y.
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  subtract(this->Y, operand);
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::DEC() { // Decrement memory. Carry ignored.
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  set_flags(--operand);
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::EOR() { // XOR Accumulator with memory.
  u8 operand = this->operand(mode);
  set_flags(this->A^operand);
}

template <mem_mode mode>
void cpu_main::INC() { // Increment memory. Carry ignored.
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  set_flags(++operand);
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::LDA(){ // Load memory into accumulator.
  u8 operand = this->operand(mode);
  this->A = operand;
  set_flags(this->A);
}

template <mem_mode mode>
void cpu_main::LDX(){ // Load memory into X.
  u8 operand = this->operand(mode);
  this->X = operand;
  set_flags(this->X);
}

template <mem_mode mode>
void cpu_main::LDY(){ // Load memory with Y.
  u8 operand = this->operand(mode);
  this->Y = operand;
  set_flags(this->Y);
}

template <mem_mode mode>
void cpu_main::LSR() { // Logical shift right.
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  this->P.C = (operand%2);
  operand = operand >> 1;
  set_flags(operand);
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::ORA() { // "OR" memory with accumulator.
  u8 operand = this->operand(mode);
  set_flags(this->A | operand);
}

template <mem_mode mode>
void cpu_main::ROL() { // Rotate one bit left.
  //Old carry bit becomes the LSB and the old MSB becomes the carry bit.
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  u8 old_carry = this->P.C.get();
  u8 new_carry = (operand & 0x80);

  operand = (operand << 1); // Left shift.
  operand = operand | old_carry; // Put old carry on LSB.
  this->P.C = new_carry; // old MSB becomes new carry bit.

  set_flags(operand);
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::ROR() { // Rotate one bit right.
  //Old carry bit becomes the MSB and the old LSB becomes the carry bit.
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  u8 old_carry = this->P.C.get();
  u8 new_carry = (operand & 0x01);

  operand = (operand >> 1); // Right shift.
  operand = operand | (old_carry<<7); // Put old carry on MSB.
  this->P.C = new_carry; // old LSB becomes new carry bit.

  set_flags(operand);
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::SBC() { // Subtract operand from accumulator with borrow.
  // SBC does A -> A - M - (1-C) and then sets relevant flags. Flags are set as if
  // (M+(1-C)) was subtracted from A.
  u8 operand = this->operand(mode);
  u8 op1 = this->A;
  u8 op2 = operand;
  if (this->P.C.get()==0) op2++;

  u8 result = subtract(op1,op2);
  this->A = result;
  // Set the overflow flag if needed.
  this->P.V = (op1^result) & ((~op2)^result) & 0x80;
}

template <mem_mode mode>
void cpu_main::STA() { // // Store Accumulator in Memory
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  operand = this->A;
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::STX(){   // Store Index X in Memory
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  operand = this->X;
  this->mem.write_address(address,operand);
}

template <mem_mode mode>
void cpu_main::STY(){   // Store Index Y in Memory
  u16 address = this->get_address(mode);
  u8 operand  = this->mem.read_address(address);

  operand = this->Y;
  this->mem.write_address(address,operand);
}

#endif /* CPU_MAIN_IMPL_HPP */
