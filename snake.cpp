#include <iostream>
#include <ncurses.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include<locale.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;
struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
int tick; // 스테이지마다 속도 다르게 표현
int xo=15;  // 뱀의 초기 위치
int yo=15;  // 뱀의 초기 위치
bool gameOver=false;
int bodyX[30] = {0} , bodyY[30] = {0};
int body_len = 3;                // 게임 실행 시 초기 뱀길이 3

int growthX, growthY;
int poisonX, poisonY;
int gate1, gate2;
int gate1_x,gate1_y;
int gate2_x,gate2_y;

int mission_body_len=5;           // 미션 보드 뱀길이
int Growth_Item = 2;              // 미션 보드 Growth_Item
int numberOfgate = 2;             // 미션 보드 numberOfgate
int timeseconds = 20;             // 미션 보드 최소 합산 초

int current_body_len = 3;             // 스코어 보드 뱀길이
int current_Growth_Item = 0;          // 스코어 보드 Growth_Item
int current_numberOfgate = 0;        // 스코어 보드 numberOfgate
double current_timeseconds = 0.0;   // 스코어 보드 최소 합산 초

int direction[5][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 0} };
int map[4][32][32] =  //stage 1, stage 2 , stage 3, stage 4  맵 설정
{
{
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
},
{
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
},
{
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
},
{
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
}
};
char opposition_key = ' ' ;
char key_input = ' ';    // 게임실행시 움직임 입력받는 key 값
char key_input2 = ' ';   // 시작화면에서 q 버튼 누르는 받는 key 값

WINDOW *win2;
WINDOW *win3;
WINDOW *win4;
WINDOW *win5;
WINDOW *NextStage_window;

void startScreen();          // 맨 처음 시작 화면 출력
void GameScreen(int stage_number);    // stage_number 값에 따른 게임실행화면 출력

void stage(int stage_num);         // stage_num 에 따른 stage 구현
void EatG(int stage_num);          // G 를 먹으면
void PoisonP(int stage_num);       // P 를 먹으면
void GameOver();              // GameOver 화면 윈도우에 띄우기
void keyinput(int stage_num, char key);
void GrowthItem(int stage_num);     // G 출현
void PoisonItem(int stage_num);     // P 출현
void MakeGate(int stage_number);     // Gate 출현
int HeadPosition();              // gate 위치에 따른 뱀의 위치,방향 설정
void Gate(int stage_num, int direct);   // 방향에 따라 달라지는 Gate 통과방식
void NextStage();         // NextStage 가기 전 화면 출력
void Success(double current_timeseconds);      // 마지막 스테이지에서 성공시 성공화면 출력

