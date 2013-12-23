#include <stdio.h>
#include <signal.h>
#include <pthread.h>	
#include <sys/time.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <stdlib.h>
 
#define MAX_X 100  
#define MAX_Y 25 
 
struct point {
	int x;
	int y;
};

struct point SnakeBody[50];
struct point food;
struct winsize w;

int wait_flag;
int MAX_ROW;
int MAX_COL;
int score;
int speed = 200000;
char c_score[4];
 
int Length = 4; 
int life = 1;  
int input = 0;
pthread_t id;
 
void FoodCheck();
void FoodProduce();
void Initializition();
void SnakeHeadMovement();
void SnakeBodyMovement();
 
void DeathCheck();
void Paint();
void * waitForKey(void *);
void drawDot(int x,int y,char s);
void clearDot(int x,int y);
void end();
 
 
int main(int argc,char** argv) {

	ioctl(0, TIOCGWINSZ, &w);
	MAX_ROW = w.ws_row - 1;
	MAX_COL = w.ws_col - 26;
	score = 0;
 
	Initializition();
	refresh();
	curs_set(0);
	while (life) {

	Paint();
	usleep(speed);
	sprintf(c_score,"%d",score);
	mvaddstr(w.ws_row - 20, w.ws_col - 10, c_score);
	SnakeHeadMovement();
	DeathCheck();
	}
	end();
	return 0;
}
 
void * waitForKey(void *para) {
	while (wait_flag) {
	input = getch();
	usleep(1000);
	}
}
 
void end() {
	move(MAX_ROW/2-2, MAX_COL/2-23);
	addstr("--------------------------------------------");
	move(MAX_ROW/2-1, MAX_COL/2-23);
	addstr("                                            ");
	move(MAX_ROW/2, MAX_COL/2-23);
	addstr("           Press any key to quit!           ");
	move(MAX_ROW/2+1, MAX_COL/2-23);
	addstr("                                            ");
	move(MAX_ROW/2+2, MAX_COL/2-23);
	addstr("--------------------------------------------");
	refresh();
	wait_flag = 0;
	pthread_join(id, NULL);
	if(getch())
		endwin();
}
 
 
void FoodProduce() {
	int superposition = 0;
	int i;
	srand(time(NULL));
	do {
		food.x = (rand() % ((MAX_COL-2) / 2))*2+2; 
		food.y =rand() % (MAX_ROW-1)+1;
		for (i = 0; i < Length; i++) {
			if (food.x == SnakeBody[i].x && food.y == SnakeBody[i].y)
				superposition = 1;
			}
	}
	while (superposition);
}
 
void Initializition() {
 	initscr();
	noecho();
	int i;
	for (i = 3; i <= 6; i++) {
		SnakeBody[6 - i].x = 4;
		SnakeBody[6 - i].y = i;
	}
	FoodProduce();
	wait_flag = 1;
	int ret;
	ret = pthread_create(&id, NULL, waitForKey, NULL);
	if (ret != 0) {
		exit(1);
 	}
 
	for ( i = 0; i <= MAX_COL; i+=2) { 
		drawDot(i, 0,'*');
		drawDot(i, MAX_ROW,'*');
	}
	 
	for (i = 0; i <= MAX_ROW; i++) {
		drawDot(0, i,'*');
		drawDot(MAX_COL, i,'*');
	}

	for (i = MAX_COL + 1;i < w.ws_col; i+=2){
		drawDot(i, 0,'*');
		drawDot(i, MAX_ROW,'*');
	}

	for (i = 0; i < MAX_ROW; i++)
	{
		drawDot(w.ws_col - 1, i,'*');
	}

	mvaddstr(w.ws_row - 20, w.ws_col - 20, "Score:");
	mvaddstr(w.ws_row - 18, w.ws_col - 23, "Thanks for the game!");
	mvaddstr(w.ws_row - 13, w.ws_col - 23, "Press W to move up.");
	mvaddstr(w.ws_row - 12, w.ws_col - 23, "Press A to move left.");
	mvaddstr(w.ws_row - 11, w.ws_col - 23, "Press S to move down.");
	mvaddstr(w.ws_row - 10, w.ws_col - 23, "Press D to move right.");
	mvaddstr(w.ws_row - 9 , w.ws_col - 23, "Press Q to move quit.");

	refresh();
 
}

void SnakeBodyMovement() {
	int i;
	for (i = Length - 1; i > 0; i--) {
		SnakeBody[i].x = SnakeBody[i - 1].x;
		SnakeBody[i].y = SnakeBody[i - 1].y;
}
 
}
 
void SnakeHeadMovement() {
 
	clearDot(SnakeBody[Length - 1].x, SnakeBody[Length - 1].y);
	int directionX, directionY;
	int newX, newY;
	newX = SnakeBody[0].x;
	newY = SnakeBody[0].y;

	directionX = SnakeBody[0].x - SnakeBody[1].x;
	directionY = SnakeBody[0].y - SnakeBody[1].y;

	if (input =='w' && directionY<=0)
		newY--;
	else if (input =='s' && directionY>=0 )
		newY++;
	else if (input =='a' && directionX<=0)
		newX -= 2;
	else if (input =='d' && directionX>=0)
		newX += 2;
	else if (input == 'q'){
		life = 0;
		end();
	}
	else if (input == '+' && speed > 25000){
		speed = speed / 2;
		newX += directionX;
		newY += directionY;
	}
	else if (input == '-' && speed < 800000){
		speed = speed * 2;
		newX += directionX;
		newY += directionY;
	}
	else {
		newX += directionX;
		newY += directionY;
	}
	FoodCheck();
	SnakeBodyMovement();
	SnakeBody[0].x = newX;
	SnakeBody[0].y = newY;
 
}
 
void FoodCheck() {
	if (food.x == SnakeBody[0].x && food.y == SnakeBody[0].y) {
		Length = Length + 1;
		score = score + 1;
		FoodProduce();
	}
}
//判断是否死亡
 
void DeathCheck() {
	int i;
	if (SnakeBody[0].x <=1 || SnakeBody[0].x >= MAX_COL  || SnakeBody[0].y <= 0 || SnakeBody[0].y >=MAX_ROW)
		life = 0;
	for (i = 4; i < Length; i++)
		if (SnakeBody[0].x == SnakeBody[i].x && SnakeBody[0].y == SnakeBody[i].y)
	life = 0;
}
 
void Paint() {
	int i = 0;

	drawDot(SnakeBody[i].x, SnakeBody[i].y,'@');
	for (i=1; i < Length; i++) {
		drawDot(SnakeBody[i].x, SnakeBody[i].y,'o');
	}
	drawDot(food.x, food.y,'$');
	move(0,0);
	refresh();
}
 
void drawDot(int x,int y,char s) {
	move(y, x);
	addch(s);
}
 
void clearDot(int x,int y) {
	move(y, x);
	addch(' ');
}
