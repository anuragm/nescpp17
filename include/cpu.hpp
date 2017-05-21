#ifndef CPU_HPP
#define CPU_HPP

#include "util.hpp"
#include <cstdint>
#include <cstddef>

//Emulate a 6502 CPU.

//The status register. It is a 8 bit register, where each bit represents a status.
union statReg{
  bitReg<0> C ; //Carry Flag
  bitReg<1> Z ; //Zero Flag
  bitReg<2> I ; //Interrupt enable/disable flag
  bitReg<3> D ; //Decimal mode flag. Not working on NES.
  bitReg<4> B ; //Set when software interrupt is execulted.
  bitReg<5> UNUSED ;
  bitReg<6> V ; //Overflow flag
  bitReg<7> S ; //Sign flag
  u8 byte;
};

struct cpu_6502 {
  statReg P; // Process status register.
  u8 A; // accumulator register.
  u8 X; // X
  u8 Y; // Y
  u8 SP; //Stack pointer.
  u8 PC; //Program counter.
  u8 mem[64*1024]; // The memeory of the machine. Much of this is mirrored. 64 KB
};

struct ppu_2c02{
  u8 mem[16*1024]; //PPU memory. 16 KB Physical, 64 KB virtual
  u8 spr_ram[256]; //Sprite RAM. 256 Bytes.
};

#endif /* CPU_HPP */
