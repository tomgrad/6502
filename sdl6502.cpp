#include "sdl6502.h"
#include <iostream>
#include <bitset>

SDL6502::SDL6502() {
  SDL_Init(SDL_INIT_EVERYTHING);
  atexit(SDL_Quit);
  window =
      SDL_CreateWindow("6502 emu by tomgrad", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, width, width, SDL_WINDOW_SHOWN);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
  screen = SDL_GetWindowSurface(window);
  colormap = {SDL_MapRGB(screen->format, 0, 0, 0),
              SDL_MapRGB(screen->format, 255, 255, 255),
              SDL_MapRGB(screen->format, 136, 0, 0),
              SDL_MapRGB(screen->format, 170, 255, 238),
              SDL_MapRGB(screen->format, 204, 68, 204),
              SDL_MapRGB(screen->format, 0, 204, 85),
              SDL_MapRGB(screen->format, 0, 0, 170),
              SDL_MapRGB(screen->format, 238, 238, 119),
              SDL_MapRGB(screen->format, 221, 136, 85),
              SDL_MapRGB(screen->format, 102, 68, 0),
              SDL_MapRGB(screen->format, 255, 119, 119),
              SDL_MapRGB(screen->format, 51, 51, 51),
              SDL_MapRGB(screen->format, 119, 119, 119),
              SDL_MapRGB(screen->format, 170, 255, 102),
              SDL_MapRGB(screen->format, 0, 136, 255),
              SDL_MapRGB(screen->format, 187, 187, 187)};

  done = false;
}

SDL6502::~SDL6502() {}

void SDL6502::sendAscii(SDL_Keysym k) {
  if (k.sym > 0xff)
    return;
  byte ch;
  if (k.mod == KMOD_LSHIFT || k.mod == KMOD_RSHIFT)
    ch = k.sym - 32;
  else
    ch = k.sym;
  mem[0xff] = ch;
  //    std::cout << int(ch) << std::endl;
}

void SDL6502::ProcessEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      done = true;
      break;
    case SDL_KEYDOWN: {
      auto &sym = event.key.keysym.sym;
      sendAscii(event.key.keysym);

      if (sym == SDLK_ESCAPE)
        done = true;
      if (sym == SDLK_d)

      {
        debug = !debug;
        next = !debug;
      }
      if (sym == SDLK_n)
        next = true;
      if (sym == SDLK_r)
        monitorReg();
      if (sym == SDLK_0)
        monitorMem(0, 0x0100);
      if (sym == SDLK_1)
        monitorMem(0x0100, 0x0200);
      if (sym == SDLK_2)
        monitorMem(0x0200, 0x0300);

      break;
    }
    }
  }
}

void SDL6502::Refresh() {
  SDL_Rect cell;
  cell.w = dx;
  cell.h = dx;

  for (int i = 0; i < N; ++i) {
    cell.x = i % L * cell.w;
    cell.y = i / L * cell.h;
    SDL_FillRect(screen, &cell, colormap[*(mem + graph + i) % 16]);
  }

  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  SDL_UpdateWindowSurface(window);
}

void SDL6502::monitorReg() {
  std::cout << "A: $" << h(A) << " X: $" << h(X) << " Y: $" << h(Y) << " *PC: $"
            << h(PC - mem, 4) << " *SP: $" << h(SP) << " NV-BDIZC: "
            //              << N << V << "-" << B << D << I << Z << C;
            << std::bitset<8>(S) << std::endl;
}

void SDL6502::monitorMem(unsigned short la, unsigned short ha) {

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
