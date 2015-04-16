#include "6502.h"
#include <thread>

class txt6502 : public CPU {
public:
  void monitorReg() {
    std::cout << "A: $" << h(A) << " X: $" << h(X) << " Y: $" << h(Y)
              << " *PC: $" << h(PC - mem, 4) << " *SP: $" << h(SP)
              << " NV-BDIZC: "
              //              << N << V << "-" << B << D << I << Z << C;
              << std::bitset<8>(S);
  }

  void monitorMem(unsigned short la = 0, unsigned short ha = memsize) {

    std::cout << "\n      00          04          08          0C       0F ";
    std::cout << "10          14          18          1C       1F";
    for (int addr = la; addr < ha; ++addr) {
      if (!(addr % 32))
        std::cout << std::endl << h(addr, 4) << ": ";
      if (mem[addr])
        std::cout << h(mem[addr]);
      else
        std::cout << "..";
      std::cout << " ";
    }
    std::cout << std::endl;
  }

  void termDisplay() {
    std::vector<int> cmap16 = {40, 107, 41,  46,  45, 42,  44,  103,
                               43, 30,  101, 100, 47, 102, 104, 47};
    std::cout << "\033c";
    std::cout << "\e[49m|";
    for (int i = 0; i < 32; ++i)
      std::cout << "==";
    std::cout << "\e[49m|\n|";
    for (auto i = graph; i < graph + 1024; ++i) {
      std::cout << "\e[" << cmap16[mem[i]] << "m";
      std::cout << "  ";
      if (!((i + 1) % 32))
        std::cout << "\e[39m\e[49m|\n|";
    }
    for (int i = 0; i < 32; ++i)
      std::cout << "==";
    std::cout << "|\n";
  }

  void loop() {
    std::thread th([this]() { start(); });
    std::thread thView([this]() {
      do {
        termDisplay();
      } while (!(S & (1 << 5)));
    });

    th.join();
    thView.join();
  }
};

int main(int ac, char *av[]) {
  std::cout << "6502 emu by tomgrad\n";
  txt6502 cpu;
  if (ac > 1) {
    cpu.loadProg(av[1]);
  }

  cpu.loop();

  //  cpu.start();
  //  cpu.monitorReg();

  //  cpu.monitorMem(0x00, 0x100);
  //  cpu.monitorMem(0x200, 0x300);
  //  cpu.monitorMem(0x600, 0x700);

  //  cpu.termDisplay();
  return 0;
}
