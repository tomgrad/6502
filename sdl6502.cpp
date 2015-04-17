#include "sdl6502.h"
#include <iostream>

SDL6502::SDL6502() {
  SDL_Init(SDL_INIT_EVERYTHING);
  atexit(SDL_Quit);
  //    screen = SDL_SetVideoMode(width, width, 16,
  //    SDL_HWSURFACE|SDL_DOUBLEBUF);
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
  //  , dyn = true, draw = false;
  //  color = 0, r = 1;
}

SDL6502::~SDL6502() {}

void SDL6502::ProcessEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      done = true;
      break;
    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_ESCAPE)
        done = true;
      //      if (event.key.keysym.sym == SDLK_a)
      //        dyn = true;
      //      if (event.key.keysym.sym == SDLK_z)
      //        dyn = false;
      //            if (event.key.keysym.sym == SDLK_r)
      //                Reset();
      //            if (event.key.keysym.sym == SDLK_0)
      //                color=0;
      //            if (event.key.keysym.sym == SDLK_1)
      //                color=1;
      //            if (event.key.keysym.sym == SDLK_2)
      //                color=2;
      //            if (event.key.keysym.sym == SDLK_3)
      //                color=3;
      //            if (event.key.keysym.sym == SDLK_m)
      //                ++r;
      //            if (event.key.keysym.sym == SDLK_n)
      //                --r;
      //            if (event.key.keysym.sym == SDLK_s)
      //                setCells(color);
      //            if (event.key.keysym.sym == SDLK_g)
      //                setNeigh(0);
      //            if (event.key.keysym.sym == SDLK_h)
      //                setNeigh(2);

      break;
    }
      //    case SDL_MOUSEBUTTONDOWN: {
      //      draw = true;
      //    }

      //    case SDL_MOUSEMOTION: {
      //      if (draw) {
      //        for (int x = -r; x < r + 1; ++x)
      //          for (int y = -r; y < r + 1; ++y)

      //            setCell((event.motion.x / dx + x + (event.motion.y / dx + y)
      //            *
      //            L) %
      //                        (L * L),
      //                    color);
      //      }
      //      break;
      //    }
      //    case SDL_MOUSEBUTTONUP: {
      //      draw = false;
      //      break;
      //    }
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
    SDL_FillRect(screen, &cell, colormap[mem[graph + i]]);
  }
  // std::cout << "refresh" << std::endl;

  SDL_RenderClear(renderer);
  //   SDL_RenderCopy(renderer, screen, NULL, NULL);
  SDL_RenderPresent(renderer);
  SDL_UpdateWindowSurface(window);
  //   SDL_Flip(screen);
}
