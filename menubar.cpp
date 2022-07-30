#include <ncurses.h>
#include <locale.h>
#include <string>

class Menu {
public: 
	std::string text;
	std::string *options;
	int options_num ;
	char ch;
	int start_x;
	int selected_option;
public: 
	Menu(std::string text, char ch, std::string *options, int options_num) {
		this->text = text;
		this->ch = ch;
		this->options = options;
		this->options_num = options_num;
		this->selected_option = 0;
	}

	void selectNextOption()
	{
		this->selected_option++;
		if(this->selected_option >=  this->options_num)
		{
			this->selected_option = 0;
		}
	}

	void selectPrevOption()
	{
		this->selected_option--;
		if(this->selected_option < 0) 
		{
			this->selected_option = this->options_num -1;
		}
	}
};

class MenuBar {
	public: 
		Menu* menus;
		int menus_num;
		int selected_num;
		WINDOW* optionwin;
		WINDOW* win;
	public:
		MenuBar(WINDOW* &win, Menu* menus, int menus_num)
		{	
			this->win = win;
			this->menus = menus;
			this->menus_num = menus_num;
			this->selected_num = -1;
			int current_start_x = 2;

			int yMax, xMax, yBeg, xBeg;

			getmaxyx(this->win, yMax, xMax);
			getbegyx(this->win, yBeg, xBeg);
			this->optionwin = newwin(yMax - 2, xMax -2, yBeg + 1, xBeg + 1);
			keypad(this->optionwin, TRUE);
			wrefresh(this->optionwin);

			// generate all start_x of menus
			for(size_t i = 0; i < menus_num; i++)
			{
				this->menus[i].start_x = current_start_x;
				current_start_x += this->menus[i].text.length() + 1;
			}

		}


		void drawOptions()
		{
			int start_y = 1;
			int start_x = 1;
			int optionsLen = this->menus[this->selected_num].options_num;

			for(size_t i = 0; i < optionsLen; i++)
			{
				mvwprintw(this->optionwin, start_y, start_x, this->menus[this->selected_num].options[i].c_str());
				start_y++;
			}	

			wrefresh(this->optionwin);

		}
		void draw()
		{
			for(size_t i =0; i < this->menus_num; i++)
			{
				if(i == this->selected_num)
				{
					wattron(this->win, A_REVERSE);
					mvwprintw(this->win, 0, this->menus[i].start_x, this->menus[i].text.c_str());
					wattroff(this->win, A_REVERSE);


				}
				else 
				{
				  mvwprintw(this->win, 0, this->menus[i].start_x, this->menus[i].text.c_str());
				}
			}
		}

		void handlTrigger(char& ch)
		{
			for(size_t i = 0; i < this->menus_num; i++)
			{
				if(this->menus[i].ch == ch) {
					this->selected_num = i;
					return;
				}
			}

			this->selected_num = -1;
		}
};

// detect ctl+c
int main()
{
	initscr();
	setlocale(LC_ALL, "");
	cbreak();
	noecho();
	curs_set(0);
	refresh();

	if(!has_colors())
	{
		return -1;
	}

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_CYAN);

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	WINDOW* win = newwin(yMax / 2, xMax / 2,  yMax / 4, xMax / 4);
	box(win, 0, 0);
   
    std::string options1[3] = {"Create file", "Open file", "Settings"};
    std::string options2[3] = {"Previous", "Next", "Termnial"};
    std::string options3[3] = {"Git/remote", "Set colors", "Themes"};
	Menu menus[3] = {
		Menu("File", 'f', options1, 3),
		Menu("Edit", 'e',options1,3),
		Menu("Options", 'o',options1,3)
	};

	MenuBar menubar = MenuBar(win, menus, 3);
	menubar.draw();
	menubar.drawOptions();

	char ch;
	while((ch = wgetch(win)))
	{
		if(ch == 'x'){
			endwin();
			return 0;
		}
		menubar.handlTrigger(ch);
		menubar.draw();

	}	

	endwin();
	return 0;
}