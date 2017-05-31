// Includes implementation of non-templated opcodes.
#include "opcodes.hpp"

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

void opcodes::DEX() { // Decrement X. Carry ignored.
  set_flags(--cpu->X);
}

void opcodes::DEY() { // Decrement Y. Carry ignored.
  set_flags(--cpu->Y);
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

void opcodes::NOP() { // No operation.
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

void opcodes::SEC() { // Set carry flag.
  cpu->P.C.set();
}

void opcodes::SED() { // Set decimal flag. Not that it does anything.
  cpu->P.D.set();
}

void opcodes::SEI() { // Srt interrupt disable status.
  cpu->P.I.set();
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
