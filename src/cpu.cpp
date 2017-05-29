// CPU Implementation

#include "cpu.hpp"

u8& cpu_6502::operand(mem_mode mode){
  u8 address;
  union {u16 word; u8 bytes[2];} address16;

  switch(mode){
  case m_IMM: //Immediate mode.
    // The next byte in the program mode is returned.
    return mem[PC+1];

  case m_ZPG: // Zero page mode.
    // The next byte is the address of first 256 bytes of memory. The address is 16 bits
    // (for total of 64 KB memory, but we only use the 8 bits to address the zero'th page.
    address = mem[PC+1];
    return mem[address];

  case m_ZPX: // Zero Page X
    // The adress is the 8 bits of the next byte, added to the 8 bits of X
    // register. IF the addition overflows, it's mod 0xFF. This is automatically garunteed
    // by C++ standard.
    address = mem[PC+1] + X;
    return mem[address];

  case m_ZPY: // Zero Page Y
    // The adress is the 8 bits of the next byte, added to the 8 bits of Y
    // register. IF the addition overflows, it's mod 0xFF. This is automatically garunteed
    // by C++ standard.
    address = mem[PC+1] + Y;
    return mem[address];

  case m_ABS: // Absolute memory address.
    //In which case, the next two bytes (little endian)
    // are the memory address. x64 is also little endian. So put the first byte we
    // read in first half of 16 bit address and second byte in 2nd half.
    address16.bytes[0] = mem[PC+1];
    address16.bytes[1] = mem[PC+2];
    return mem[address16.word];

  case m_ABX: // Absolute memory addres w.r.t. X.
    //Read the next two bytes, and add the value stored in X register. Return value in
    //this location.
    address16.bytes[0] = mem[PC+1];
    address16.bytes[1] = mem[PC+2];
    return mem[address16.word+X];

  case m_ABY: // Absolute memory addres w.r.t. Y.
    address16.bytes[0] = mem[PC+1];
    address16.bytes[1] = mem[PC+2];
    return mem[address16.word+X];

  case m_IND: // Indirection.
    // Read the next byte. Go the the this memory location, and read the next two
    // bytes. This gives an address. Return this value.
    address = mem[PC+1];
    address16.bytes[0] = mem[address];
    address16.bytes[1] = mem[address+1];
    return mem[address16.word];

  case m_INX: // Indirection X.
    //Read the next byte, add it to X register. Go to this memory location, and read the
    //next two bytes. This gives an address. Return the value at this address.
    address = mem[PC+1] + X;
    address16.bytes[0] = mem[address];
    address16.bytes[1] = mem[address+1];
    return mem[address16.word];

  case m_INY: // Post-Indexed Indirect Addressing
    // Read the next byte. Go to this memory location, and read the next two bytes. To
    // this value, add Y. This gives an address. Return the value at this address.
    address = mem[PC+1];
    address16.bytes[0] = mem[address];
    address16.bytes[1] = mem[address+1];
    return mem[address16.word+Y];

    /*case m_REL: // Relative.
    // Consider the value of next byte as a signed integer. Add it to PC and return that
    // number. (Do we have to set any flags here, or indeed during other calculations? )
    u8 temp = mem[PC+1];
    i8 val = temp<128 ? temp : (temp-256);
    return PC + val; */

  default:
    return mem[PC]; // Control should never reach here.
  };

}
