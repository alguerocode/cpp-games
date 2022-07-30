#include <ncursesw/ncurses.h>
#include <iostream>
#include <locale.h>

#define MAP_WIDTH 45
#define MAP_HEIGHT 20

struct Player
{
    int y, x;
    char body;
};

int main()
{
    Player player = {5, 10, '@'};
    initscr();
    setlocale(LC_ALL, "");
    cbreak();
    noecho();

    refresh();
    WINDOW *GAME_WIN = newwin(MAP_HEIGHT, MAP_WIDTH, 3, 6);
    keypad(GAME_WIN, TRUE);
    box(GAME_WIN, 0, 0);
    mvwprintw(GAME_WIN, player.y, player.x, "%c", player.body);
    wrefresh(GAME_WIN);

    // game engine
    while (1)
    {
        // game logic
        int ch = wgetch(GAME_WIN);
        mvwprintw(GAME_WIN, player.y, player.x, "%c", '.');
        switch (ch)
        {
        case KEY_UP:
            if (player.y > 1)
                player.y--;
            break;
        case KEY_DOWN:
            if (player.y < MAP_HEIGHT - 2)
                player.y++;
            break;
        case KEY_RIGHT:
            if (player.x < MAP_WIDTH - 2)
                player.x++;
            break;
        case KEY_LEFT:
            if (player.x > 1)
                player.x--;
            break;
        case (int) 'x':
            endwin();
            exit(0);
        }

        mvwprintw(GAME_WIN, player.y, player.x, "%c", player.body);
        //
    }
    endwin();
    return 0;
}