int main()
{
  int stage_number = 0;

  tick = 0;
  std::srand(static_cast<int>(std::time(0))); // 현재시간을랜덤해서 seed 로 준다.
  int whileTimes = 0;

  bodyX[1] = xo+1;
  bodyY[1] = yo;
  bodyX[2] = xo+2;
  bodyY[2] = yo;

  setlocale(LC_ALL,"");
  startScreen();

  for(int stage_number=0 ; stage_number<4 ; stage_number++ ){
    gameOver=false;           // for문을 돌 때, gameOver 값을 false 로 다시 초기화 해준다.
    if(key_input == 'd'){
      opposition_key = 'a';
      body_len=3;
      xo = 15;
      yo = 15;
      bodyX[1] = xo-1;
      bodyY[1] = yo;
      bodyX[2] = xo-2;
      bodyY[2] = yo;
    }
    else{
      body_len=3;
      xo = 15;
      yo = 15;
      bodyX[1] = xo+1;
      bodyY[1] = yo;
      bodyX[2] = xo+2;
      bodyY[2] = yo;
    }

    GameScreen(stage_number);
    GrowthItem(stage_number);
    PoisonItem(stage_number);
    MakeGate(stage_number);
    if(stage_number != 0)
    {   // Stage1 을 제외한 스테이지에서는 다시 스코어보드 변수 초기화, 미션 보드 변수 변화
      current_Growth_Item=0;
      current_body_len=3;
      current_numberOfgate = 0;
      mission_body_len+=2;
      Growth_Item+=2;
      numberOfgate+=1;
      timeseconds+=20;
    }
    usleep(1000000);

    while(gameOver==false){

        current_timeseconds = ((double)whileTimes/1000000.0000);
        if(kbhit()){
          key_input = getch();
        }

        keyinput(stage_number, key_input);  //xo, yo 값 바꿔줌, body위치 재설정
        EatG(stage_number); //뱀의 머리가 닿으면 G가 다른위치로 바뀌고 길이 늘려주기
        PoisonP(stage_number);
        Gate(stage_number, HeadPosition());

        if(stage_number==0 && current_body_len >= mission_body_len && current_Growth_Item>=2 && current_timeseconds>timeseconds &&current_numberOfgate>=numberOfgate) gameOver=true;
        if(stage_number==1 && current_body_len >= mission_body_len && current_Growth_Item>=2 && current_timeseconds>timeseconds &&current_numberOfgate>=numberOfgate) gameOver=true;
        if(stage_number==2 && current_body_len >= mission_body_len && current_Growth_Item>=2 && current_timeseconds>timeseconds &&current_numberOfgate>=numberOfgate) gameOver=true;
        if(stage_number==3 && current_body_len >= mission_body_len && current_Growth_Item>=2 && current_timeseconds>timeseconds &&current_numberOfgate>=numberOfgate) gameOver=true;
        stage(stage_number);

        GameScreen(stage_number);
        usleep(tick);
        whileTimes += tick;
        if(current_timeseconds>=7.0 && whileTimes % 15000000 == 0 ) MakeGate(stage_number);
        if(whileTimes % 3000000 == 0){ // 와일문 20번 돌때마다 틱바꿔주기
          GrowthItem(stage_number);
          PoisonItem(stage_number);
        }
    }
    if (stage_number==3){
      Success(current_timeseconds);     // 모든 스테이지 성공 시, 게임 실행 합산 시간 알려줌.
    }
    else{
      NextStage();       // 스테이지 넘어갈 때 화면 출력
    }
    clear();
  }
  return 0;
}

void startScreen(){     // 게임 시작화면
  initscr();        // Curses 모드 시작
  attrset(A_BOLD);
  border('|','|','-','-','+','+','+','+');
  mvprintw(6,23,"____________   ______________ ___________   _____________________  ____________");
  mvprintw(7,23,"__  ___/__  | / /__    |__  //_/__  ____/   __  ____/__    |__   |/  /__  ____/");
  mvprintw(8,23,"_____ \\__   |/ /__  /| |_  ,<  __  __/      _  / __ __  /| |_  /|_/ /__  __/ ");
  mvprintw(9,23,"____/ /_  /|  / _  ___ |  /| | _  /___      / /_/ / _  ___ |  /  / / _  /___  ");
  mvprintw(10,23,"/____/ /_/ |_/  /_/  |_/_/ |_| /_____/      \\____/  /_/  |_/_/  /_/  /_____/ ");
  mvprintw(19,18,"                 up");
  mvprintw(20,18,"               ______ ");
  mvprintw(21,18,"              ||    ||");
  mvprintw(22,18,"              || w  ||");
  mvprintw(23,18,"              ||____||");
  mvprintw(24,18,"              |/____\\|");
  mvprintw(25,18,"      ______   ______   ______");
  mvprintw(26,18,"     ||    || ||    || ||    ||");
  mvprintw(27,18,"left || a  || || s  || || d  || right ");
  mvprintw(28,18,"     ||____|| ||____|| ||____|| ");
  mvprintw(29,18,"     |/____\\| |/____\\| |/____\\| ");
  mvprintw(30,18,"                                  ");
  mvprintw(31,18,"          down");
  mvprintw(14,50,"           /^\\/^\\ ");
  mvprintw(15,50,"         _|__|  O|");
  mvprintw(16,50,"\\/     /~     \\_/ \\ ");
  mvprintw(17,50," \\____|__________/  \\ ");
  mvprintw(18,50,"        \\_______      \\ ");
  mvprintw(19,50,"                `\\     \\                 \\ ");
  mvprintw(20,50,"                  |     |                  \\ ");
  mvprintw(21,50,"                 /      /                    \\ ");
  mvprintw(22,50,"                /     /                       \\\\");
  mvprintw(23,50,"              /      /                         \\ \\ ");
  mvprintw(24,50,"             /     /                            \\  \\ ");
  mvprintw(25,50,"           /     /             _----_            \\   \\ ");
  mvprintw(26,51,"         /     /           _-~      ~-_         |   |");
  mvprintw(27,58," (      (        _-~    _--_    ~-_     _/   |");
  mvprintw(28,51,"         \\      ~-____-~    _-~    ~-_    ~-_-~    /");
  mvprintw(29,51,"           ~-_           _-~          ~-_       _-~");
  mvprintw(30,50,"               ~--______-~                ~-___-~");
  mvprintw(32,10,"      ____                      _____ _   _ _____ _____ ____    _              _             _   _ ");
  mvprintw(33,10,"     |  _ \\ _ __ ___  ___ ___  | ____| \\ | |_   _| ____|  _ \\  | |_ ___    ___| |_ __ _ _ __| |_| |");
  mvprintw(34,10,"     | |_) | '__/ _ \\/ __/ __| |  _| |  \\| | | | |  _| | |_) | | __/ _ \\  / __| __/ _` | '__| __| |");
  mvprintw(35,10,"     |  __/| | |  __/\\__ \\__ \\ | |___| |\\  | | | | |___|  _ <  | || (_) | \\__ \\ || (_| | |  | |_|_|");
  mvprintw(36,10,"     |_|   |_|  \\___||___/___/ |_____|_| \\_| |_| |_____|_| \\_\\  \\__\\___/  |___/\\__\\__,_|_|   \\__(_)");
  key_input2 = getch();
  clear();
  if(key_input2=='q'){  // q 버튼 누르면 터미널로 돌아감.
    endwin();
    exit(0);
  }
  else if(key_input2=='\n'){    // Enter 버튼을 누르면 게임시작.
      clear();
      refresh();
    }
  else{   // 'q' 키와 '엔터키 ' 이외에는 그냥 계속 시작화면 보여줌.(게임을 시작하려면 Enter키 눌러야 한다.)
      startScreen();
    }
}

