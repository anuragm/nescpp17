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
  if (mode==m_IMM)
    cpu->A = cpu->A << 1;
  else
    operand = operand << 1;

  // FIXME : Take into account flags.
}

// Compare and jump operations.
void opcodes::BCC(){ //Branch on Carry Clear
  if (cpu->P.C.get() == 0)
    do_jump(cpu->mem[cpu->PC++]);
}

void opcodes::BCS(){   // Branch on Carry Set
  if (cpu->P.C.get() == 1)
    do_jump(cpu->mem[cpu->PC++]);
}

void opcodes::BEQ(){   // Branch on Result Zero
  if (cpu->P.Z.get() == 1)
    do_jump(cpu->mem[cpu->PC++]);
}

void opcodes::BMI() { // Branch on result minus
  if (cpu->P.S.get() == 1)
    do_jump(cpu->mem[cpu->PC++]);
}

void opcodes::BNE() { // Branch on result not zero
  if (cpu->P.Z.get() == 0)
    do_jump(cpu->mem[cpu->PC++]);
}

void opcodes::BPL() { // Branch on result plus
  if (cpu->P.S.get() == 0)
    do_jump(cpu->mem[cpu->PC++]);
}

void opcodes::BRK() { // Force break.
  //Reference here.
  //http://nesdev.com/the%20%27B%27%20flag%20&%20BRK%20instruction.txt

  cpu->P.B.set(); // Break flag is set.
  cpu->PC++; //Increment the program counter.
  cpu->mem[cpu->SP--] = get_high_byte(cpu->PC); // Push the high byte on stack.
  cpu->mem[cpu->SP--] = get_low_byte(cpu->PC);  // Push the low byte on stack.
  cpu->mem[cpu->SP--] = cpu->P.byte;            // Push the status flags.

  //And then, set PC to the value found in 0xFFFE and 0xFFFF.
  cpu->PC = combine_bytes(cpu->mem[0xFFFE],cpu->mem[0xFFFF]);
}

template <mem_mode mode>
void opcodes::BIT(){ // Test bits in memory with accumulator
  u8 operand = cpu->operand(mode);
  if (cpu->A & operand)
    cpu->P.Z.set();

  (operand & 0b10000000) ? cpu->P.S.set() : cpu->P.S.clear();
  (operand & 0b01000000) ? cpu->P.V.set() : cpu->P.V.clear();
}

void opcodes::BVC() { // Branch on overflow clear.
  if (cpu->P.V.get() == 0)
    do_jump(cpu->mem[cpu->PC++]);
}

void opcodes::BVS() { // Branch on overflow set
  if (cpu->P.V.get() == 1)
    do_jump(cpu->mem[cpu->PC++]);
}

void opcodes::CLC() { // Clear carry flag
  cpu->P.C.clear();
}

void opcodes::CLD() { // Clear decimal mode.
  cpu->P.D.clear();
}

void opcodes::CLI() { // Clear interrupt disable bit
  cpu->P.I.clear();
}

