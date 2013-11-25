#ifndef MAIN_H
#define MAIN_H 1
#include <time.h>

#define REFRESH_DELAY (60000 - 8000 * game_speed)

#define LEFT 1
#define RIGHT 2

void draw(void);
int update(void);
void init(void);
void init_window(void);
void post_process(void);

// void update_plate();
void draw_plate(void);
void update_ball(void);
void draw_ball(void);
int game_over(void);

void quithandler(int sig);

float dirn_x, dirn_y;
float cur_x, cur_y;
int plate_1, plate_2;
int plate_len;
int max_x, max_y;

int game_set;

time_t start_time;
float game_speed;

#endif
