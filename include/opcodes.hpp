#ifndef OPCODES_HPP
#define OPCODES_HPP

#include <functional>
#include "util.hpp"
#include "cpu_core.hpp"

// Defines all the opcode function, and a function table.
//std::function<u8()> opcodes[256]; // A function array for 256 opcodes.

//---------Opcodes adapted from http://nesdev.com/6502_cpu.txt ///

struct opcodes {
  cpu_core_6502* core;
  void set_flags(const u8& result);  // Sets zero and sign flag depending on input.
  void do_jump(const u8& offset);    // Performs relative addressing computations.
  u8   subtract(const u8& a, const u8& b);  // Performs a-b and sets relevant flags.
  u8   add(const u8& a, const u8& b);       // Performs a+b and sets the relevant flags.

  // Opcodes that require memory operations.
  template <mem_mode mode> void ADC();   // Add memory to accumulator. Add with carry.
  template <mem_mode mode> void AND();   // AND memory with accumulator.
  template <mem_mode mode> void ASL();   // Shift Left One Bit (Memory or Accumulator)
  template <mem_mode mode> void CMP();   // Compare Memory and Accumulator
  template <mem_mode mode> void CPX();   // Compare Memory and Index X
  template <mem_mode mode> void CPY();   // Compare Memory and Index Y
  template <mem_mode mode> void DEC();   // Decrement Memory by One
  template <mem_mode mode> void EOR();   // "Exclusive-Or" Memory with Accumulator
  template <mem_mode mode> void INC();   // Increment Memory by One
  template <mem_mode mode> void LDA();   // Load Accumulator with Memory
  template <mem_mode mode> void LDX();   // Load Index X with Memory
  template <mem_mode mode> void LDY();   // Load Index Y with Memory
  template <mem_mode mode> void LSR();   // Shift Right One Bit (Memory or Accumulator)
  template <mem_mode mode> void ORA();   // "OR" Memory with Accumulator
  template <mem_mode mode> void ROL();   // Rotate One Bit Left (Memory or Accumulator)
  template <mem_mode mode> void ROR();   // Rotate One Bit Right (Memory or Accumulator)
  template <mem_mode mode> void SBC();   // Subtract Memory from Accumulator with Borrow
  template <mem_mode mode> void STA();   // Store Accumulator in Memory
  template <mem_mode mode> void STX();   // Store Index X in Memory
  template <mem_mode mode> void STY();   // Store Index Y in Memory
  template <mem_mode mode> void BIT();   // Test Bits in Memory with Accumulator

  // No operand opcodes.
  void BCC();   // Branch on Carry Clear
  void BCS();   // Branch on Carry Set
  void BEQ();   // Branch on Result Zero

  void BMI();   // Branch on Result Minus
  void BNE();   // Branch on Result not Zero
  void BPL();   // Branch on Result Plus
  void BRK();   // Force Break
  void BVC();   // Branch on Overflow Clear
  void BVS();   // Branch on Overflow Set
  void CLC();   // Clear Carry Flag
  void CLD();   // Clear Decimal Mode
  void CLI();   // Clear interrupt Disable Bit
  void CLV();   // Clear Overflow Flag
  void DEX();   // Decrement Index X by One
  void DEY();   // Decrement Index Y by One

  void INX();   // Increment Index X by One
  void INY();   // Increment Index Y by One
  void JMP_IND();  // Jump to New Location, Indirect.
  void JMP_ABS();  // Jump to New Location, Direct.
  void JSR();   // Jump to New Location Saving Return Address
  void NOP();   // No Operation
  void PHA();   // Push Accumulator on Stack
  void PHP();   // Push Processor Status on Stack
  void PLA();   // Pull Accumulator from Stack
  void PLP();   // Pull Processor Status from Stack
  void RTI();   // Return from Interrupt
  void RTS();   // Return from Subroutine
  void SEC();   // Set Carry Flag
  void SED();   // Set Decimal Mode
  void SEI();   // Set Interrupt Disable Status

  void TAX();   // Transfer Accumulator to Index X
  void TAY();   // Transfer Accumulator to Index Y
  void TSX();   // Transfer Stack Pointer to Index X
  void TXA();   // Transfer Index X to Accumulator
  void TXS();   // Transfer Index X to Stack Pointer
  void TYA();   // Transfer Index Y to Accumulator
};

#include "opcodes_impl.hpp" // Include the implementation of templated functions.

#endif /* OPCODES_HPP */
