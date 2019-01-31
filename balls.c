#include <ncurses.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int width = 10, height = 10;

#define GRAV -0.05;

struct Ball
{
    float x, y;
    float vx, vy;
    int color;
};

void move_balls(struct Ball *balls, int numballs)
{
    getmaxyx(stdscr, height, width);
    int i;
    for (i = 0; i < numballs; i++)
    {
        struct Ball *b = &balls[i];
        b->x += b->vx;
        b->y += b->vy;
        b->vy += GRAV;

        if (b->x >= width - 1 || b->x < 0)
        {
            b->vx *= -1;
            b->x += b->vx;
        }
        /* Bouncing off floor */
        if (b->y > height - 1)
        {
            b->y = height - 1;
            b->vy *= -1;
        }
        /* Bouncing off flooring */
        if (b->y < 0)
        {
            b->y = 0;
            b->vy = (float)rand() / (float)(RAND_MAX / 2) + 1;
        }
    }
}

void print_balls(struct Ball *balls, int numballs)
{
    int i;
    erase();
    for (i = 0; i < numballs; i++)
    {
        struct Ball b = balls[i];
        attron(COLOR_PAIR(b.color) | A_BOLD);
        mvprintw(height - floor(b.y), floor(b.x), "o");
        attroff(COLOR_PAIR(b.color) | A_BOLD);
    }
    refresh();
}


int main(int argc, char *argv[])
{
    int numballs = 100;
    int i;
    if (argc > 1)
    {
        numballs = atoi(argv[1]);
    }
    struct Ball balls[numballs];
    srand(time(NULL));

    initscr();
    nodelay(stdscr, TRUE);
    cbreak();
    curs_set(FALSE);
    start_color();
    use_default_colors();
    for (i = 0; i < 8; i++)
    {
        init_pair(i + 1, i, -1);
    }

    getmaxyx(stdscr, height, width);
    for (i = 0; i < numballs; i++)
    {
        struct Ball *b = &balls[i];
        b->x = rand() % width;
        b->y = rand() % height;
        b->vx = rand() % 4 + 1;
        b->vy = rand() % 2 + 1;
        b->color = rand() % 8 + 1;
        if (rand() % 2 == 0)
        {
            b->vx *= -1;
        }
        if (rand() % 2 == 0)
        {
            b->vy *= -1;
        }
    }
    
    int c;
    while ((c = getch()) != 'q')
    {
        print_balls(balls, numballs);
        move_balls(balls, numballs);
        usleep(33000);
    }

    nodelay(stdscr, FALSE);
    nocbreak();
    endwin();
    return EXIT_SUCCESS;
}
