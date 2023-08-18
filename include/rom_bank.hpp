#ifndef ROM_BANK_HPP
#define ROM_BANK_HPP

#include <memory>

#include "util.hpp"

// Declares different templated rom banks for character and program rom.

template <std::size_t size_in_kb>
class rom_bank {
  static const std::size_t bank_size = size_in_kb * 1024;
  bool allocated = false;
  std::size_t num_banks;

  u8 *data;
  std::size_t base_address;
  std::size_t current_bank;

 public:
  rom_bank(){};
  rom_bank(u8 _num_banks);
  void init(u8 _num_banks);

  ~rom_bank();

  void switch_bank(std::size_t bank_id);
  std::size_t get_current_bank();

  u8 &operator[](std::size_t location);
  void set_bank(std::size_t bank_id, u8 *_bank_data);
};

//-------------Declaration for templated functions. -------------------
template <std::size_t size_in_kb>
void rom_bank<size_in_kb>::init(u8 _num_banks) {
  num_banks = _num_banks;
  if (allocated) delete[] data;

  data = new u8[num_banks * bank_size];
  base_address = 0;
  allocated = true;
}

template <std::size_t size_in_kb>
rom_bank<size_in_kb>::rom_bank(u8 _num_banks) {
  init(_num_banks);
}

template <std::size_t size_in_kb>
rom_bank<size_in_kb>::~rom_bank() {
  if (allocated) delete[] data;
  allocated = false;
};

template <std::size_t size_in_kb>
void rom_bank<size_in_kb>::switch_bank(std::size_t bank_id) {
  current_bank = bank_id;
  base_address = bank_id * bank_size;
}

template <std::size_t size_in_kb>
u8 &rom_bank<size_in_kb>::operator[](std::size_t location) {
  return data[base_address + location];
}

template <std::size_t size_in_kb>
std::size_t rom_bank<size_in_kb>::get_current_bank() {
  return current_bank;
}

template <std::size_t size_in_kb>
void rom_bank<size_in_kb>::set_bank(std::size_t bank_id, u8 *_bank_data) {
  std::size_t base_addr = bank_id * bank_size;
  std::copy(_bank_data, _bank_data + bank_size, (data + base_addr));
}

#endif /* ROM_BANK_HPP */
