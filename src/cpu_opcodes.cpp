// Implements the CPU non-templated opcodes.
#include "cpu.hpp"

// Compare and jump operations.
void cpu::BCC() {  // Branch on Carry Clear
  if (this->P.C.get() == 0) do_jump(this->mem[this->PC++]);
}

void cpu::BCS() {  // Branch on Carry Set
  if (this->P.C.get() == 1) do_jump(this->mem[this->PC++]);
}

void cpu::BEQ() {  // Branch on Result Zero
  if (this->P.Z.get() == 1) do_jump(this->mem[this->PC++]);
}

void cpu::BMI() {  // Branch on result minus
  if (this->P.S.get() == 1) do_jump(this->mem[this->PC++]);
}

void cpu::BNE() {  // Branch on result not zero
  if (this->P.Z.get() == 0) do_jump(this->mem[this->PC++]);
}

void cpu::BPL() {  // Branch on result plus
  if (this->P.S.get() == 0) do_jump(this->mem[this->PC++]);
}

void cpu::BRK() {  // Force break.
  // Reference here.
  // http://nesdev.com/the%20%27B%27%20flag%20&%20BRK%20instruction.txt

  this->P.B.set();                            // Break flag is set.
  this->PC++;                                 // Increment the program counter.
  this->push_stack(get_high_byte(this->PC));  // Push the high byte on stack.
  this->push_stack(get_low_byte(this->PC));   // Push the low byte on stack.
  this->push_stack(this->P.byte);             // Push the status flags.

  // And then, set PC to the value found in 0xFFFE and 0xFFFF.
  this->PC = combine_bytes(this->mem[0xFFFE], this->mem[0xFFFF]);
}

void cpu::BVC() {  // Branch on overflow clear.
  if (this->P.V.get() == 0) do_jump(this->mem[this->PC++]);
}

void cpu::BVS() {  // Branch on overflow set
  if (this->P.V.get() == 1) do_jump(this->mem[this->PC++]);
}

void cpu::CLC() {  // Clear carry flag
  this->P.C.clear();
}

void cpu::CLD() {  // Clear decimal mode.
  this->P.D.clear();
}

void cpu::CLI() {  // Clear interrupt disable bit
  this->P.I.clear();
}

void cpu::CLV() {  // Clear overflow flag
  this->P.V.clear();
}

void cpu::DEX() {  // Decrement X. Carry ignored.
  set_flags(--this->X);
}

void cpu::DEY() {  // Decrement Y. Carry ignored.
  set_flags(--this->Y);
}

void cpu::INX() {  // Increment X.
  set_flags(++this->X);
}

void cpu::INY() {  // Increment Y
  set_flags(++this->Y);
}

void cpu::JMP_IND() {  // Indirect jump
  // Read the two bytes. Go the the this memory location, and read the next two
  // bytes. This gives an address. Return this value.
  u8 low_byte = this->mem[this->PC++];
  u8 high_byte = this->mem[this->PC++];
  u16 address = combine_bytes(low_byte, high_byte);

  // BUG in 6502 used in NES. If address is at page boundary (xxFF) , then the
  // high byte is read from (xx00) instead of ((xx+1)00).
  low_byte = this->mem[address];
  u16 hb_address =
      (get_low_byte(address) == 0xFF) ? combine_bytes(00, get_high_byte(address)) : (address + 1);
  high_byte = this->mem[hb_address];
  address = combine_bytes(low_byte, high_byte);

  this->PC = address;  // And jump.
}

void cpu::JMP_ABS() {  // Direct jump to given address.
  u8 low_byte = this->mem[this->PC++];
  u8 high_byte = this->mem[this->PC++];
  u16 address = combine_bytes(low_byte, high_byte);
  this->PC = address;
}

void cpu::JSR() {  // Jump to absolute address, Saving Return Address
  u8 low_byte = this->mem[this->PC++];
  u8 high_byte = this->mem[this->PC++];
  u16 address = combine_bytes(low_byte, high_byte);
  // JSR Bug. At this point, PC is at the next opcode. However, JSR pushes PC-1
  // as the return address.
  this->push_stack(get_high_byte(this->PC - 1));
  this->push_stack(get_low_byte(this->PC - 1));
  this->PC = address;
}

void cpu::NOP() {  // No operation.
}

void cpu::PHA() {  // Push accumulator to stack.
  this->push_stack(this->A);
}

void cpu::PHP() {  // Push processor status to stack.
  this->push_stack(this->P.byte);
}

void cpu::PLA() {  // Pop stack and store in accumulator.
  this->A = this->pop_stack();
  set_flags(this->A);
}

void cpu::PLP() {  // Pop stack and store in process status.
  this->P.byte = this->pop_stack();
}

void cpu::RTI() {  // Return from interrupt
  // An interrupt pushed PC into the stack, high byte followed by low byte. Then
  // it pushed status register. Now, pop back ... so reverse order.
  this->P.byte = this->pop_stack();
  u8 low_byte = this->pop_stack();
  u8 high_byte = this->pop_stack();
  this->PC = combine_bytes(low_byte, high_byte);
}

void cpu::RTS() {  // Return from subroutine.
  // An JSR pushed (PC-1) into the stack, high byte followed by low byte.
  // Now, pop back ... so reverse order.
  u8 low_byte = this->pop_stack();
  u8 high_byte = this->pop_stack();
  this->PC = combine_bytes(low_byte, high_byte) + 1;
}

void cpu::SEC() {  // Set carry flag.
  this->P.C.set();
}

void cpu::SED() {  // Set decimal flag. Not that it does anything.
  this->P.D.set();
}

void cpu::SEI() {  // Srt interrupt disable status.
  this->P.I.set();
}

void cpu::TAX() {  // Transfer accumulator to X
  this->X = this->A;
  set_flags(this->X);
}

void cpu::TAY() {  // Transfer accumulator to Y
  this->Y = this->A;
  set_flags(this->Y);
}

void cpu::TSX() {  // Transfer Stack Pointer to Index X.
  this->X = this->SP;
  set_flags(this->X);
}

void cpu::TXA() {  // Transfer Index X to Accumulator
  this->A = this->X;
  set_flags(this->A);
}

void cpu::TXS() {  // Transfer Index X to Stack Pointer
  this->SP = this->X;
}

void cpu::TYA() {  // Transfer Index Y to Accumulator
  this->A = this->Y;
  set_flags(this->A);
}