void GameScreen(int x){        // 게임 실행 화면 출력
  initscr();
  stage(x);

  start_color();
  init_pair(1, COLOR_BLACK,COLOR_WHITE);
  win2 = newwin(13,40,15,60);

  mvwprintw(win2,1,1,"*-*-*-*-*-*-*-SCORE-BOARD-*-*-*-*-*-*-");
  wborder(win2,'|','|','-','-','X','X','X','X');
  mvwprintw(win2,3,10,"Length :  ");
  mvwprintw(win2,3,20,"%d",current_body_len);    // 스코어 보드에 나타나는 현재 뱀 길이
  mvwprintw(win2,6,10,"Growth-Item:     ");
  mvwprintw(win2,6,25,"%d",current_Growth_Item);  // 스코어 보드에 나타나는 현재 먹은 Growth_Item 개수
  mvwprintw(win2,9,10,"# of Gate:     ");
  mvwprintw(win2,9,23,"%d",current_numberOfgate);   // 스코어 보드에 나타나는 현재 통과한 Gatr 개수
  mvwprintw(win2,11,10,"seconds:     ");
  mvwprintw(win2,11,20,"%f",current_timeseconds);    // 스코어 보드에 나타나는 현재 흐르는 시간
  mvwprintw(win2,11,24,"초 가 지나는 중!");

  wrefresh(win2);

  win3 = newwin(13,40,29,60);
  mvwprintw(win3,1,1,"*-*-*-*-*-*-*-MISSION-*-*-*-*-*-*-*-*-*-");
  mvwprintw(win3,3,10,"Snake_length:     ");
  mvwprintw(win3,3,25,"%d",mission_body_len);      // 미션 보드에  나타나는 뱀 길이
  mvwprintw(win3,6,10,"+(growth):     ");
  mvwprintw(win3,6,22,"%d",Growth_Item);          // 미션 보드에 나타나는 Growth_Item 개수
  mvwprintw(win3,9,10,"Gate:     ");
  mvwprintw(win3,9,18,"%d",numberOfgate);      // 미션 보드에 나타나는 통과할 게이트 수
  mvwprintw(win3,11,10,"seconds:     ");
  mvwprintw(win3,11,18," 합산");
  mvwprintw(win3,11,24,"%d",timeseconds);       // 미션 보드에 나타나는 실행해야 할 합산 최소 시간
  mvwprintw(win3,11,26,"초는 버티세요!");
  wborder(win3,'|','|','-','-','X','X','X','X');
  wrefresh(win3);

  win4 = newwin(8,80,0,25);
  attrset(A_BOLD);
  mvwprintw(win4,0,2,"                 _                                                  ____");
  mvwprintw(win4,1,2,"                | |                                                / . .\\");
  mvwprintw(win4,2,2," ___ _ __   __ _| | _____    __ _  __ _ _ __ ___   ___             \\  ---<");
  mvwprintw(win4,3,2,"/ __| '_ \\ / _` | |/ / _ \\  / _` |/ _` | '_ ` _ \\ / _ \\             \\  /");
  mvwprintw(win4,4,2,"\\__ \\ | | | (_| |   <  __/ | (_| | (_| | | | | | |  __/   __________/ /");
  mvwprintw(win4,5,2,"|___/_| |_|\\__,_|_|\\_\\___|  \\__, |\\__,_|_| |_| |_|\\___|-=:___________/ ");
  mvwprintw(win4,6,2,"                             __/ |                     ");
  mvwprintw(win4,7,2,"                            |___/ ");
  wrefresh(win4);

  win5 = newwin(3,40,11,60);
  attrset(A_BOLD);
  wbkgd(win5,COLOR_PAIR(1));
  wattron(win5,COLOR_PAIR(1));
  mvwprintw(win5,1,15,"STAGE ");
  mvwprintw(win5,1,22,"%d",x+1);     // 몇번 째 stage인지 알려주는 출력 윈도우

  wrefresh(win5);
  curs_set(0);

  delwin(win2);
  delwin(win3);
  delwin(win4);
}

