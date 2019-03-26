#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

char** make_array(int height, int width);
int game_logic(char** screen_new, char** screen, char* pause, int* prior_time, int HEIGHT, int WIDTH);
int render(char** screen, char** screen_new, int HEIGHT, int WIDTH, char populated);
int handle_input(char** screen, int* cursor_y, int* cursor_x, char* pause, int HEIGHT, int WIDTH, char populated);

int main(void)
{
    int HEIGHT = 24;
    int WIDTH = 80;
    
    const unsigned char populated = 178; //█

    int prior_time = 0;
    
    int cursor_y = 0;
    int cursor_x = 0;

    char pause = 0;

    char** screen = make_array(HEIGHT, WIDTH);
    char** screen_new = make_array(HEIGHT, WIDTH);
    
    /*char* data = calloc(WIDTH*HEIGHT, sizeof(char));
    char** screen = malloc(HEIGHT*sizeof(char*));
    char* tmp_data = calloc(WIDTH*HEIGHT, sizeof(char));
    char** screen_new = malloc(HEIGHT*sizeof(char*)); 
    
    //assigning row pointers.
    for(int i=0; i<HEIGHT; i++)
    {
	screen[i] = data + i*WIDTH;
	screen_new[i] = tmp_data + i*WIDTH;
	}*/

    initscr();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    
    for(int i=0; i<1;)
    {
      switch((handle_input(screen_new, &cursor_y, &cursor_x, &pause, HEIGHT, WIDTH, populated)))
	{
	  case 'q':
	    i = 1;
	    break;
	    
	  case 'r':
	    free(screen);
	    free(*screen);
	    free(screen_new);
	    free(*screen_new);
	    getmaxyx(stdscr, HEIGHT, WIDTH);
	    screen = make_array(HEIGHT, WIDTH);
	    screen_new = make_array(HEIGHT, WIDTH);
	    cursor_y = 0;
	    cursor_x = 0;
	    break;
	}
	//char** screen_new = game_logic(screen, &pause, &prior_time, HEIGHT, WIDTH);
	game_logic(screen_new, screen, &pause, &prior_time, HEIGHT, WIDTH);
	render(screen_new, screen, HEIGHT, WIDTH, populated);
	
	move(cursor_y, cursor_x);
	wrefresh(stdscr);

	//free(*screen_new);
	//free(screen_new);
    }
    

    free(*screen);
    free(screen);
    free(*screen_new);
    free(screen_new);

    endwin();
    
    return 0;
}

char** make_array(int height, int width)
{
  char* data = calloc(width*height, sizeof(char));
  char** array = malloc(height*sizeof(char*));

  for(int i=0; i<height; i++)
  {
    array[i] = data + i * width;
  }
  
  return array;
}

int render(char**screen_new, char** screen, int HEIGHT, int WIDTH, char populated)
{
    for(int y=0; y<HEIGHT; y++)
    {
	for(int x=0;x<WIDTH; x++)
	{
	    if((screen[y][x] = screen_new[y][x]) == 1)
	    {
		mvaddch(y, x, populated);
	    }
	    else
	    {
		mvaddch(y, x, ' ');
	    }
	}
    }

    return 0;
}

int game_logic(char** screen_new, char** screen, char* pause, int* prior_time, int HEIGHT, int WIDTH)
{
  //char* data = calloc(WIDTH * HEIGHT, sizeof(char));
  //char** screen_new = malloc(HEIGHT*sizeof(char*));

    if(0 == *pause && (clock() - *prior_time) > 100000)
    {
	*prior_time = clock();
	/*for(int i=0; i<HEIGHT; i++)
	{
	    screen_new[i] = data + i*WIDTH;
	    }*/
	
	for(int y=0;y<HEIGHT;y++)
	{
	    for(int x=0;x<WIDTH;x++)
	    {
		int x_upper = x - 1;
		int x_lower = x + 1;
		int y_upper = y - 1;
		int y_lower = y + 1;
		
		int populated = 0;
		
		if(y_upper < 0)
		{
		    y_upper = HEIGHT - 1;
		}
		if(y_lower > HEIGHT - 1)
		{
		    y_lower = 0;
		}

		if(x_upper < 0)
		{
		    x_upper = WIDTH - 1;
		}
		if(x_lower > WIDTH - 1)
		{
		    x_lower = 0;
		}
		
		if(screen[y_upper][x_upper] == 1)
		{
		    populated++;
		}
		if(screen[y_upper][x] == 1)
		{
		    populated++;
		}
		if(screen[y_upper][x_lower] == 1)
		{
		    populated++;
		}

		if(screen[y][x_upper] == 1)
		{
		    populated++;
		}
		/*if(a1[y][x] == 1)
		  {
		  populated++;
		  }*/
		if(screen[y][x_lower] == 1)
		{
		    populated++;
		}

		if(screen[y_lower][x_upper] == 1)
		{
		    populated++;
		}
		if(screen[y_lower][x] == 1)
		{
		    populated++;
		}
		if(screen[y_lower][x_lower] == 1)
		{
		    populated++;
		}
	    
		if(screen[y][x] == 1)
		{
		    switch(populated)
		    {
		        case 2: //Carries into the next one
		        case 3:
			    screen_new[y][x] = 1;
			    break;
		        default:
			    screen_new[y][x] = 0;
		    }
		}
		else
		{
		    if(populated == 3)
		    {
			screen_new[y][x] = 1;
		    }
		}

	    }
	}
	return 0;
    }
    if(1 == *pause)
    {
	
	mvprintw(0, 0, "PAUSE");
	return 0;
    }
}

int handle_input(char** screen_new, int* cursor_y, int* cursor_x, char* pause, int HEIGHT, int WIDTH, char populated)
{
    int key = getch();

    switch(key)
    {	    
        case KEY_UP:
	    *cursor_y -= 1;

	    if(*cursor_y < 0)
	    {
		*cursor_y = HEIGHT - 1;
	    }
	    break;

        case KEY_DOWN:
	    *cursor_y += 1;

	    if(*cursor_y > HEIGHT - 1)
	    {
		*cursor_y = 0;
	    }
	    break;

        case KEY_RIGHT:
	    *cursor_x += 1;

	    if(*cursor_x > WIDTH - 1)
	    {
		*cursor_x = 0;
	    }
	    break;

        case KEY_LEFT:
	    *cursor_x -= 1;

	    if(*cursor_x < 0)
	    {
		*cursor_x = WIDTH - 1;
	    }
	    break;
	    
        case ' ':
	    if(screen_new[*cursor_y][*cursor_x] == 0)
	    {
		screen_new[*cursor_y][*cursor_x] = 1;
		
	    }
	    else
	    {
		screen_new[*cursor_y][*cursor_x] = 0;
		//mvprintw(23, 0, "don't");
	    }
	    //mvprintw(23, 0, "populate");
	    
	    break;

        case 'p':
	    *pause = !*pause;
	    //game_logic(screen, pause, HEIGHT, WIDTH);
	    break;

        default:
	    break;
    }
    
    //move(*cursor_y, *cursor_x);

    return key;
}
