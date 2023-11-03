#pragma once

#define SAFE_DELETE(ptr) if(ptr) { delete ptr; ptr = nullptr; }

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 962;

const int BLOCK_SIZE = 64;
const int EMPTY_TILE = 0;
const int WALL_TILE = 1;
const int BACKGROUND_TILE = 2;
const int SIZE_BACKGROUND_TILE = 3;
const int LEVEL_OFFSET_X = 320;
const int LEVEL_OFFSET_Y = 130;

#if defined(_DEBUG)
#define CHECK_MEMORY_LEAKS 0
#define CREATE_DEBUG_CONSOLE 1
#define PAUSE_CONSOLE_WHEN_CLOSING 0
#define START_IN_GAME 1
#define SHOW_DEBUG_GRID 0
#define STARTING_LIFE 2
#define START_AT_ROUND 0

#else

#define CHECK_MEMORY_LEAKS 0
#define CREATE_DEBUG_CONSOLE 0
#define PAUSE_CONSOLE_WHEN_CLOSING 0
#define START_IN_GAME 0
#define SHOW_DEBUG_GRID 0

#endif





