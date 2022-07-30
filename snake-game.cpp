#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
// #include <locale.h>

#define BOARD_WIDTH 60
#define BOARD_HEIGHT 30
#define START_X 6
#define START_Y 3

enum Directions
{
    up = -1,
    down = 1,
    right = -2,
    left = 2
};

class Snake
{
public:
    std::vector<std::vector<int>> body; // => matrix {y, x}
    Directions direction;
    WINDOW *gamewin;
    char segment;

public:
    Snake(WINDOW *gamewin, char segment, int starter_length, int starter_y, int starter_x)
    {
        this->gamewin = gamewin;
        this->direction = left;
        this->segment = segment;

        for (int i = 0; i < starter_length; i++)
        {
            std::vector<int> newPosSegment = {starter_y, starter_x++};
            body.push_back(newPosSegment);
        }
    }

    void incrementSnakeBody()
    {
        std::vector<int> newSegment = body[body.size() - 1];
        newSegment[1]++;
        body.push_back(newSegment);
    }

    void updateSnakeState()
    {

        int head[2] = {body[0][0], body[0][1]};

        switch (direction)
        {
        case up:
            body[0][0]--;
            break;
        case down:
            body[0][0]++;
            break;
        case right:
            body[0][1]++;
            break;
        case left:
            body[0][1]--;
            break;
        default:
            break;
        }

        for (size_t i = 1; i < body.size(); i++)
        {
            int temp[2] = {body[i][0], body[i][1]};
            body[i][0] = head[0];
            body[i][1] = head[1];

            head[0] = temp[0];
            head[1] = temp[1];
        }
    }

    bool isOuterBoard(int endY, int endX, int startY = 1, int startX = 1)
    {
        return (
            body[0][0] < startY ||
            body[0][1] < startX ||
            body[0][0] > endY ||
            body[0][1] > endX);
    }

    bool isCrashBody()
    {
        return (mvwinch(gamewin, body[0][0], body[0][1]) == (int)segment);
    }

    void clearSnakeTail()
    {
        std::vector<int> snakeTail = body[body.size() - 1];
        mvwprintw(gamewin, snakeTail[0], snakeTail[1], " ");
    }
    void draw()
    {
        for (size_t i = 0; i < body.size(); i++)
        {
            mvwprintw(gamewin, body[i][0], body[i][1], "%c", segment);
        }
    }
};

class SnakeGame
{
private:
    Snake *snake;
    WINDOW *gamewin;
    bool gameOver;
    int startX;
    int startY;
    int wWidth;  // window width
    int wHeight; // window height
    char food;
    int score;

public:
    SnakeGame(WINDOW *&gamewin, Snake *snake, char food)
    {
        this->gamewin = gamewin;
        this->snake = snake;
        this->startY = gamewin->_begy;
        this->startX = gamewin->_begx;
        this->wWidth = gamewin->_maxx;
        this->wHeight = gamewin->_maxy;
        this->gameOver = false;
        this->food = food;
        this->score = 0;
    }
    void init()
    {
        wtimeout(gamewin, 150);
        keypad(gamewin, TRUE);
    }
    void drawBoard()
    {
        box(gamewin, 0, 0);
        wrefresh(gamewin);
    }
    bool isGameOver()
    {
        return gameOver;
    }
    void updateSnake()
    {
        snake->clearSnakeTail();
        snake->updateSnakeState();
    }

    void validateSnake()
    {
        if (snake->isCrashBody() || snake->isOuterBoard(gamewin->_maxy - 1, gamewin->_maxx - 1))
        {
            gameOver = true;
        } else if(isSnakeEeatTheFood()) {
            snake->incrementSnakeBody();
            score++;
            printScore();
            generateDrawFood();
        }
    }

    void printScore() {
        wattron(gamewin, A_REVERSE);
        mvwprintw(gamewin, 0, 0, "SCORE: %i ", score);
        wattroff(gamewin, A_REVERSE);
    }
    void generateDrawFood()
    {
        while (1)
        {
            int randomX = (rand() % (gamewin->_maxx - 2)) + 1;
            int randomY = (rand() % (gamewin->_maxy - 2)) + 1;

            // if random numbers exist
            if (mvwinch(gamewin, randomY, randomX) == (int)(snake->segment))
            {
                continue;
            }

            // draw score
            mvwaddch(gamewin, randomY, randomX, food);
            break;
        }
    }

    bool isSnakeEeatTheFood()
    {
        return (mvwinch(gamewin, snake->body[0][0], snake->body[0][1]) == (int)food);
    }

    void drawSnake()
    {
        snake->draw();
    }
    int getDirection(int &input)
    {

        switch (input)
        {
        case KEY_UP:
            return Directions::up;
        case KEY_DOWN:
            return Directions::down;
        case KEY_RIGHT:
            return Directions::right;
        case KEY_LEFT:
            return Directions::left;
        case 'x':
            endwin();
            exit(0);
        default:
            return 0;
        }
    }

    void processInput()
    {
        int input = getInput();
        int curDirection = (int)getDirection(input);
        if (curDirection && curDirection + snake->direction != 0)
        {
            snake->direction = (Directions)curDirection;
        }
    }

    int getInput()
    {
        return wgetch(gamewin);
    }
};

int main()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    clear();
    srand(time(0));
    refresh();

    WINDOW *gamewin = newwin(BOARD_HEIGHT, BOARD_WIDTH, START_Y, START_X);

    // Snake snake;
    Snake *snake = new Snake{gamewin, '#', 200, BOARD_HEIGHT / 2, BOARD_WIDTH / 2};
    SnakeGame Game{gamewin, snake, 'A'};

    Game.init();
    Game.drawBoard();
    Game.printScore();
    Game.generateDrawFood();
    while (!Game.isGameOver())
    {
        Game.processInput();
        Game.updateSnake();
        Game.validateSnake();
        Game.drawSnake();
    }

    wattron(gamewin, A_REVERSE);
    wprintw(gamewin, "You Lose Click anything to leave :/");
    wattroff(gamewin, A_REVERSE);
    wrefresh(gamewin);

    getch();
    getch();
    delete snake;
    endwin();
    return 0;
}