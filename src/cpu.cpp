// CPU Implementation

#include "cpu.hpp"

u8& cpu_6502::operand(mem_mode mode){
  //When control reaches here, the PC is at one byte after the opcode. For example, for
  //immediate mode, the value of memory at the program counter is returned, and PC is
  //incremented by 1, etc.
  u16 address, hb_address; u8 low_byte, high_byte;

  switch(mode){
  case m_IMM: //Immediate mode.
    // The next byte in the program mode is returned.
    return mem[PC++];

  case m_ZPG: // Zero page mode.
    // The next byte is the address of first 256 bytes of memory. The address is 16 bits
    // (for total of 64 KB memory, but we only use the 8 bits to address the zero'th page.
    address = mem[PC++];
    return mem[address];

  case m_ZPX: // Zero Page X
    // The adress is the 8 bits of the next byte, added to the 8 bits of X
    // register. If the addition overflows, it's mod 0xFF.
    address = (mem[PC++] + X) % 0xFF;
    return mem[address];

  case m_ZPY: // Zero Page Y
    // The adress is the 8 bits of the next byte, added to the 8 bits of Y
    // register. If the addition overflows, it's mod 0xFF.
    address = (mem[PC++] + Y) % 0xFF;
    return mem[address];

  case m_ABS: // Absolute memory address.
    //In which case, the next two bytes (little endian)
    // are the memory address. Read the low byte first, then the high byte, and combine
    // them together.
    low_byte  = mem[PC++];
    high_byte = mem[PC++];
    address = combine_bytes(low_byte,high_byte);
    return mem[address];

  case m_ABX: // Absolute memory addres w.r.t. X.
    //Read the next two bytes, and add the value stored in X register. Return value in
    //this location.
    low_byte = mem[PC++];
    high_byte = mem[PC++];
    address = combine_bytes(low_byte,high_byte);
    return mem[address+X];

  case m_ABY: // Absolute memory address w.r.t. Y.
    low_byte = mem[PC++];
    high_byte = mem[PC++];
    address = combine_bytes(low_byte,high_byte);
    return mem[address+Y];

  case m_IND: // Indirection. Only used by JMP. Implement JMP bug.
    // Read the two bytes. Go the the this memory location, and read the next two
    // bytes. This gives an address. Return this value.
    low_byte  = mem[PC++];
    high_byte = mem[PC++];
    address = combine_bytes(low_byte,high_byte);

    // BUG in 6502 used in NES. If address is at page boundary (xxFF) , then the high byte
    // is read from (xx00) instead of ((xx+1)00).
    low_byte = mem[address];
    hb_address = (get_low_byte(address) == 0xFF)? combine_bytes(00,get_high_byte(address)) : (address+1);
    high_byte = mem[hb_address];

    address = combine_bytes(low_byte,high_byte);
    return mem[address];

  case m_INX: // Indirection X. Pre-Index Indirect.
    //Read the next byte, add it to X register. Go to this memory location, and read the
    //next two bytes. This gives an address. Return the value at this address.
    address = (mem[PC++] + X) % 0xFF;
    low_byte = mem[address];
    high_byte = mem[address+1];
    address = combine_bytes(low_byte,high_byte);
    return mem[address];

  case m_INY: // Post-Indexed Indirect Addressing
    // Read the next byte. Go to this memory location, and read the next two bytes. To
    // this value, add Y. This gives an address. Return the value at this address.
    address = mem[PC++];
    low_byte = mem[address];
    high_byte = mem[address+1];
    address = combine_bytes(low_byte,high_byte);
    return mem[address+Y];

  case m_ACCUM: // Accumulator addressing. Return the reference to accumulator.
    return A;

  default:
    return mem[PC++]; // Control should never reach here.
  };
}
