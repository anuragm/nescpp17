#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <cstddef>

// Defines varies typedef and like.
typedef uint_fast8_t  u8;
typedef uint_fast16_t u16;
typedef uint_fast32_t u32;
typedef uint_fast64_t u64;

//A structure to access individual bits.
template<std::size_t bitnum,typename T=u8>
struct bitReg{
  T data;

  constexpr const T mask() const { return 1u << bitnum; }
  void set()   { data = (data | mask()) ; }
  void unset() { data = (data & ~mask()); }
  T    get()   { return (data & mask())>>bitnum; }
};

#endif /* UTIL_HPP */
