#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

std::string h(int x, int w = 2) {
  stringstream out;
  out << std::hex << std::setw(w) << std::setfill('0') << x;
  return out.str();
}

typedef unsigned char byte;

class CPU {

  byte A;
  byte X;
  byte Y;
  byte *mem;
  byte *PC;
  int N;

public:
  CPU():A(0),X(0),Y(0) {
    N = 512;
    mem = new byte(N);
    PC = mem;
    for (int i = 0; i < N; ++i)
      mem[i] = 0;
    mem[N - 1] = 0xff;
  }

  void loadProg(char *prog, int addr = 0) {
    string s(prog);
    byte *ptr = mem + addr;
    for (auto x : s)
      *(ptr++) = x;
  }

  void start() {
    while (!processOpCode())
        ++PC;
  }

  int processOpCode() {
    switch (*PC) {
    case 0x69:  // ADC
        A+=*(++PC); break;
    case 0x6c:  // JMP
        PC=mem+*(++PC) + *(++PC) * 0xff-1; break;
    case 0xa0: // LDY
      Y = *(++PC); break;
    case 0xa2: // LDX
      X = *(++PC); break;
    case 0xa9: // LDA
      A = *(++PC); break;
    case 0xaa:  //TAX
      X=A; break;
    case 0x8c: // STY
      mem[*(++PC) + *(++PC) * 0xff] = Y; break;
    case 0x8d: // STA
      mem[*(++PC) + *(++PC) * 0xff] = A; break;
    case 0x8e: // STX
      mem[*(++PC) + *(++PC) * 0xff] = X; break;
    case 0xe8:  // INX
      ++X; break;
    case 0x00:  // BRK
      return 1;
    default:
      break;
    }
    return 0;
  }

  void monitor() {
      cout << "*** MONITOR ***\n";
      cout << "A: $" << h(A) << " X: $" << h(X) << " Y: $" << h(Y) << " *PC: " << h(PC-mem);

    for (int addr = 0; addr < N; ++addr) {
      if (!(addr % 32))
        cout << endl << h(addr, 4) << ": ";
      if (mem[addr])
        cout << h(mem[addr]);
      else
        cout << "..";
      cout << " ";
    }
    cout << endl;
  }
};

int main(int ac, char *av[]) {
  cout << "6502 emu by tomgrad\n";
  CPU cpu;
  if (ac > 1)
    cpu.loadProg(av[1]);

  cpu.monitor();
  cpu.start();
  cpu.monitor();
  return 0;
}
