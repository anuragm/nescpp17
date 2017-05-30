#ifndef CPU_HPP
#define CPU_HPP

#include "util.hpp"
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

//----------------- CPU Memory class --------------------
template<std::size_t size>
class cpu_memory{
  u8 mem[size];
public:
  // Return appropriate memory location, taking mirroring into account.
  u8& operator[] (u16 address){
    u8& data = mem[address];

    if(address<=0x1FFF)
      data = mem[address % 0x0800];

    if(0x2000<=address && address<0x4000)
      data = mem[0x2000 + (address % 8)];

    return data;
  };

  void zeros(){
    std::memset(mem,0,size);
  }
};

//----------------- CPU Class --------------------------------
struct cpu_6502 {
  statReg P; // Process status register.
  u8 A; // accumulator register.
  u8 X; // X
  u8 Y; // Y
  u8 SP; //Stack pointer.
  u16 PC; //Program counter.
  cpu_memory<64*1024> mem; // The memory of the machine. Much of this is mirrored. 64 KB
  u8& operand(mem_mode mode); // Returns a reference to the memory where operand is stored.
};

#endif /* CPU_HPP */
