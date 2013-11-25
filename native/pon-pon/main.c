#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "main.h"

int main(int argc, char *argv[]) {
    init_window();

    init();

    while (update())
        draw();

    post_process();
    return 0;
}

void init_window(void)
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nonl();
    nodelay(stdscr, TRUE);
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    signal(SIGQUIT, quithandler);
    signal(SIGINT, quithandler);
}

void init(void)
{
    time(&start_time);
    srand(start_time);
    dirn_x = dirn_y = 0.f;
    while (dirn_x == 0.f || dirn_y == 0.f) {
        dirn_x = (float)(rand() % 11 - 5) / 5;
        dirn_y = (float)(rand() % 11 - 5) / 7;
    }

    getmaxyx(stdscr, max_y, max_x);

    cur_x = max_x / 2;
    cur_y = max_y / 2;

    plate_1 = plate_2 = cur_y;

    game_speed = 1;
    plate_len = 3;
    game_set = 0;

    refresh();
}

int update(void)
{
    if (game_set != 0)
        return game_over();
    switch(getch()) {
        case ERR:
            break;
        case 'a':
            if (plate_1 > 0)
                plate_1 -= 1;
            break;
        case 'z':
            if (plate_1 < max_y)
                plate_1 += 1;
            break;
        case KEY_UP:
            if (plate_2 > 0)
                plate_2 -= 1;
            break;
        case KEY_DOWN:
            if (plate_2 < max_y)
                plate_2 += 1;
            break;
        case 'q':
            return FALSE;
        case '=':
        case '+':
            if (plate_len < 10)
                ++plate_len;
            break;
        case '-':
            if (plate_len > 1)
                --plate_len;
            break;
        default :
            break;
    }

    update_ball();
    if (start_time - time(NULL) >= 10 * game_speed
            && REFRESH_DELAY >= 30000)
        game_speed += 0.5;
    return TRUE;
}

void draw(void)
{
    clear();
    draw_plate();
    draw_ball();

    usleep(REFRESH_DELAY);

    refresh();
}

void draw_ball()
{
    mvaddch(floorf(cur_y), floorf(cur_x), '@');
}

void draw_plate()
{
    int i;
    for (i = 0; i < plate_len; ++i) {
        mvaddch(plate_1 + i, 0, ACS_VLINE);
        mvaddch(plate_2 + i, max_x - 1, ACS_VLINE);
    }
}

void update_ball()
{
    cur_x += dirn_x;
    cur_y += dirn_y;

    if (cur_x <= 0) {
        if (plate_1 > cur_y || plate_1 + plate_len < cur_y) {
            game_set = RIGHT;
            return;
        }
    } else if (cur_x >= max_x){
        if (plate_2 > cur_y || plate_2 + plate_len < cur_y) {
            game_set = LEFT;
            return;
        }
    }

    if (cur_x >= max_x || cur_x <= 0) {
        cur_x -= dirn_x * 2;
        dirn_x = - dirn_x;
    }
    if (cur_y >= max_y || cur_y <= 0) {
        cur_y -= dirn_y * 2;
        dirn_y = -dirn_y;
    }
}

void post_process(void)
{
    endwin();
}

void quithandler(int sig)
{
    post_process();
    exit(0);
}

int game_over(void)
{
    clear();
    char *str;
    if (game_set == LEFT)
        str = "Game set LEFT!";
    else str = "Game set RIGHT!";
    mvaddstr(max_y/2, max_x/2, str);
    refresh();
    mvaddstr(max_y/2+1, max_x/2, "Retry? (y/n)");
    nodelay(stdscr, FALSE);
    switch (getch()) {
        case 'y':
            init();
            nodelay(stdscr, TRUE);
            return TRUE;
        case 'n':
            quithandler(0);
            return FALSE;
        default:
            return TRUE;
    }
    return TRUE;
}
