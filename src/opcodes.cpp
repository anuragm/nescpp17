// Contains the implementation of all the opcodes.
#include "opcodes.hpp"
#include "cpu.hpp"

u8 operand(mem_mode mode, cpu_6502& cpu){
  u8 address;
  union {u16 word; u8 bytes[2];} address16;

  switch(mode){
  case IMM: //Immediate mode.
    // The next byte in the program mode is returned.
    return cpu.mem[cpu.PC+1];

  case ZPG: // Zero page mode.
    // The next byte is the address of first 256 bytes of memory. The address is 16 bits
    // (for total of 64 KB memory, but we only use the 8 bits to address the zero'th page.
    address = cpu.mem[cpu.PC+1];
    return cpu.mem[address];

  case ZPX: // Zero Page X
    // The adress is the 8 bits of the next byte, added to the 8 bits of X
    // register. IF the addition overflows, it's mod 0xFF. This is automatically garunteed
    // by C++ standard.
    address = cpu.mem[cpu.PC+1] + cpu.X;
    return cpu.mem[address];

  case ZPY: // Zero Page Y
    // The adress is the 8 bits of the next byte, added to the 8 bits of Y
    // register. IF the addition overflows, it's mod 0xFF. This is automatically garunteed
    // by C++ standard.
    address = cpu.mem[cpu.PC+1] + cpu.Y;
    return cpu.mem[address];

  case ABS: // Absolute memory address.
    //In which case, the next two bytes (little endian)
    // are the memory address. x64 is also little endian. So put the first byte we
    // read in first half of 16 bit address and second byte in 2nd half.
    address16.bytes[0] = cpu.mem[cpu.PC+1];
    address16.bytes[1] = cpu.mem[cpu.PC+2];
    return cpu.mem[address16.word];

  case ABX: // Absolute memory addres w.r.t. X.
    //Read the next two bytes, and add the value stored in X register. Return value in
    //this location.
    address16.bytes[0] = cpu.mem[cpu.PC+1];
    address16.bytes[1] = cpu.mem[cpu.PC+2];
    return cpu.mem[address16.word+cpu.X];

  case ABY: // Absolute memory addres w.r.t. Y.
    address16.bytes[0] = cpu.mem[cpu.PC+1];
    address16.bytes[1] = cpu.mem[cpu.PC+2];
    return cpu.mem[address16.word+cpu.X];

  case IND: // Indirection.
    // Read the next byte. Go the the this memory location, and read the next two
    // bytes. This gives an address. Return this value.
    address = cpu.mem[cpu.PC+1];
    address16.bytes[0] = cpu.mem[address];
    address16.bytes[1] = cpu.mem[address+1];
    return cpu.mem[address16.word];

  case INX: // Indirection X.
    //Read the next byte, add it to X register. Go to this memory location, and read the
    //next two bytes. This gives an address. Return the value at this address.
    address = cpu.mem[cpu.PC+1] + cpu.X;
    address16.bytes[0] = cpu.mem[address];
    address16.bytes[1] = cpu.mem[address+1];
    return cpu.mem[address16.word];

  case INY: // Post-Indexed Indirect Addressing
    // Read the next byte. Go to this memory location, and read the next two bytes. To
    // this value, add Y. This gives an address. Return the value at this address.
    address = cpu.mem[cpu.PC+1];
    address16.bytes[0] = cpu.mem[address];
    address16.bytes[1] = cpu.mem[address+1];
    return cpu.mem[address16.word+cpu.Y];

  case REL: // Relative.
    // Consider the value of next byte as a signed integer. Add it to PC and return that
    // number. (Do we have to set any flags here, or indeed during other calculations? )
    u8 temp = cpu.mem[cpu.PC+1];
    i8 val = temp<128 ? temp : (temp-256);
    return cpu.PC + val;
  };

  return 0;
}
