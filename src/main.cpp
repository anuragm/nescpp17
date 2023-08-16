#include <iostream>
#include "cpu.hpp"
#include "cartridge.hpp"

int main(int argc, char** argv){

  if (argc!=2){
    std::printf("Need a file name. %s <filename>\n",argv[0]);
    return 0;
  }

  std::string fileName = argv[1];
  cartridge car(fileName);
  car.print_debug_info();

  return 0;
}
