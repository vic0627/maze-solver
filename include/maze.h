#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>

#define MAX_DIM 100

// Maze 結構
typedef struct
{
    int width;            // 迷宮寬度
    int height;           // 迷宮高度
    char **grid;          // 二維字元陣列，儲存 '*'、's'、'e'、' ' 等符號
    int start_x, start_y; // 起點座標
    int end_x, end_y;     // 終點座標
} Maze;

// 讀取並驗證迷宮檔案，成功回傳 true，失敗回傳 false
bool load_maze(const char *filepath, Maze *m);
// 釋放動態配置記憶體
void free_maze(Maze *m);
// 列印迷宮
void print_maze(const Maze *m);

#endif