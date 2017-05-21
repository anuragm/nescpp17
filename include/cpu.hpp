#ifndef CPU_HPP
#define CPU_HPP

#include "util.hpp"
#include <cstddef>

//Emulate a 6502 CPU.


//A structure to access individual bits.
template<std::size_t bitnum,typename T=u8>
struct bitReg{
  T data;

  T    mask()  { return 1u << bitnum; }
  void set()   { data = (data | mask()) ; }
  void unset() { data = (data & ~mask()); }
  T    get()   { return (data & mask())>>bitnum; }
};

//The status register. It is a 8 bit register, where each bit represents a status.
union statReg{
  bitReg<0> C ; //Carry Flag
  bitReg<1> Z ; //Zero Flag
  bitReg<2> I ; //Interrupt enable/disable flag
  bitReg<3> D ; //Decimal mode flag
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
};

#endif /* CPU_HPP */
