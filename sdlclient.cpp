#include "sdl6502.h"
#include <thread>
int main(int ac, char *av[]) {
  SDL6502 emu;
  if (ac > 1) {
    emu.loadProg(av[1]);
  }

  std::thread th([&emu]() { emu.start(); });

  while (emu.IsOpened()) {
    emu.ProcessEvents();
    emu.Refresh();
  }
  emu.setStatus(statusBit::B, 1);
  th.join();

  return 0;
}
