#include "cpu.hpp"

// ------------- Data return ------------------------------------- //
u8 cpu::operand(mem_mode mode) {
  // When control reaches here, the PC is at one byte after the opcode. For
  // example, for immediate mode, the value of memory at the program counter is
  // returned, and PC is incremented by 1, etc.
  u16 address = get_address(mode);
  return mem[address];
}

// ------------------ Address return --------------------------- ////
u16 cpu::get_address(mem_mode mode) {
  // When control reaches here, the PC is at one byte after the opcode. For
  // example, for immediate mode, the value of memory at the program counter is
  // returned, and PC is incremented by 1, etc.
  u16 address;
  u8 low_byte, high_byte;

  switch (mode) {
    case m_IMM:  // Immediate mode.
      // The next byte in the program mode is returned.
      address = PC++;
      return address;

    case m_ZPG:  // Zero page mode.
      // The next byte is the address of first 256 bytes of memory. The address is
      // 16 bits (for total of 64 KB memory, but we only use the 8 bits to address
      // the zero'th page.
      address = mem[PC++];
      return address;

    case m_ZPX:  // Zero Page X
      // The adress is the 8 bits of the next byte, added to the 8 bits of X
      // register. If the addition overflows, it's mod 0xFF.
      address = (mem[PC++] + X) % 0xFF;
      return address;

    case m_ZPY:  // Zero Page Y
      // The adress is the 8 bits of the next byte, added to the 8 bits of Y
      // register. If the addition overflows, it's mod 0xFF.
      address = (mem[PC++] + Y) % 0xFF;
      return address;

    case m_ABS:  // Absolute memory address.
      // In which case, the next two bytes (little endian)
      // are the memory address. Read the low byte first, then the high byte, and
      // combine them together.
      low_byte = mem[PC++];
      high_byte = mem[PC++];
      address = combine_bytes(low_byte, high_byte);
      return address;

    case m_ABX:  // Absolute memory addres w.r.t. X.
      // Read the next two bytes, and add the value stored in X register. Return
      // value in this location.
      low_byte = mem[PC++];
      high_byte = mem[PC++];
      address = combine_bytes(low_byte, high_byte);
      return address + X;

    case m_ABY:  // Absolute memory address w.r.t. Y.
      low_byte = mem[PC++];
      high_byte = mem[PC++];
      address = combine_bytes(low_byte, high_byte);
      return address + Y;

    case m_INX:  // Indirection X. Pre-Index Indirect.
      // Read the next byte, add it to X register. Go to this memory location, and
      // read the next two bytes. This gives an address. Return the value at this
      // address.
      address = (mem[PC++] + X) % 0xFF;
      low_byte = mem[address];
      high_byte = mem[address + 1];
      address = combine_bytes(low_byte, high_byte);
      return address;

    case m_INY:  // Post-Indexed Indirect Addressing
      // Read the next byte. Go to this memory location, and read the next two
      // bytes. To this value, add Y. This gives an address. Return the value at
      // this address.
      address = mem[PC++];
      low_byte = mem[address];
      high_byte = mem[address + 1];
      address = combine_bytes(low_byte, high_byte);
      return address + Y;

    case m_ACCUM:  // Accumulator addressing. Return the reference to accumulator.
      return A;

    default:
      address = PC++;
      return address;  // Control should never reach here.
  };
}

// ------------------- Stack functions ----------------------- //
u8 cpu::pop_stack() {
  // The 6502 stack grows downwards. Thus, when items are popped, stack pointer
  // increases. The location of stack is 0x0100 to 0x01FF, thus 256 bytes.
  u8 data = mem[0x0100 + SP];
  SP++;
  return data;
}

void cpu::push_stack(u8 data) {
  mem.write_address(0x0100 + SP, data);
  SP--;
}
