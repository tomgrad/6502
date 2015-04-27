#ifndef M6502_H
#define M6502_H

#include <iomanip>
#include <sstream>
//#include <fstream>
//#include <bitset>
#include <vector>
#include <random>
//#include <iostream>

// using namespace std;

// converse decimal int to hex 00 format string
std::string h(unsigned short int x, unsigned int w = 2);

typedef unsigned char byte;
enum class addrType {
  Immediate,
  Absolute,
  ZeroPage,
  Indirect,
  AbsoluteX,
  AbsoluteY,
  ZeroPageX,
  ZeroPageY,
  IndexedIndirect,
  IndirectIndexed
};

enum class statusBit { N, V, B, D, I, Z, C };

class CPU {
protected:
  byte A = 0; // registers
  byte X = 0;
  byte Y = 0;
  byte *mem;      // first byte
  byte *PC;       // program counter
  byte SP = 0xff; // stack offset
  byte S = 0;     // processor status

  static const unsigned short int memsize = 0xffff; // memory size (bytes)

  // memory ranges
  int stack = 0x0100;
  int graph = 0x0200; // video device / terminal, ex. 32x32 pix/chars
  static const int prog = 0x0600;
  std::mt19937 randGen;
  std::uniform_int_distribution<byte> randByte;
  bool next = true;

public:
  CPU();

  void loadProg(char *fn, int addr = prog);
  void start();
  byte &addr(addrType type);
  void setStatus(statusBit bit, bool val);
  byte add(byte a, byte b);
  byte sub(byte a, byte b);
  void cmp(byte a, byte b);
  int processOpCode();
  bool debug = false;
  bool isRunning=true;
};

#endif
