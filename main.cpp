#include "6502.h"

#include <iostream>

int main(int ac, char *av[]) {
  std::cout << "6502 emu by tomgrad\n";
  CPU cpu;
  if (ac > 1) {
    cpu.loadProg(av[1]);
  }

  cpu.start();
  //  cpu.monitorReg();

  //  cpu.monitorMem(0x00, 0x100);
  //  cpu.monitorMem(0x200, 0x300);
  //  cpu.monitorMem(0x600, 0x700);

  //  cpu.termDisplay();
  return 0;
}
