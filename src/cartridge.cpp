#include "cartridge.hpp"
#include <fstream>

//------------------ Cartridge functions ---------------------//
cartridge::cartridge(std::string file){

  // Open and read the NES cartridge file.
  std::ifstream fh;
  fh.open(file, std::ifstream::binary | std::ifstream::in);
  fh.seekg(0,fh.beg); // Go to the start of the file.

  //Read in the required flags.
  fh.read(reinterpret_cast<char*>(header),16);

  num_prg_rom = header[4];
  num_chr_rom = header[5];
  num_prg_ram = header[8];

  mirroring_type   = static_cast<cartridge::mirror_type>(!!(header[6] &  1));
  prg_ram_present  = header[6] &  2;
  trainer_present  = header[6] &  4;
  four_screen_vram = header[6] & 16;

  mapper_number = (header[6]>>4) + (header[7] & 0xF0);
  prg_rom.init(num_prg_rom);
  chr_rom.init(num_chr_rom);

  u8* buffer;
  if (trainer_present){
    buffer = new u8 [512];
    fh.read(reinterpret_cast<char*>(buffer),512);
    std::copy(buffer,buffer+512,trainer);
    delete[] buffer;
  }

  // Read PRG ROM.
  buffer = new u8 [16*1024];
  for (std::size_t ii=0;ii<num_prg_rom;ii++){
    fh.read(reinterpret_cast<char*>(buffer),16*1024);
    prg_rom.set_bank(ii,buffer);
  }
  delete[] buffer;

  //Read CHR ROM.
  buffer = new u8 [8*1024];
  for (std::size_t ii=0;ii<num_prg_rom;ii++){
    fh.read(reinterpret_cast<char*>(buffer),16*1024);
    chr_rom.set_bank(ii,buffer);
  }
  delete[] buffer;

  // Discarding other part of NES ROM here.
}

void cartridge::print_debug_info(){
  std::printf("Size of header is %lu.\n",sizeof(header));
  std::printf("The header is : ");

  for(int ii=0; ii<16;ii++)
    std::printf("0x%x ",header[ii]);

  std::printf("\n");

  std::printf("The value of other variables is : \n");
  std::printf("Number of PRG ROMs is %u \n",num_prg_rom);
  std::printf("Number of CHR ROMs is %u \n",num_chr_rom);
  std::printf("Number of PRG RAM is %u. \n",num_prg_ram);
  std::printf("Is there a trainer? %s \n", trainer_present? "Yes" : "No" );
  std::printf("Mapper number is %u. \n",mapper_number);
  std::printf("Is there a four_screen_vram? %s \n", four_screen_vram? "Yes" : "No" );
  std::printf("Is there a PRG RAM? %s \n", prg_ram_present? "Yes" : "No" );
  std::printf("Mirroring type is %s.\n",
              mirroring_type ==
              cartridge::mirror_type::horiz ? "Horizontal" : "Vertical");
}
