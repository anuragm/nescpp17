// Contains the implementation of all the opcodes.
#include "opcodes.hpp"

// Implement each function.
void opcodes::do_jump(u8 offset)
{
  i8 jump_value = offset<128 ? offset : (offset-256);
  cpu->PC += jump_value;
}

void opcodes::set_flags(){
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
  u8& operand = cpu->operand(mode); // This has side effect of incrementing the PC.
  if (mode==m_IMM)
    cpu->A = cpu->A << 1;
  else
    operand = operand << 1;
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
