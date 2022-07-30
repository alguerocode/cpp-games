#include <ncursesw/ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

class Player
{
public:
    int x, y;
    wchar_t *body;
    int game_width;
    int game_height;

public:
    Player(int yinput, int xinput, int height, int width)
    {
        init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
        body = (wchar_t *)L"▲";
        x = xinput;
        y = yinput;
        game_height = height;
        game_width = width;
    }
    void clear(WINDOW *win)
    {
            wattron(win, COLOR_PAIR(3));

        mvwprintw(win, y, x, " ");
    wattroff(win, COLOR_PAIR(3));

    }
    void draw(WINDOW *win)
    {
            wattron(win, COLOR_PAIR(1));
        mvwaddwstr(win, y, x, body);
            wattroff(win, COLOR_PAIR(1));
    }
    void mvup()
    {
        body = (wchar_t *)L"▲";
        if (y > 1)
            y--;
    };
    void mvdown()
    {
        body = (wchar_t *)L"▼";
        if (y < game_height - 2)
            y++;
    }
    void mvleft()
    {
        body = (wchar_t *)L"◄";
        if (x > 1)
            x--;
    };
    void mvright()
    {
        body = (wchar_t *)L"►";
        if (x < game_width - 2)
            x++;
    };
};

class Game
{
private:
    int game_width;
    int game_height;
    int game_current_score;
    int max_score;
    wchar_t *score_body;
    int current_score_x;
    WINDOW *scorewin;

public:
    WINDOW *gamewin;
    Player *player = nullptr;

public:
    Game(int height, int width, int start_y, int start_x, const wchar_t *s_body, int max)
    {
        initscr();
        noecho();
        cbreak();
        refresh();

        game_width = width;
        game_height = height - 1;
        max_score = max;
        game_current_score = 0;
        score_body = (wchar_t *)s_body;
        current_score_x = 7;
        player = new Player(5, 10, game_height, game_width);
        gamewin = newwin(game_height, game_width, start_y + 1, start_x);
        scorewin = newwin(1, game_width, start_y, start_x);

        keypad(gamewin, TRUE);
    }

    void generate_draw_scores()
    {
        // generate random score distinations
        wattron(gamewin, COLOR_PAIR(2));

        srand(time(0));
        for (size_t i = 0; i < max_score; i++)
        {
            while (1)
            {
                int randomX = (rand() % (game_width - 2)) + 1;
                int randomY = (rand() % (game_height - 2)) + 1;

                // if random numbers exist
                if (mvwinch(gamewin, randomY, randomX) == (int)(*score_body) || mvwinch(gamewin, randomY, randomX) == (int)(*player->body))
                {
                    continue;
                }

                // draw score
                mvwaddwstr(gamewin, randomY, randomX, score_body);
                break;
            }
        }
        wattroff(gamewin, COLOR_PAIR(2));
    }

    void draw()
    {
        box(gamewin, 0, 0);
        wattron(scorewin, A_REVERSE);
        wprintw(scorewin, "SCORE: 0/%i            || PRESS 'x' TO EXIT  ", max_score);
        wattroff(scorewin, A_REVERSE);

        wrefresh(gamewin);
        wrefresh(scorewin);
    }

    bool is_player_touch_score()
    {
        int current_player_cell = mvwinch(gamewin, player->y, player->x);
        return current_player_cell == (int)(*score_body);
    }
    void increment_score()
    {
        game_current_score++;
        wattron(scorewin, A_REVERSE);
        mvwprintw(scorewin, 0, current_score_x, "%i/%i", game_current_score, max_score);
        wattroff(scorewin, A_REVERSE);
        wrefresh(scorewin);

        if (game_current_score >= max_score)
        {
            mvwprintw(gamewin, 1, 1, "YOU WIN!!!");
        }
    }

    void end()
    {
        endwin();
    }
};

int main()
{
    // remove the hightlited cursor
    Game game{20, 45, 3, 6, L"♦", 30};
    setlocale(LC_ALL, "");
    curs_set(0);

    // check if the terminal has colors
    if (!has_colors())
    {
        printw("your terminal doesn't support colors, press any key to leave");
        refresh();
        getch();
        game.end();
        exit(0);
    }

    // initilize colors
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3,  COLOR_BLACK, COLOR_CYAN);

    game.draw();
    game.generate_draw_scores();

    game.player->draw(game.gamewin);

    while (1)
    {
        int ch = wgetch(game.gamewin);
        game.player->clear(game.gamewin);

        switch (ch)
        {
        case KEY_UP:
            game.player->mvup();
            break;
        case KEY_DOWN:
            game.player->mvdown();
            break;
        case KEY_RIGHT:
            game.player->mvright();
            break;
        case KEY_LEFT:
            game.player->mvleft();
            break;
        case (int)'x':
            game.end();
            exit(0);
        }
        if (game.is_player_touch_score())
        {
            game.increment_score();
        };

        game.player->draw(game.gamewin);
    }

    game.end();

    return 0;
}