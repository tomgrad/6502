#include "6502.h"

#include <iomanip>
#include <sstream>
#include <fstream>
#include <bitset>
#include <vector>
#include <iostream>

// using namespace std;

// converse decimal int to hex 00 format string
std::string h(unsigned short int x, unsigned int w) {
  std::stringstream out;
  out << std::hex << std::setw(w) << std::setfill('0') << x;
  return out.str();
}

CPU::CPU() {
  mem = new byte[memsize];
  PC = mem + prog;
  for (int i = 0; i < memsize; ++i)
    mem[i] = 0;
  mem[memsize - 1] = 0xff;
}

void CPU::loadProg(char *fn, int addr) {
  std::ifstream file(fn, std::ios::binary);

  char *ptr = (char *)(mem + addr);
  file.read(ptr, 64000);
  file.close();
}

void CPU::start() {
  while (isRunning) {
    if (next) {
      mem[0xfe] = randByte(randGen);
      processOpCode();
      ++PC;
      next = !debug;
    }
  }
}

byte &CPU::addr(addrType type) {
  byte tmp;
  switch (type) {
  case addrType::Immediate:
    return *(++PC);
  case addrType::Absolute:
    return mem[*(++PC) + *(++PC) * 0x0100];
  case addrType::AbsoluteX:
    return mem[*(++PC) + 0x100 * *(++PC) + X];
  case addrType::AbsoluteY:
    return mem[*(++PC) + 0x100 * *(++PC) + Y];
  case addrType::ZeroPage:
    return mem[*(++PC)];
  case addrType::ZeroPageX:
    return mem[*(++PC) + X];
  case addrType::ZeroPageY:
    return mem[*(++PC) + Y];
  case addrType::Indirect:
    return mem[mem[*(PC + 1)] + 0x0100 * mem[*(PC++ + 1) + 1]];
  case addrType::IndexedIndirect:
    return mem[mem[*(PC + 1) + X] + mem[*(++PC) + X + 1] * 0x0100];
  case addrType::IndirectIndexed:
    return mem[mem[*(PC + 1)] + 0x0100 * mem[*(++PC) + 1] + Y];
  }
}

void CPU::setStatus(statusBit bit, bool val) {
  switch (bit) {
  case statusBit::C:
    S = val ? S | 1 : S & ~1;
    break;
  case statusBit::Z:
    S = val ? S | 1 << 1 : S & ~(1 << 1);
    break;
  case statusBit::I:
    S = val ? S | 1 << 2 : S & ~(1 << 2);
    break;
  case statusBit::D:
    S = val ? S | 1 << 3 : S & ~(1 << 3);
    break;
  case statusBit::B:
    S = val ? S | 1 << 4 : S & ~(1 << 4);
    break;
  case statusBit::V:
    S = val ? S | 1 << 6 : S & ~(1 << 6);
    break;
  case statusBit::N:
    S = val ? S | 1 << 7 : S & ~(1 << 7);
    break;
  }
}

byte CPU::add(byte a, byte b) {
  byte result = a + b + (S & 1);
  setStatus(statusBit::N, result & 0x80);
  setStatus(statusBit::C, result < a || result < b);
  setStatus(statusBit::Z, !result);
  setStatus(statusBit::V, (a ^ result) & (b ^ result) & 0x80);
  return result;
}

byte CPU::sub(byte a, byte b) {
  byte result = a - b - 1 + (S & 1);
  setStatus(statusBit::N, result & 0x80);
  setStatus(statusBit::C,
            ~(result > a || result > b)); // not sure about this one
  setStatus(statusBit::Z, !result);
  setStatus(statusBit::V,
            (a ^ result) & (b ^ result) & 0x80); // ...and that one
  return result;
}

void CPU::cmp(byte a, byte b) {
  setStatus(statusBit::Z, a == b);
  setStatus(statusBit::C, a >= b);
  setStatus(statusBit::N, (a - b) & 0x80);
}

