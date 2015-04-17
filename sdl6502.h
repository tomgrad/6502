#ifndef SDL6502_H
#define SDL6502_H

#include <SDL2/SDL.h>
#include "6502.h"

#include <vector>

class SDL6502 : public CPU {
public:
  SDL6502();
  ~SDL6502();
  void Refresh();
  void ProcessEvents();
  bool IsOpened() { return !done; };

protected:
  SDL_Surface *screen;
  SDL_Window *window;
  SDL_Renderer *renderer;
  int width = 320;
  int L = 32;
  int N = 1024;
  std::vector<Uint32> colormap;
  bool done;
  //  , dyn, draw;
  //  int color, r,
  int dx = 10;
};

#endif
