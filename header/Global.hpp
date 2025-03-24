#pragma once

constexpr unsigned char CELL_SIZE = 8;

constexpr unsigned char CLEAR_EFFECT_DURATION = 8;

constexpr unsigned char COLUMNS = 10;
// 每消除2列就加速
constexpr unsigned char LINES_TO_INCREASE_SPEED = 2;

constexpr unsigned char MOVE_SPEED = 4;

constexpr unsigned char ROWS = 20;

constexpr unsigned char SCREEN_RESIZE = 4;

constexpr unsigned char SOFT_DROP_SPEED = 2;

extern unsigned char START_FALL_SPEED;

constexpr unsigned char EASY_FALL_SPEED = 32;

constexpr unsigned char MEDIUM_FALL_SPEED = 16;

constexpr unsigned char HARD_FALL_SPEED = 8;

constexpr unsigned short FRAME_DURATION = 17000;

struct Position
{
    char x;
    char y;
};
