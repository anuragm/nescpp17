// Includes implementation of non-templated opcodes.
#include "opcodes.hpp"

// Compare and jump operations.
void opcodes::BCC(){ //Branch on Carry Clear
  if (core->P.C.get() == 0)
    do_jump(core->mem[core->PC++]);
}

void opcodes::BCS(){   // Branch on Carry Set
  if (core->P.C.get() == 1)
    do_jump(core->mem[core->PC++]);
}

void opcodes::BEQ(){   // Branch on Result Zero
  if (core->P.Z.get() == 1)
    do_jump(core->mem[core->PC++]);
}

void opcodes::BMI() { // Branch on result minus
  if (core->P.S.get() == 1)
    do_jump(core->mem[core->PC++]);
}

void opcodes::BNE() { // Branch on result not zero
  if (core->P.Z.get() == 0)
    do_jump(core->mem[core->PC++]);
}

void opcodes::BPL() { // Branch on result plus
  if (core->P.S.get() == 0)
    do_jump(core->mem[core->PC++]);
}

void opcodes::BRK() { // Force break.
  //Reference here.
  //http://nesdev.com/the%20%27B%27%20flag%20&%20BRK%20instruction.txt

  core->P.B.set(); // Break flag is set.
  core->PC++; //Increment the program counter.
  core->mem[core->SP--] = get_high_byte(core->PC); // Push the high byte on stack.
  core->mem[core->SP--] = get_low_byte(core->PC);  // Push the low byte on stack.
  core->mem[core->SP--] = core->P.byte;            // Push the status flags.

  //And then, set PC to the value found in 0xFFFE and 0xFFFF.
  core->PC = combine_bytes(core->mem[0xFFFE],core->mem[0xFFFF]);
}

void opcodes::BVC() { // Branch on overflow clear.
  if (core->P.V.get() == 0)
    do_jump(core->mem[core->PC++]);
}

void opcodes::BVS() { // Branch on overflow set
  if (core->P.V.get() == 1)
    do_jump(core->mem[core->PC++]);
}

void opcodes::CLC() { // Clear carry flag
  core->P.C.clear();
}

void opcodes::CLD() { // Clear decimal mode.
  core->P.D.clear();
}

void opcodes::CLI() { // Clear interrupt disable bit
  core->P.I.clear();
}

void opcodes::CLV() { // Clear overflow flag
  core->P.V.clear();
}

void opcodes::DEX() { // Decrement X. Carry ignored.
  set_flags(--core->X);
}

void opcodes::DEY() { // Decrement Y. Carry ignored.
  set_flags(--core->Y);
}

void opcodes::INX() { // Increment X.
  set_flags(++core->X);
}

void opcodes::INY() { // Increment Y
  set_flags(++core->Y);
}

void opcodes::JMP() { //Indirect jump
  // Read the two bytes. Go the the this memory location, and read the next two
  // bytes. This gives an address. Return this value.
  u8 low_byte  = core->mem[core->PC++];
  u8 high_byte = core->mem[core->PC++];
  u16 address = combine_bytes(low_byte,high_byte);

  // BUG in 6502 used in NES. If address is at page boundary (xxFF) , then the high byte
  // is read from (xx00) instead of ((xx+1)00).
  low_byte = core->mem[address];
  u16 hb_address =
    (get_low_byte(address) == 0xFF)? combine_bytes(00,get_high_byte(address)) : (address+1);
  high_byte = core->mem[hb_address];
  address = combine_bytes(low_byte,high_byte);

  core-> PC = address; // And jump.
}

void opcodes::JSR() { //Jump to absolute address
  u8 low_byte = core->mem[core->PC++];
  u8 high_byte = core->mem[core->PC++];
  u16 address = combine_bytes(low_byte,high_byte);
  // JSR Bug. At this point, PC is at the next opcode. However, JSR pushes PC-1 as the
  // return address.
  core->mem[core->SP--] = get_high_byte(core->PC-1);
  core->mem[core->SP--] = get_low_byte(core->PC-1);
  core->PC = address;
}

void opcodes::NOP() { // No operation.
}

void opcodes::PHA() { // Push accumulator to stack.
  core->mem[core->SP--] = core->A;

}

void opcodes::PHP() { // Push processor status to stack.
  core->mem[core->SP--] = core->P.byte;
}

void opcodes::PLA() { // Pop stack and store in accumulator.
  core->A = core->mem[core->SP++];
  set_flags(core->A);
}

void opcodes::PLP() { // Pop stack and store in process status.
  core->P.byte = core->mem[core->SP++];
}

void opcodes::RTI() { // Return from interrupt
  // An interrupt pushed PC into the stack, high byte followed by low byte. Then it pushed
  // status register. Now, pop back ... so reverse order.
  core->P.byte = core->mem[core->SP--];
  u8 low_byte = core->mem[core->SP--];
  u8 high_byte = core->mem[core->SP--];
  core->PC = combine_bytes(low_byte,high_byte);
}

void opcodes::RTS() { // Return from subroutine.
  // An JSR pushed (PC-1) into the stack, high byte followed by low byte.
  // Now, pop back ... so reverse order.
  u8 low_byte = core->mem[core->SP--];
  u8 high_byte = core->mem[core->SP--];
  core->PC = combine_bytes(low_byte,high_byte)+1;
}

void opcodes::SEC() { // Set carry flag.
  core->P.C.set();
}

void opcodes::SED() { // Set decimal flag. Not that it does anything.
  core->P.D.set();
}

void opcodes::SEI() { // Srt interrupt disable status.
  core->P.I.set();
}

void opcodes::TAX() { // Transfer accumulator to X
  core->X = core->A;
  set_flags(core->X);
}

void opcodes::TAY() { // Transfer accumulator to Y
  core->Y = core->A;
  set_flags(core->Y);
}

void opcodes::TSX(){ // Transfer Stack Pointer to Index X.
  core->X = core->SP;
  set_flags(core->X);
}

void opcodes::TXA(){   // Transfer Index X to Accumulator
  core->A = core->X;
  set_flags(core->A);
}

void opcodes::TXS(){   // Transfer Index X to Stack Pointer
  core->SP = core->X;
}

void opcodes::TYA(){    // Transfer Index Y to Accumulator
  core->A = core->Y;
  set_flags(core->A);
}
