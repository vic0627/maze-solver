#ifndef ANIMATION_H
#define ANIMATION_H

#include "maze.h"

// 使用 ANSI 控制序列與 usleep 在 terminal 中實時演示老鼠走迷宮
// x, y: 老鼠當前座標
void animate_step(Maze *m, int x, int y);

// 設定每幀延遲（單位：毫秒）
void set_frame_delay_ms(int ms);

#endif