void stage(int stage_num){

  WINDOW *win1;

  tick = 150000;
  initscr();
  win1 = newwin(32,32,10,24);
  wrefresh(win1);
  mvwprintw(win1, yo, xo, "0");
  for(int i=0;i<32;i++){
    for(int j=0;j<32;j++){
      if(map[stage_num][i][j] == 2) mvwprintw(win1,i, j, "*" );      // map 배열 값이 2 이면 immune-wall
      else if ((map[stage_num][i][j] == 1) || (map[stage_num][i][j] == 3 )){  // map  배열 값이 1 or 3 이면 wall,gate
        mvwprintw(win1,i, j, "+" );
      }
      if(map[stage_num][i][j] == 3 && current_timeseconds>=7.0){mvwprintw(win1, i,j, "X");}

      for(int k = 1; k<body_len; k++){
        if(growthX == j && growthY == i && map[stage_num][i][j] != 1 && map[stage_num][i][j] !=2 && map[stage_num][i][j] !=3){
          mvwprintw(win1, i, j, "G");
        }
        else if(poisonX == j && poisonY == i && map[stage_num][i][j] != 1 && map[stage_num][i][j] !=2 && map[stage_num][i][j] !=3)
        {
          mvwprintw(win1, i, j, "P");
        }
        else if(bodyX[k] == j && bodyY[k] == i){
            mvwprintw(win1, i, j, "o");
        }
      }
    }
  }
  wrefresh(win1);
  keypad(stdscr,TRUE);
  noecho();
  curs_set(0);
  delwin(win1);
  endwin();
}

void GrowthItem(int stage_num)
{
  int crush = 0;
  while(1)
  {
    growthX = (rand()%30) + 1;
    growthY = (rand()%30) + 1;
    for(int i = 0; i < body_len; i++)
    {
      if(growthX == bodyX[i] && growthY == bodyY[i])
      {
        crush++;
        break;
      }
    }
    // 벽이나 gate 위치와 다른 위치에서 아이템이 출현하도록 설정
    if(map[stage_num][growthY][growthX] == 1 ||map[stage_num][growthY][growthX] == 2 ||map[stage_num][growthY][growthX] == 3)crush++;
    if(crush == 1) continue;
    else break;
  }
}

void PoisonItem(int stage_num)
{
  int crush = 0;
  while(1)
  {
    poisonX = (rand()%28) + 1;
    poisonY = (rand()%28) + 1;
    for(int i = 0; i < body_len; i++)
    {
      if((poisonX == bodyX[i] && poisonY == bodyY[i]) || (poisonX == growthX && poisonY == growthY))
      {
        crush++;
        break;
      }
      // 벽이나 gate 위치와 다른 위치에서 아이템이 출현하도록 설정
      if(map[stage_num][poisonY][poisonX] == 1|| map[stage_num][poisonY][poisonX] == 2 ||map[stage_num][poisonY][poisonX] == 3){crush++;}
    }
    if(crush == 1) continue;
    else break;
  }
}

