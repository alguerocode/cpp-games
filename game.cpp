#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char** argv)
{
  // initialize the screen
  initscr();
  cbreak();
  noecho();
  int height, width, y_start, x_start;
  height = 10;
  width = 20;
  y_start = 5;
  x_start = 10;
  WINDOW* window = newwin(height, width, y_start, x_start);
  wattron(window, A_REVERSE);
  
  refresh();
  box(window, 0, 0);
  wrefresh(window);

  wprintw(window, "welcome there");
  wrefresh(window);
  getch();
  getch();
  
  endwin();
  return 0;
}