void opcodes::CLV() { // Clear overflow flag
  cpu->P.V.clear();
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

void opcodes::DEX() { // Decrement X. Carry ignored.
  set_flags(--cpu->X);
}

void opcodes::DEY() { // Decrement Y. Carry ignored.
  set_flags(--cpu->Y);
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

void opcodes::INX() { // Increment X.
  set_flags(++cpu->X);
}

void opcodes::INY() { // Increment Y
  set_flags(++cpu->Y);
}

void opcodes::JMP() { //Indirect jump
  // Read the two bytes. Go the the this memory location, and read the next two
  // bytes. This gives an address. Return this value.
  u8 low_byte  = cpu->mem[cpu->PC++];
  u8 high_byte = cpu->mem[cpu->PC++];
  u16 address = combine_bytes(low_byte,high_byte);

  // BUG in 6502 used in NES. If address is at page boundary (xxFF) , then the high byte
  // is read from (xx00) instead of ((xx+1)00).
  low_byte = cpu->mem[address];
  u16 hb_address =
    (get_low_byte(address) == 0xFF)? combine_bytes(00,get_high_byte(address)) : (address+1);
  high_byte = cpu->mem[hb_address];
  address = combine_bytes(low_byte,high_byte);

  cpu-> PC = address; // And jump.
}

void opcodes::JSR() { //Jump to absolute address
  u8 low_byte = cpu->mem[cpu->PC++];
  u8 high_byte = cpu->mem[cpu->PC++];
  u16 address = combine_bytes(low_byte,high_byte);
  // JSR Bug. At this point, PC is at the next opcode. However, JSR pushes PC-1 as the
  // return address.
  cpu->mem[cpu->SP--] = get_high_byte(cpu->PC-1);
  cpu->mem[cpu->SP--] = get_low_byte(cpu->PC-1);
  cpu->PC = address;
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
void opcodes::LSR() { //Logical shift right.
  u8& operand = cpu->operand(mode);
  (operand%2 == 0)? cpu->P.C.clear() : cpu->P.C.set();
  operand = operand >> 1;
  set_flags(operand);
}

void opcodes::NOP() { // No operation.
}

template <mem_mode mode>
void opcodes::ORA() { // "OR" memory with accumulator.
  u8 operand = cpu->operand(mode);
  set_flags(cpu->A | operand);
}

void opcodes::PHA() { // Push accumulator to stack.
  cpu->mem[cpu->SP--] = cpu->A;

}

void opcodes::PHP() { // Push processor status to stack.
  cpu->mem[cpu->SP--] = cpu->P.byte;
}

void opcodes::PLA() { // Pop stack and store in accumulator.
  cpu->A = cpu->mem[cpu->SP++];
  set_flags(cpu->A);
}

void opcodes::PLP() { // Pop stack and store in process status.
  cpu->P.byte = cpu->mem[cpu->SP++];
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

void opcodes::RTI() { // Return from interrupt
  // An interrupt pushed PC into the stack, high byte followed by low byte. Then it pushed
  // status register. Now, pop back ... so reverse order.
  cpu->P.byte = cpu->mem[cpu->SP--];
  u8 low_byte = cpu->mem[cpu->SP--];
  u8 high_byte = cpu->mem[cpu->SP--];
  cpu->PC = combine_bytes(low_byte,high_byte);
}

void opcodes::RTS() { // Return from subroutine.
  // An JSR pushed (PC-1) into the stack, high byte followed by low byte.
  // Now, pop back ... so reverse order.
  u8 low_byte = cpu->mem[cpu->SP--];
  u8 high_byte = cpu->mem[cpu->SP--];
  cpu->PC = combine_bytes(low_byte,high_byte)+1;
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

void opcodes::SEC() { // Set carry flag.
  cpu->P.C.set();
}

void opcodes::SED() { // Set decimal flag. Not that it does anything.
  cpu->P.D.set();
}

void opcodes::SEI() { // Srt interrupt disable status.
  cpu->P.I.set();
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

void opcodes::TAX() { // Transfer accumulator to X
  cpu->X = cpu->A;
  set_flags(cpu->X);
}

void opcodes::TAY() { // Transfer accumulator to Y
  cpu->Y = cpu->A;
  set_flags(cpu->Y);
}

void opcodes::TSX(){ // Transfer Stack Pointer to Index X.
  cpu->X = cpu->SP;
  set_flags(cpu->X);
}

void opcodes::TXA(){   // Transfer Index X to Accumulator
  cpu->A = cpu->X;
  set_flags(cpu->A);
}


void opcodes::TXS(){   // Transfer Index X to Stack Pointer
  cpu->SP = cpu->X;
}

void opcodes::TYA(){    // Transfer Index Y to Accumulator
  cpu->A = cpu->Y;
  set_flags(cpu->A);
}

#endif
