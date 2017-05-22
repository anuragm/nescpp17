#ifndef OPCODES_HPP
#define OPCODES_HPP

#include <functional>
#include "util.hpp"
#include "cpu.hpp"

enum mem_mode // Total 11 modes for NES.
  {
   IMM=0, // Immediate mode
   //--------Zero Page -------------
   ZPG,  //  Zero Page
   ZPX,  //  Zero Page X
   ZPY,  //  Zero Page Y
   //---------Absolute--------------
   ABS,  //  Absolute
   ABX,  //  Absolute X (Indexed Addressing X)
   ABY,  //  Absolute Y (Indexed Addressing Y)
   //---------Indirect--------------
   IND,  //  Indirection.
   INX,  //  Pre-Indexed Indirect Addressing
   INY,  //  Post-Indexed Indirect Addressing
   //---------Relative----------------
   REL   //  Relative
  };

// Returns the operand of the opcode given the memoery mode.
u8 operand(mem_mode mode, cpu_6502& cpu);

// Defines all the opcode function, and a function table.
//std::function<u8()> opcodes[256]; // A function array for 256 opcodes.




#endif /* OPCODES_HPP */
