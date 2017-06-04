#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP
// Here, we implement the ROM loading functions.

#include "util.hpp"
#include "rom_bank.hpp"
#include <vector>
#include <memory>

class cartridge {
  u8 trainer[512];
  u8 header[16];
  rom_bank<16> prg_rom;
  rom_bank<8>  chr_rom;

  enum mirror_type : bool { horiz = false, vert = true};
  mirror_type mirroring_type;

  bool trainer_present;
  bool prg_ram_present;
  bool four_screen_vram;

  u8   num_prg_rom; // In units of 16 kiB
  u8   num_chr_rom; // In units of 8 KiB
  u8   num_prg_ram; // In units of 8 KiB
  u8   mapper_number;

public:
  cartridge() = delete ;
  cartridge(std::string file); // Load the file into RAM.
  void print_debug_info(); // Print the header.
  bool check_rom(); // Checks if the ROM has correct header.
};

#endif /* CARTRIDGE_HPP */
