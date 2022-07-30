#include <ncurses.h>

int main()
{
    initscr();
    refresh();
    noecho();
    WINDOW *menuwin = newwin(10, 40, 2, 4);
    box(menuwin, 0, 0);

    char choices[3][10] = {"Play", "Dashboard", "Setting"};
    int choice;
    int highlight = 0;

    keypad(menuwin, TRUE);
    int chociesLength = (sizeof(choices) / sizeof(*choices));
    while (1)
    {
        for (int i = 0; i < chociesLength; i++)
        {
            if (i == highlight)
            {
                wattron(menuwin, A_REVERSE);
                mvwprintw(menuwin, i + 1, 2, choices[i]);
                wattroff(menuwin, A_REVERSE);
            }
            else
            {
                mvwprintw(menuwin, i + 1, 2, choices[i]);
            }
        }
        wrefresh(menuwin);
        choice = wgetch(menuwin);

        switch (choice)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            break;
        case KEY_DOWN:
            if (highlight < chociesLength - 1)
            {
                highlight++;
            }
            break;
        default:
            break;
        }

        if (choice == 10)
        {
            wclear(menuwin);
            box(menuwin, 0, 0);
            mvwprintw(menuwin, 1, 2, "Your Choice: %s", choices[highlight]);
            mvwprintw(menuwin, 3, 2, "back to menu -> press cancel");
            wrefresh(menuwin);
            
            while (1)
            {
                choice = wgetch(menuwin);
                if (choice == 263)
                    break;
            }
            wclear(menuwin);
            box(menuwin, 0, 0);
        }
    }

    endwin();
    return 0;
}