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
#include <vector>
#include <cstdlib>
#include <iomanip>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//Screen
const int FRAME_PER_SECOND = 30; //fps
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 144;
const int COLOR_KEY_G = 176;
const int COLOR_KEY_B = 216;

const int RENDER_DRAW_COLOR = 192;

const int MAX_FRAME = 7;
const int MAX_SHIELD_FRAME = 7;
const int THREAT_FREAM_NUM = 4;

#define NUM_HP 3

const int COME_BACK_TIME = 30;
const int MAP_COME_BACK = 64*3;

const int SPEED_UP_INTERVAL = 10000;
const float SPEED_INCREASE_FACTOR = 0.0f;
const float MAX_INCREASE_PLAYER = 3.0f;
const float MAX_INCREASE_THREAT = 5.0f;

static const unsigned SPAWN_MIN_THREAT = 500;
static const unsigned SPAWN_MAX_THREAT = 2000;
static const unsigned SPAWN_MIN_COIN = 500;
static const unsigned SPAWN_MAX_COIN = 2000;
static const unsigned SPAWN_MIN_HP = 500;
static const unsigned SPAWN_MAX_HP = 2000;

#define BLANK_TILE 0
#define TILE_SIZE 64

#define MAX_MAP_X 30
#define MAX_MAP_Y 10

#define GRAVITY_SPEED 4
#define PLAYER_SPEED 15

#define PLAYER_JUMP_VAL 40

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

    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);

#endif
