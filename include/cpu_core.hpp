#ifndef CPU_CORE_HPP
#define CPU_CORE_HPP

#include "util.hpp"
#include "mmu.hpp"
#include <cstdint>
#include <cstddef>
#include <cstring>

//Emulate a 6502 CPU.

//The status register. It is a 8 bit register, where each bit represents a status.
union statReg{
  bitReg<0> C ; //Carry Flag
  bitReg<1> Z ; //Zero Flag
  bitReg<2> I ; //Interrupt enable/disable flag
  bitReg<3> D ; //Decimal mode flag. Not working on NES.
  bitReg<4> B ; //Set when software interrupt is executed.
  bitReg<5> UNUSED ;
  bitReg<6> V ; //Overflow flag
  bitReg<7> S ; //Sign flag
  u8 byte;
};

//----------------- CPU Core Class --------------------------------
struct cpu_core_6502 {
  statReg P; // Process status register.
  u8 A; // accumulator register.
  u8 X; // X
  u8 Y; // Y
  u8 SP; //Stack pointer.
  u16 PC; //Program counter.
  cpu_core_memory<64*1024> mem; // The memory of the machine. 64 KB

  u16 get_address(mem_mode mode); // Return data address for given memory mode.
  u8  operand(mem_mode mode);     // Return data stored at said address.

  //Stack operations.
  void push_stack(u8 data);
  u8   pop_stack();
};

#endif /* CPU_CORE_HPP */