void EatG(int stage_num){         // G를 먹으면
  if(xo == growthX && yo == growthY)
  {
    GrowthItem(stage_num);
    body_len++;
    current_body_len++;
    current_Growth_Item++;
    bodyX[body_len-1] = bodyX[body_len-2];
    bodyY[body_len-1] = bodyY[body_len-2];
  }
}

void PoisonP(int stage_num){      // P를 먹으면
  if(xo == poisonX && yo == poisonY)
  {
    PoisonItem(stage_num);
    body_len--;
    current_body_len--;
    bodyX[body_len-1] = 0;
    bodyY[body_len-1] = 0;
  }
}

void keyinput(int stage_num, char key){
  int tmpX = bodyX[0];
  int tmpY = bodyY[0];
  int tmp2X, tmp2Y;

  bodyX[0] = xo;
  bodyY[0] = yo;

  if(key =='q'){
    GameOver();
    clear();
    endwin();
    exit(0);
  }

  else if(key =='w' || key =='a' || key =='s'||key =='d'){
    for(int i = 0; i<body_len; i++){
      tmp2X = bodyX[i];
      tmp2Y = bodyY[i];
      bodyX[i] = tmpX;
      bodyY[i] = tmpY;
      tmpX = tmp2X;
      tmpY = tmp2Y;
      }
    if (opposition_key == key){  // 현재 입력 받은 키와 전에 입력받은 키의 반대키가 같으면 게임종료.
      gameOver = true;
      GameOver();
    }
    else{
      switch(key){
        case 'w'://up
          yo--;
          opposition_key = 's';  //opposition_key 재설정
          break;
        case 'a'://left
          xo--;
          opposition_key = 'd';
          break;
        case 'd'://right
          xo++;
          opposition_key = 'a';
          break;
        case 's': //down
          yo++;
          opposition_key = 'w';
          break;
        default:
          break;
      }
    }

    if (map[stage_num][yo][xo] ==1 ){ // 벽 닿으면 종료
      gameOver = true;
      GameOver();
    }
    for(int i = 1; i < sizeof(bodyX) / sizeof(int); i++) // 머리와 몸이 닿으면 종료되는 코드
    {
      if(xo == bodyX[i] && yo == bodyY[i])
      {
        gameOver = true;
        GameOver();
      }
    }
  }
  else{}
  if (body_len == 2){  // 현재 입력 받은 키와 전에 입력받은 키의 반대키가 같으면 게임종료.
    gameOver = true;
    GameOver();
  }
}

void GameOver(){   // 게임 오버 화면 출력
  clear();
  initscr();
  WINDOW *gameover_window;   // 게임오버  윈도우

  wborder(gameover_window,'|','|','-','-','+','+','+','+');

  gameover_window = newwin(25,43,5,5);
  wbkgd(gameover_window,COLOR_PAIR(2));
  wattron(gameover_window,COLOR_PAIR(2));
  attrset(A_BOLD);
  mvprintw(12,25,"  ,--,     .--.           ,---.    .---..-.   .-.,---.  ,---.   ");
  mvprintw(13,25,".' .'     / /\\ \\ |\\    /| | .-'   / .-. )\\ \\ / / | .-'  | .-.\\  ");
  mvprintw(14,25,"|  |  __ / /__\\ \\|(\\  / | | `-.   | | |(_)\\ V /  | `-.  | `-'/ ");
  mvprintw(15,25,"\\  \\ ( _)|  __  |(_)\\/  | | .-'   | | | |  ) /   | .-'  |   (  ");
  mvprintw(16,25," \\  `-) )| |  |)|| \\  / | |  `--. \\ `-' / (_)    |  `--.| |\\ \\ ");
  mvprintw(17,25," )\\____/ |_|  (_)| |\\/| | /( __.'  )---'         /( __.'|_| \\)\\ ");
  mvprintw(18,25,"(__)             '-'  '-'(__)     (_)           (__)        (__)");
  mvprintw(19,59,"                      __    __    __    __");
  mvprintw(20,59,"                     /  \\  /  \\  /  \\  /  \\");
  mvprintw(21,5,"__________________________________________________________________________/  __\\/  __\\/  __\\/  __\\___________________");
  mvprintw(22,5,"_________________________________________________________________________/  /__/  /__/  /__/  /______________________");
  mvprintw(23,59,"                   | / \\   / \\   / \\   / \\  \\____");
  mvprintw(24,59,"                   |/   \\_/   \\_/   \\_/   \\    o \\");
  mvprintw(25,59,"                                           \\_____/--<");
  wborder(gameover_window,'|','|','-','-','X','X','X','X');
  wrefresh(gameover_window);
  getch();
  clear();
  endwin();
  exit(0);
}

