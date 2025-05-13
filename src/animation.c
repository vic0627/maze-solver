#include "../include/animation.h"
#include <stdio.h>
#include <unistd.h> // usleep

// 動態幀延遲（microseconds），預設 500 ms
static int frame_delay_us = 500 * 1000;

// 外部可呼叫此函式，以 ms 為單位設定延遲
void set_frame_delay_ms(int ms)
{
    if (ms > 0)
        frame_delay_us = ms * 1000;
}

// 清除螢幕並將游標移到左上角
static void clear_screen(void)
{
    // ANSI escape sequences: \033[2J 清除螢幕, \033[H 移到座標 (1,1)
    printf("\033[2J");
    printf("\033[H");
}

void animate_step(Maze *m, int x, int y)
{
    // 1. 更新迷宮地圖：
    //    - 先將舊的老鼠標記還原為空白（或保留為探訪過的路徑）
    //    - 再將新位置標記為 '@'
    static int prev_x = -1, prev_y = -1;
    if (prev_x >= 0 && prev_y >= 0)
    {
        // 還原為空白路徑，或可改成其他標記如 '.'
        if (m->grid[prev_y][prev_x] != 's' && m->grid[prev_y][prev_x] != 'e')
        {
            m->grid[prev_y][prev_x] = ' ';
        }
    }
    // 標記老鼠當前位置
    m->grid[y][x] = '@';

    // 2. 清除並重繪 (含 @ 顯示為黃色)
    clear_screen();
    for (int row = 0; row < m->height; row++)
    {
        for (int col = 0; col < m->width; col++)
        {
            char c = m->grid[row][col];
            if (c == '@')
            {
                // 33 = 黃色前景，0m 重置樣式
                printf("\033[33m@\033[0m");
            }
            else
            {
                putchar(c);
            }
        }
        putchar('\n');
    }
    fflush(stdout);

    // 3. 記錄位置以供下次還原
    prev_x = x;
    prev_y = y;

    // 4. 延遲
    usleep(frame_delay_us);
}
