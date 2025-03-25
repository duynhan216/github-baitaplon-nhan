#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <fstream>
#include <ctime>
#include <cmath>
#include <set>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//Screen
const int FRAME_PER_SECOND = 25; //fps
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 144;
const int COLOR_KEY_G = 176;
const int COLOR_KEY_B = 216;

const int RENDER_DRAW_COLOR = 192;

const int MAX_FRAME = 7;

const int COME_BACK_TIME = 30;
const int MAP_COME_BACK = 64*3;

#define BLANK_TILE 0
#define TILE_SIZE 64

#define MAX_MAP_X 150
#define MAX_MAP_Y 10

#define GRAVITY_SPEED 3
#define MAX_FALL_SPEED 15
#define PLAYER_SPEED 10
#define PLAYER_JUMP_VAL 30

typedef struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};

typedef struct Map
{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name;
};

#endif // COMMON_FUNCTION_H_