int CPU::processOpCode() {
  //    std::cout << h(*PC, 2) << " ";
  unsigned short tmp;
  switch (*PC) {
  // LDA - Load the Accumulator
  case 0xa9:
    A = addr(addrType::Immediate);
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0xa1:
    A = addr(addrType::IndexedIndirect);
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0xb1:
    A = addr(addrType::IndirectIndexed);
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0xa5:
    A = addr(addrType::ZeroPage);
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0xb5:
    A = addr(addrType::ZeroPageX);
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0xad:
    A = addr(addrType::Absolute);
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0xbd:
    A = addr(addrType::AbsoluteX);
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0xb9:
    A = addr(addrType::AbsoluteY);
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;

  // STA - Store the Accumulator
  case 0x85:
    addr(addrType::ZeroPage) = A;
    break;
  case 0x95:
    addr(addrType::ZeroPageX) = A;
    break;
  case 0x9d:
    addr(addrType::AbsoluteX) = A;
    break;
  case 0x99:
    addr(addrType::AbsoluteY) = A;
    break;
  case 0x8d:
    addr(addrType::Absolute) = A;
    break;
  case 0x81:
    addr(addrType::IndexedIndirect) = A;
    break;
  case 0x91:
    addr(addrType::IndirectIndexed) = A;
    break;

  // LDX - Load the X register
  case 0xa2:
    X = addr(addrType::Immediate);
    setStatus(statusBit::Z, !X);
    setStatus(statusBit::N, X & 0x80);
    break;
  case 0xa6:
    X = addr(addrType::ZeroPage);
    setStatus(statusBit::Z, !X);
    setStatus(statusBit::N, X & 0x80);
    break;
  case 0xae:
    X = addr(addrType::Absolute);
    setStatus(statusBit::Z, !X);
    setStatus(statusBit::N, X & 0x80);
    break;
  case 0xbe:
    X = addr(addrType::AbsoluteY);
    setStatus(statusBit::Z, !X);
    setStatus(statusBit::N, X & 0x80);
    break;

  // STX - Store the X register
  case 0x86:
    addr(addrType::ZeroPage) = X;
    break;
  case 0x96:
    addr(addrType::ZeroPageY) = X;
    break;
  case 0x8e:
    addr(addrType::Absolute) = X;
    break;

  // LDY - Load the Y register
  case 0xa0:
    Y = addr(addrType::Immediate);
    setStatus(statusBit::Z, !Y);
    setStatus(statusBit::N, Y & 0x80);
    break;
  case 0xa4:
    Y = addr(addrType::ZeroPage);
    setStatus(statusBit::Z, !Y);
    setStatus(statusBit::N, Y & 0x80);
    break;
  case 0xb4:
    Y = addr(addrType::ZeroPageX);
    setStatus(statusBit::Z, !Y);
    setStatus(statusBit::N, Y & 0x80);
    break;
  case 0xac:
    Y = addr(addrType::Absolute);
    setStatus(statusBit::Z, !Y);
    setStatus(statusBit::N, Y & 0x80);
    break;
  case 0xbc:
    Y = addr(addrType::AbsoluteX);
    setStatus(statusBit::Z, !Y);
    setStatus(statusBit::N, Y & 0x80);
    break;

  // STY - Store the Y register
  case 0x84:
    addr(addrType::ZeroPage) = Y;
    break;
  case 0x94:
    addr(addrType::ZeroPageX) = Y;
    break;
  case 0x8c:
    addr(addrType::Absolute) = Y;
    break;

  // ADC - Add with Carry
  case 0x69:
    A = add(A, addr(addrType::Immediate));
    break;
  case 0x65:
    A = add(A, addr(addrType::ZeroPage));
    break;
  case 0x75:
    A = add(A, addr(addrType::ZeroPageX));
    break;
  case 0x6d:
    A = add(A, addr(addrType::Absolute));
    break;
  case 0x7d:
    A = add(A, addr(addrType::AbsoluteX));
    break;
  case 0x79:
    A = add(A, addr(addrType::AbsoluteY));
    break;
  case 0x61:
    A = add(A, addr(addrType::IndexedIndirect));
    break;
  case 0x71:
    A = add(A, addr(addrType::IndirectIndexed));
    break;

  // SBC - Subtract with Carry
  case 0xe9:
    A = sub(A, addr(addrType::Immediate));
    break;
  case 0xe5:
    A = sub(A, addr(addrType::ZeroPage));
    break;
  case 0xf5:
    A = sub(A, addr(addrType::ZeroPageX));
    break;
  case 0xed:
    A = sub(A, addr(addrType::Absolute));
    break;
  case 0xfd:
    A = sub(A, addr(addrType::AbsoluteX));
    break;
  case 0xf9:
    A = sub(A, addr(addrType::AbsoluteY));
    break;
  case 0xe1:
    A = sub(A, addr(addrType::IndexedIndirect));
    break;
  case 0xf1:
    A = sub(A, addr(addrType::IndirectIndexed));
    break;

  // Status Instructions
  case 0x18: // CLC
    setStatus(statusBit::C, 0);
    break;
  case 0x38: // SEC
    setStatus(statusBit::C, 1);
    break;
  case 0x58: // CLI
    setStatus(statusBit::I, 0);
    break;
  case 0x78: // SEI
    setStatus(statusBit::I, 1);
    break;
  case 0xd8: // CLD
    setStatus(statusBit::D, 0);
    break;
  case 0xf8: // SED
    setStatus(statusBit::D, 1);
    break;
  case 0xb8: // CLV
    setStatus(statusBit::V, 0);
    break;

  // JMP
  case 0x4c: // absolute
    PC = mem + *(++PC) + *(++PC) * 0x0100 - 1;
    break;
  case 0x6c: // indirect
    PC = mem + mem[*(++PC) + *(++PC) * 0x0100] - 1;
    break;

  // Register instructions

  case 0xaa: // TAX
    X = A;
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0x8a: // TXA
    A = X;
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0xa8: // TAY
    Y = A;
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0x98: // TYA
    A = Y;
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;
  case 0xe8: // INX
    ++X;
    setStatus(statusBit::Z, !X);
    setStatus(statusBit::N, X & 0x80);
    break;
  case 0xc8: // INY
    ++Y;
    setStatus(statusBit::Z, !Y);
    setStatus(statusBit::N, Y & 0x80);
    break;
  case 0xca: // DEX
    --X;
    setStatus(statusBit::Z, !X);
    setStatus(statusBit::N, X & 0x80);
    break;
  case 0x88: // DEY
    --Y;
    setStatus(statusBit::Z, !Y);
    setStatus(statusBit::N, Y & 0x80);
    break;

  // Logical Instructions
  // AND
  case 0x29:
    A &= addr(addrType::Immediate);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x25:
    A &= addr(addrType::ZeroPage);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x35:
    A &= addr(addrType::ZeroPageX);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x2d:
    A &= addr(addrType::Absolute);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x3d:
    A &= addr(addrType::AbsoluteX);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x39:
    A &= addr(addrType::AbsoluteY);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x21:
    A &= addr(addrType::IndexedIndirect);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x31:
    A &= addr(addrType::IndirectIndexed);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;

  // ORA
  case 0x09:
    A |= addr(addrType::Immediate);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x05:
    A |= addr(addrType::ZeroPage);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x15:
    A |= addr(addrType::ZeroPageX);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x0d:
    A |= addr(addrType::Absolute);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x1d:
    A |= addr(addrType::AbsoluteX);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x19:
    A |= addr(addrType::AbsoluteY);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x01:
    A |= addr(addrType::IndexedIndirect);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x11:
    A |= addr(addrType::IndirectIndexed);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;

  // EOR
  case 0x49:
    A ^= addr(addrType::Immediate);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x45:
    A ^= addr(addrType::ZeroPage);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x55:
    A ^= addr(addrType::ZeroPageX);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x4d:
    A ^= addr(addrType::Absolute);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x5d:
    A ^= addr(addrType::AbsoluteX);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x59:
    A ^= addr(addrType::AbsoluteY);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x41:
    A ^= addr(addrType::IndexedIndirect);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;
  case 0x51:
    A ^= addr(addrType::IndirectIndexed);
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);
    break;

  // CMP
  case 0xc9:
    cmp(A, addr(addrType::Immediate));
    break;
  case 0xc5:
    cmp(A, addr(addrType::ZeroPage));
    break;
  case 0xd5:
    cmp(A, addr(addrType::ZeroPageX));
    break;
  case 0xcd:
    cmp(A, addr(addrType::Absolute));
    break;
  case 0xdd:
    cmp(A, addr(addrType::AbsoluteX));
    break;
  case 0xd9:
    cmp(A, addr(addrType::AbsoluteY));
    break;
  case 0xc1:
    cmp(A, addr(addrType::IndexedIndirect));
    break;
  case 0xd1:
    cmp(A, addr(addrType::IndirectIndexed));
    break;

  // CPX
  case 0xe0:
    cmp(X, addr(addrType::Immediate));
    break;
  case 0xe4:
    cmp(X, addr(addrType::ZeroPage));
    break;
  case 0xec:
    cmp(X, addr(addrType::Absolute));
    break;

  // CPY
  case 0xc0:
    cmp(Y, addr(addrType::Immediate));
    break;
  case 0xc4:
    cmp(Y, addr(addrType::ZeroPage));
    break;
  case 0xcc:
    cmp(Y, addr(addrType::Absolute));
    break;

  // Branch
  case 0xd0: // BNE
    if (!(S & 2))
      PC += *(PC + 1) < 128 ? *(PC + 1) + 1 : *(PC + 1) - 0xff;
    else
      ++PC;
    //      std::cout << " bne " << h(PC - mem, 4) << std::endl;
    break;

  case 0xf0: // BEQ
    if (S & 2)
      PC += *(PC + 1) < 128 ? *(PC + 1) + 1 : *(PC + 1) - 0xff;
    else
      ++PC;
    break;

  case 0x10: // BPL - Branch if Positive
    if (!(S & 0x80))
      PC += *(PC + 1) < 128 ? *(PC + 1) + 1 : *(PC + 1) - 0xff;
    else
      ++PC;
    break;

  // INC - Increment memory
  case 0xe6:
    tmp = ++addr(addrType::ZeroPage);
    setStatus(statusBit::Z, !tmp);
    setStatus(statusBit::N, tmp & 0x80);

    break;
  case 0xf6:
    tmp = ++addr(addrType::ZeroPageX);
    setStatus(statusBit::Z, !tmp);
    setStatus(statusBit::N, tmp & 0x80);
    break;
  case 0xee:
    tmp = ++addr(addrType::Absolute);
    setStatus(statusBit::Z, !tmp);
    setStatus(statusBit::N, tmp & 0x80);
    break;
  case 0xfe:
    tmp = ++addr(addrType::AbsoluteX);
    setStatus(statusBit::Z, !tmp);
    setStatus(statusBit::N, tmp & 0x80);
    break;

  // DEC - Decrement memory
  case 0xc6:
    tmp = --addr(addrType::ZeroPage);
    setStatus(statusBit::Z, !tmp);
    setStatus(statusBit::N, tmp & 0x80);

    break;
  case 0xd6:
    tmp = --addr(addrType::ZeroPageX);
    setStatus(statusBit::Z, !tmp);
    setStatus(statusBit::N, tmp & 0x80);
    break;
  case 0xce:
    tmp = --addr(addrType::Absolute);
    setStatus(statusBit::Z, !tmp);
    setStatus(statusBit::N, tmp & 0x80);
    break;
  case 0xde:
    tmp = --addr(addrType::AbsoluteX);
    setStatus(statusBit::Z, !tmp);
    setStatus(statusBit::N, tmp & 0x80);
    break;

  // ASL - Arithmetic Shift Left
  case 0x0a:
    setStatus(statusBit::C, A & 0x80);
    A <<= 1;
    setStatus(statusBit::Z, !A);
    setStatus(statusBit::N, A & 0x80);
    break;

  // ROR - Rotate Right
  case 0x66: {
    byte oldC = S & 1;
    byte &tmp = addr(addrType::ZeroPage);
    setStatus(statusBit::C, tmp & 1);
    tmp >>= 1;
    tmp |= oldC << 7;

  } break;

  // ROL - Rotate Left

  case 0x2a: {
    byte oldC = S & 1;
    setStatus(statusBit::C, A & 0x80);
    A <<= 1;
    A |= oldC;
  }
    setStatus(statusBit::N, A & 0x80);
    setStatus(statusBit::Z, !A);

    break;

  case 0x26: {
    byte oldC = S & 1;
    byte &tmp = addr(addrType::ZeroPage);
    setStatus(statusBit::C, tmp & 0x80);
    tmp <<= 1;
    tmp |= oldC;
    setStatus(statusBit::N, tmp & 0x80);

  } break;

  // JSR - Jump to Subroutine
  case 0x20:
    //      std::cout << h(PC - mem, 4) << std::endl;
    mem[stack + SP--] = (PC - mem + 2) >> 8;
    mem[stack + SP--] = (PC - mem + 2) & 0x00ff;

    PC = mem + *(++PC) + *(++PC) * 0x0100 - 1;
    break;

  // RTS - Return from Subroutine
  case 0x60:
    PC = mem + mem[stack + SP + 1];
    mem[stack + ++SP] = 0;
    PC += 0x0100 * mem[stack + SP + 1];
    mem[stack + ++SP] = 0;
    //      std::cout << h(PC - mem, 4) << std::endl;
    break;

  case 0x48: // PHA
    mem[stack + SP--] = A;
    break;
  case 0x68: // PLA
    A = mem[stack + SP + 1];
    mem[stack + SP++] = 0;
    break;
  case 0xea: // NOP - no operation
    break;
  case 0x00: // BRK
isRunning=false;
      return 1;

  case 0x02:
  case 0x03:
  case 0x04:
  case 0x07:

  case 0x42:
  case 0x43:
  case 0x44:

  case 0x62:
  case 0x63:
  case 0x64:

  case 0x80:
  case 0x82:

  case 0xe7:

    std::cout << std::endl << h(*PC) << " invalid code\n";
    break;

  default:
    std::cout << std::endl << h(*PC) << " not implemented\n";
    exit(0);
    break;
  }
  return 0;
}
