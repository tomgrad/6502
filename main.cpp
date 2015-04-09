#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
// using namespace std;

// converse decimal int to hex 00 format string
std::string h(unsigned short int x, unsigned int w = 2) {
  std::stringstream out;
  out << std::hex << std::setw(w) << std::setfill('0') << x;
  return out.str();
}

typedef unsigned char byte;

class CPU {
  byte A; // registers
  byte X;
  byte Y;
  byte *mem; // first byte
  byte *PC;  // program counter
  byte SP;   // stack pointer

  // processor status
  //  bool N; // negative
  bool V; // overflow
  bool B; // BRK command
  bool D; // decimal mode
  bool I; // IRQ disable
  bool Z; // zero
  bool C; // carry

  static const unsigned short int memsize = 0x1000; // memory size (bytes)

  // memory ranges
  int stack = 0x0100;
  int graph = 0x0200; // video device / terminal, ex. 32x32 pix/chars
  static const int prog = 0x0600;

public:
  CPU() : A(0), X(0), Y(0) {
    mem = new byte[memsize];
    PC = mem + prog;
    SP = 0xff;
    for (int i = 0; i < memsize; ++i)
      mem[i] = 0;
    mem[memsize - 1] = 0xff;
  }

  void loadProg(char *fn, int addr = prog) {
    std::ifstream file(fn, std::ios::binary);

    char *ptr = (char *)(mem + addr);
    //      while (file.good())
    //      file >> *(ptr++);
    file.read(ptr, 255);
    file.close();
  }

  void start() {
    while (!processOpCode()) {
      //        std::cout<<h(PC-mem)<<" ; ";
      ++PC;
    }
  }

  int processOpCode() {
    switch (*PC) {
    // LDA
    case 0xa9: // immediate
      A = *(++PC);
      break;
    case 0xa1: // (indirect,X)
      A = mem[mem[*(PC + 1) + X] + 0x0100 * mem[*(PC + 1) + X + 1]];
      ++PC;
      break;
    case 0xb1: // (indirect),Y
      A = mem[mem[*(PC + 1)] + 0x0100 * mem[*(PC + 1) + 1] + Y];
      ++PC;
      break;

    case 0x69: // ADC
      A += *(++PC);
      break;
    case 0x6c: // JMP (little endian)
      PC = mem + *(++PC) + *(++PC) * 0x0100 - 1;
      break;
    case 0xa0: // LDY
      Y = *(++PC);
      break;
    case 0xa2: // LDX
      X = *(++PC);
      break;
    case 0xaa: // TAX
      X = A;
      break;

    case 0x8a: // TXA
      A = X;
      break;

    case 0x85: // STA 0 page
      mem[*(++PC)] = A;
      break;
    case 0x99: // STA absolute,Y
      mem[*(++PC) + 0x100 * *(++PC) + Y] = A;
      break;

    case 0x8c: // STY
      mem[*(++PC) + *(++PC) * 0x0100] = Y;
      break;
    case 0x8d: // STA
      mem[*(++PC) + *(++PC) * 0x0100] = A;
      break;
    case 0x8e: // STX
      mem[*(++PC) + *(++PC) * 0x0100] = X;
      break;
    case 0xc9: // CMP
      Z = A == *(++PC);
      break;
    case 0xca: // DEX
      --X;
      break;
    case 0xd0: // BNE
      if (!Z)
        PC += *(PC + 1) < 127 ? *(PC + 1) : *(PC + 1) - 0xff;
      else
        ++PC;
      break;
    case 0xe0: // CPX
      Z = X == *(++PC);
      break;

    case 0xc0: // CPY
      Z = Y == *(++PC);
      break;

    case 0xe8: // INX
      ++X;
      break;
    case 0xc8: // INY
      ++Y;
      break;
    case 0x48: // PHA
      mem[stack + SP] = A;
      --SP;
      break;
    case 0x68: // PLA
      A = mem[stack + SP + 1];
      mem[stack + SP] = 0;
      ++SP;
      break;
    case 0x00: // BRK
      return 1;
    default:
      std::cout << std::endl << h(*PC) << " not implemented\n";
      exit(0);
      break;
    }
    return 0;
  }

  void monitorReg() {
    std::cout << "A: $" << h(A) << " X: $" << h(X) << " Y: $" << h(Y)
              << " *PC: $" << h(PC - mem, 4) << " *SP: $" << h(SP)
              << " Z:" << Z;
  }

  void monitorMem(unsigned short la = 0, unsigned short ha = memsize) {

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
};

int main(int ac, char *av[]) {
  std::cout << "6502 emu by tomgrad\n";
  CPU cpu;
  if (ac > 1) {
    cpu.loadProg(av[1]);
  }

  cpu.monitorReg();
  cpu.start();
  cpu.monitorMem(0x00, 0x200);
  cpu.monitorMem(0x200, 0x300);
  return 0;
}
