#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <cstddef>

// Defines varies typedef and like.
typedef uint_fast8_t  u8;
typedef uint_fast16_t u16;
typedef uint_fast32_t u32;
typedef uint_fast64_t u64;
typedef int_fast8_t  i8;
typedef int_fast16_t i16;
typedef int_fast32_t i32;
typedef int_fast64_t i64;

//A structure to access individual bits.
template<std::size_t bitnum,typename T=u8>
struct bitReg{
  T data;

  constexpr const T mask() const { return 1u << bitnum; }
  void set()   { data = (data | mask()) ; }
  void unset() { data = (data & ~mask()); }
  T    get()   { return (data & mask())>>bitnum; }
};

// ----------------- Memory access ----------------------------
enum mem_mode // Total 10 modes for NES where actual memory is read.
  {
   m_IMM=0, // Immediate mode
   //--------Zero Page -------------
   m_ZPG,  //  Zero Page
   m_ZPX,  //  Zero Page X
   m_ZPY,  //  Zero Page Y
   //---------Absolute--------------
   m_ABS,  //  Absolute
   m_ABX,  //  Absolute X (Indexed Addressing X)
   m_ABY,  //  Absolute Y (Indexed Addressing Y)
   //---------Indirect--------------
   m_IND,  //  Indirection.
   m_INX,  //  Pre-Indexed Indirect Addressing
   m_INY,  //  Post-Indexed Indirect Addressing
  };

#endif /* UTIL_HPP */
