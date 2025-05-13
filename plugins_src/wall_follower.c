#include <stdlib.h>
#include <stdbool.h>
#include "../include/solver.h"
#include "../include/maze.h"

static Maze *maze_ptr;
static int cx, cy, dir; // 0=上,1=右,2=下,3=左
static bool first_move;

static const int dirs[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

void wf_init(Maze *m)
{
    maze_ptr = m;
    cx = m->start_x;
    cy = m->start_y;
    dir = 0; // 初始面向「上」可自行調整
    first_move = true;
}

bool wf_next_step(Maze *m, int *x, int *y)
{
    (void)m;
    if (first_move)
    {
        first_move = false;
        *x = cx;
        *y = cy;
        return true;
    }
    // 對右手優先：右→直→左→後
    int priorities[4] = {(dir + 1) & 3, dir, (dir + 3) & 3, (dir + 2) & 3};
    for (int i = 0; i < 4; i++)
    {
        int nd = priorities[i];
        int nx = cx + dirs[nd][0];
        int ny = cy + dirs[nd][1];
        if (nx < 0 || nx >= m->width || ny < 0 || ny >= m->height)
            continue;
        if (m->grid[ny][nx] != '*')
        {
            dir = nd;
            cx = nx;
            cy = ny;
            break;
        }
    }
    *x = cx;
    *y = cy;
    // 到達終點後停止（最後一格在 main 顯示）
    if (cx == m->end_x && cy == m->end_y)
        return false;
    return true;
}

void wf_cleanup(void)
{
    maze_ptr = NULL;
}

Solver *create_solver(void)
{
    static Solver s = {wf_init, wf_next_step, wf_cleanup};
    return &s;
}