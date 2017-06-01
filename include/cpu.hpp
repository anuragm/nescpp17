#ifndef CPU_HPP
#define CPU_HPP
//Defines the total CPU class.

#include "util.hpp"
#include "cpu_core.hpp"
#include "opcodes.hpp"

class cpu {
  typedef void (opcodes::*opcode_fn)();
  cpu_core_6502 cpu_core;
  opcodes code;
  opcode_fn opcode_table[256];

public:
  cpu(); // Default constructor.
};

#endif /* CPU_HPP */
