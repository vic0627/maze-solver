#include <stdlib.h>
#include <stdbool.h>
#include "../include/solver.h"
#include "../include/maze.h"

#define MAX_DIM 100

// 方向：上、右、下、左
static const int dirs[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

typedef struct
{
    int x, y, next_dir;
} StackNode;

// DFS 狀態
static StackNode *stack;
static int top;
static bool visited[MAX_DIM][MAX_DIM];
static Maze *maze_ptr;
static bool done;

void dfs_init(Maze *m)
{
    maze_ptr = m;
    top = 0;
    done = false;
    // 初始化 visited
    for (int y = 0; y < m->height; y++)
        for (int x = 0; x < m->width; x++)
            visited[y][x] = false;
    // 分配 stack
    stack = malloc(m->width * m->height * sizeof(StackNode));
    // 推入起點
    stack[top++] = (StackNode){m->start_x, m->start_y, 0};
    visited[m->start_y][m->start_x] = true;
}

bool dfs_next_step(Maze *m, int *x, int *y)
{
    if (done || top == 0)
        return false;
    StackNode *node = &stack[top - 1];
    int cx = node->x, cy = node->y;
    // 試探下一個方向
    while (node->next_dir < 4)
    {
        int dir = node->next_dir++;
        int nx = cx + dirs[dir][0];
        int ny = cy + dirs[dir][1];
        // 邊界內且未訪問且可通行
        if (nx >= 0 && nx < m->width && ny >= 0 && ny < m->height &&
            !visited[ny][nx] && m->grid[ny][nx] != '*')
        {
            // 訪問
            visited[ny][nx] = true;
            // 推入新的節點
            stack[top++] = (StackNode){nx, ny, 0};
            *x = nx;
            *y = ny;
            // 檢查是否到終點
            if (nx == m->end_x && ny == m->end_y)
                done = true;
            return !done;
        }
    }
    // 所有方向皆試過，回溯
    top--;
    if (top > 0)
    {
        *x = stack[top - 1].x;
        *y = stack[top - 1].y;
        return true;
    }
    return false;
}

void dfs_cleanup(void)
{
    free(stack);
    maze_ptr = NULL;
}

Solver *create_solver(void)
{
    static Solver solver = {dfs_init, dfs_next_step, dfs_cleanup};
    return &solver;
}
