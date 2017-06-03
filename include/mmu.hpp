#ifndef MMU_HPP
#define MMU_HPP

// Beginning of MMU class.

#include "util.hpp"
#include <memory>

template<std::size_t size>
class cpu_core_memory{
  // FIXME: This needs to be heavily re-factored. In particular, writes to ROM Area mean
  // something particularly special, in context of MMU implementation.

  u8 mem[size];
public:
  // Return appropriate memory location, taking mirroring into account.
  u8 operator[] (u16 address){
    u8 data = mem[address];

    if(address<=0x1FFF)
      data = mem[address % 0x0800];

    if(0x2000<=address && address<0x4000)
      data = mem[0x2000 + (address % 8)];

    return data;
  };

  void write_address(u16 address, u8 data);
  u8   read_address(u16 address);

  void zeros(){
    std::memset(mem,0,size);
  }
};

// Reading data stub. Data needs to be read from proper bank of MMU.
template<std::size_t size>
u8 cpu_core_memory<size>::read_address(u16 address){
  u8 data = mem[address];

  if(address<=0x1FFF)
    data = mem[address % 0x0800];

  if(0x2000<=address && address<0x4000)
    data = mem[0x2000 + (address % 8)];

  return data;
}

// Write data stub. Need to intercept various MMU calls.
template<std::size_t size>
void cpu_core_memory<size>::write_address(u16 address, u8 data){
    if(address<=0x1FFF)
      address = address % 0x0800;

    if(0x2000<=address && address<0x4000)
      address = 0x2000 + (address % 8);

    mem[address] = data;
}

#endif /* MMU_HPP */