void NextStage(){
  initscr();

  wborder(NextStage_window,'|','|','-','-','+','+','+','+');
  NextStage_window = newwin(25,43,5,5);
  attrset(A_BOLD);
  mvprintw(11,25," ____");
  mvprintw(12,25,"/\\  _`\\     ");
  mvprintw(13,25,"\\ \\,\\L\\_\\  __  __    ___    ___     __    ____    ____  ");
  mvprintw(14,25," \\/_\\__ \\ /\\ \\/\\ \\  /'___\\ /'___\\ /'__`\\ /',__\\  /',__\\ ");
  mvprintw(15,25,"   /\\ \\L\\ \\ \\ \\_\\ \\/\\ \\__//\\ \\__//\\  __//\\__, `\\/\\__, `\\");
  mvprintw(16,25,"   \\ `\\____\\ \\____/\\ \\____\\ \\____\\ \\____\\/\\____/\\/\\____/");
  mvprintw(17,25,"    \\/_____/\\/___/  \\/____/\\/____/\\/____/\\/___/  \\/___/ ");

  mvprintw(19,25," __  __                  __        ____    __");
  mvprintw(20,25,"/\\ \\/\\ \\                /\\ \\__    /\\  _`\\ /\\ \\__ ");
  mvprintw(21,25,"\\ \\ `\\\\ \\     __   __  _\\ \\ ,_\\   \\ \\,\\L\\_\\ \\ ,_\\    __       __      __  ");
  mvprintw(22,25," \\ \\ , ` \\  /'__`\\/\\ \\/'\\\\ \\ \\/    \\/_\\__ \\\\ \\ \\/  /'__`\\   /'_ `\\  /'__`\\");
  mvprintw(23,25,"  \\ \\ \\`\\ \\/\\  __/\\/>  </ \\ \\ \\_     /\\ \\L\\ \\ \\ \\_/\\ \\L\\.\\_/\\ \\L\\ \\/\\  __/ ");
  mvprintw(24,25,"   \\ \\_\\ \\_\\ \\____\\/\\_/\\_\\ \\ \\__\\    \\ `\\____\\ \\__\\ \\__/.\\_\\ \\____ \\ \\____\\");
  mvprintw(25,25,"    \\/_/\\/_/\\/____/\\//\\/_/  \\/__/     \\/_____/\\/__/\\/__/\\/_/\\/___L\\ \\/____/");
  mvprintw(26,25,"                                                              /\\____/ ");
  mvprintw(27,25,"                                                              \\_/__/ ");
  wrefresh(NextStage_window);

  getch();
  clear();
  endwin();
}

void Success(double current_timeseconds){
  initscr();
  WINDOW *success;
  success = newwin(45,85,5,5);
  wborder(success,'|','|','-','-','+','+','+','+');

  attrset(A_BOLD);
  mvwprintw(success,11,30,"success");
  mvwprintw(success,15,25,"게임 클리어 시간이 총 ");
  mvwprintw(success,17,25,"%f",current_timeseconds);      // 몸 길이
  mvwprintw(success,17,30,"초 걸렸습니다!!!!!!");      // 몸 길이

  wrefresh(success);

  getch();
  clear();
  endwin();
}

void MakeGate(int stage_number){
  vector <vector<int> > temp;
  for(int i=0; i<32;i++){
    for(int j=0; j<32; j++){
      if(map[stage_number][i][j]==3){
        map[stage_number][i][j]=1;
      }
      if(map[stage_number][i][j]==1){
        vector<int> temp1;

        temp1.push_back(j);
        temp1.push_back(i);

        temp.push_back(temp1);
      }
    }
  }
  while(true){
    gate1=rand()%temp.size();
    gate2=rand()%temp.size();
      if (gate1!=gate2) {
        break;
      }
  }
  gate1_x = temp[gate1][0];
  gate1_y = temp[gate1][1];
  map[stage_number][gate1_y][gate1_x]=3;

  gate2_x = temp[gate2][0];
  gate2_y= temp[gate2][1];
  map[stage_number][gate2_y][gate2_x]=3;
}

