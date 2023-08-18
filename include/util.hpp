#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstddef>
#include <cstdint>

// Defines varies typedef and like.
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// A structure to access individual bits.
template <std::size_t bitnum, typename T = u8>
struct bitReg {
  T data;

  constexpr const T mask() const { return 1u << bitnum; }
  void set() { data = (data | mask()); }
  void clear() { data = (data & ~mask()); }
  T get() { return (data & mask()) >> bitnum; }
  void operator=(bool val) { val ? set() : clear(); }
};

// ----------------- Memory access ----------------------------
enum mem_mode  // Total 10 modes for NES where actual memory is read.
{
  m_IMM = 0,  // Immediate mode
  //--------Zero Page -------------
  m_ZPG,  //  Zero Page
  m_ZPX,  //  Zero Page X
  m_ZPY,  //  Zero Page Y
  //---------Absolute--------------
  m_ABS,  //  Absolute
  m_ABX,  //  Absolute X (Indexed Addressing X)
  m_ABY,  //  Absolute Y (Indexed Addressing Y)
  //---------Indirect--------------
  m_INX,  //  Pre-Indexed Indirect Addressing
  m_INY,  //  Post-Indexed Indirect Addressing
  // --------Special additional modes -----------
  m_ACCUM  // Accumulator
};

// ---------- Utility to get high/low byte -------------- //
inline u8 get_high_byte(u16 word) { return word >> 8; }
inline u8 get_low_byte(u16 word) { return word & 0xFF; }
inline u16 combine_bytes(u8 low_byte, u8 high_byte) {
  u16 temp = low_byte + (u16(high_byte) << 8);
  return temp;
}

#endif /* UTIL_HPP */
