#include <ncurses.h>
#include "6502.h"

class ncurses6502 : public CPU {
protected:
  WINDOW *win;

public:
  ncurses6502() {
    initscr();
    noecho();
    curs_set(FALSE);
    win = newwin(32, 64, 5, 5);
    //        wmove(win,0,5);
    start_color();
    for (int i = 0; i < 8; ++i)
      init_pair(i + 1, i, COLOR_BLACK);
    //        wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    //        wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    //        waddch(win, '#' | COLOR_YELLOW);
    for (int i = 0; i < 8; ++i)
      wprintw(win, "hello ", COLOR_PAIR(i + 1));
    //        wbkgd(win, COLOR_PAIR(4));

    wrefresh(win);
    //        refresh();
  }
  ~ncurses6502() {
    delwin(win);
    endwin();
  }

  void termDisplay() {
    std::vector<int> cmap16 = {40, 107, 41,  46,  45, 42,  44,  103,
                               43, 30,  101, 100, 47, 102, 104, 47};
    //      std::cout << "\033c";
    //      std::cout << "\e[49m|";
    //      for (int i = 0; i < 32; ++i)
    //        std::cout << "==";
    //      std::cout << "\e[49m|\n|";
    //      for (auto i = graph; i < graph + 1024; ++i) {
    //        std::cout << "\e[" << cmap16[mem[i]] << "m";
    //        std::cout << "  ";
    //        if (!((i + 1) % 32))
    //          std::cout << "\e[39m\e[49m|\n|";
    //      }
    //      for (int i = 0; i < 32; ++i)
    //        std::cout << "==";
    //      std::cout << "|\n";
    wgetch(win);
  }
};

int main() {
  ncurses6502 emu;
  emu.termDisplay();

  // wrefresh(w);
  // getch();
  // delwin(w);
  // printw("hello");
  // refresh();
  // getch();
  // endwin();

  return 0;
}