void Gate(int stage_num, int direct)
{ if(direct == -1){

}
  else if(direct / 4 == 0)  //Gate1
  {
    while(1)
    {
      direct %= 4;
      int dirX = direction[direct][0];
      int dirY = direction[direct][1];
      if(gate2_x+dirX<0 ||gate2_y+dirY <0|| gate2_x+dirX >31||gate2_y+dirY>31){
        direct++;
        if(direct == 4)
        {
          direct = 0;
        }
        continue;
      }
      else if(map[stage_num][gate2_x+dirX][gate2_y+dirY] == 1|| map[stage_num][gate2_x+dirX][gate2_y+dirY] == 2 || map[stage_num][gate2_x+dirX][gate2_y+dirY] == 3 )
      {
        direct++;
        if(direct == 4)
        {
          direct = 0;
        }
        continue;
      }
      else
      {
        xo = gate2_x + dirX;
        yo = gate2_y + dirY;
        if(direct ==0){
          key_input = 'd';
          opposition_key = 'a';
        }
        else if(direct == 1){
          key_input = 's';
          opposition_key = 'w';
        }
        else if(direct == 2){
          key_input = 'a';
          opposition_key = 'd';
        }
        else if(direct == 3){
          key_input = 'w';
          opposition_key = 's';
        }
        break;
      }
    }
  }
  else if(direct / 4 == 1)    // Gate2
  {
    while(1)
    {
      direct %= 4;
      int dirX = direction[direct][0];
      int dirY = direction[direct][1];
      if(gate1_x+dirX<0 ||gate1_y+dirY <0|| gate1_x+dirX >31||gate1_y+dirY>31){
        direct++;
        if(direct == 4)
        {
          direct = 0;
        }
        continue;

      }
      else if(map[stage_num][gate1_x+dirX][gate1_y+dirY] == 1|| map[stage_num][gate1_x+dirX][gate1_y+dirY] == 2||map[stage_num][gate1_x+dirX][gate1_y+dirY] == 3 )
      {
        direct++;
        if(direct == 4)
        {
          direct = 0;
        }
        continue;
      }
      else
      {
        xo = gate1_x + dirX;
        yo = gate1_y + dirY;
        if(direct ==0){
          key_input = 'd';
          opposition_key = 'a';
        }
        else if(direct == 1){
          key_input = 's';
          opposition_key = 'w';
        }
        else if(direct == 2){
          key_input = 'a';
          opposition_key = 'd';
        }
        else if(direct == 3){
          key_input = 'w';
          opposition_key = 's';
        }
        break;
      }
    }
  }
}

int HeadPosition(){
  current_numberOfgate +=1;
  if( xo == gate1_x && yo == gate1_y){
    if( bodyX[1]==gate1_x-1 && bodyY[1]==gate1_y) return 0;  // 진행방향 오른쪽
    else if( bodyX[1]==gate1_x && bodyY[1]==gate1_y-1) return 1;   // 진행 방향 아래
    else if( bodyX[1]==gate1_x+1 && bodyY[1]==gate1_y) return 2;   // 진행 방향 왼쪽
    else if( bodyX[1]==gate1_x && bodyY[1]==gate1_y+1) return 3;   // 진행 방향 위쪽
    else{
      current_numberOfgate -= 1;
      return -1;}
  }
  else if( xo == gate2_x && yo == gate2_y){
    if( bodyX[1]==gate2_x-1 && bodyY[1]==gate2_y) return 4;  // 진행방향 오른쪽
    else if( bodyX[1]==gate2_x && bodyY[1]==gate2_y-1) return 5;   // 진행 방향 아래
    else if( bodyX[1]==gate2_x+1 && bodyY[1]==gate2_y) return 6;   // 진행 방향 왼쪽
    else if( bodyX[1]==gate2_x && bodyY[1]==gate2_y+1) return 7;   // 진행 방향 위쪽
    else{current_numberOfgate -= 1;
      return -1;}
    }
  else{current_numberOfgate -= 1;return -1;
    }
}
