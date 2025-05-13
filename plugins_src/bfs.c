#include <stdlib.h>
#include <stdbool.h>
#include "../include/solver.h"
#include "../include/maze.h"

#define MAX_DIM 100

// 方向：上、右、下、左
static const int dirs[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

typedef struct
{
    int x, y;
} Point;

typedef struct
{
    int x, y, prev;
} QueueNode;

// BFS 狀態
static QueueNode *queue_nodes;
static int head, tail;
static bool visited[MAX_DIM][MAX_DIM];
static int *path_indices;
static int path_len;
static int path_pos;
static Maze *maze_ptr;

void bfs_init(Maze *m)
{
    maze_ptr = m;
    head = 0;
    tail = 0;
    path_len = 0;
    path_pos = 0;
    // 初始化 visited
    for (int y = 0; y < m->height; y++)
        for (int x = 0; x < m->width; x++)
            visited[y][x] = false;
    // 分配隊列與路徑索引
    int cap = m->width * m->height;
    queue_nodes = malloc(cap * sizeof(QueueNode));
    path_indices = malloc(cap * sizeof(int));
    // 開始點
    queue_nodes[tail++] = (QueueNode){m->start_x, m->start_y, -1};
    visited[m->start_y][m->start_x] = true;
    // BFS 搜尋
    int end_index = -1;
    while (head < tail)
    {
        QueueNode node = queue_nodes[head];
        if (node.x == m->end_x && node.y == m->end_y)
        {
            end_index = head;
            break;
        }
        for (int d = 0; d < 4; d++)
        {
            int nx = node.x + dirs[d][0];
            int ny = node.y + dirs[d][1];
            if (nx >= 0 && nx < m->width && ny >= 0 && ny < m->height &&
                !visited[ny][nx] && m->grid[ny][nx] != '*')
            {
                visited[ny][nx] = true;
                queue_nodes[tail++] = (QueueNode){nx, ny, head};
            }
        }
        head++;
    }
    // 若找到終點，回溯路徑
    if (end_index >= 0)
    {
        int idx = end_index;
        while (idx >= 0)
        {
            path_indices[path_len++] = idx;
            idx = queue_nodes[idx].prev;
        }
        // 反轉路徑
        for (int i = 0; i < path_len / 2; i++)
        {
            int tmp = path_indices[i];
            path_indices[i] = path_indices[path_len - 1 - i];
            path_indices[path_len - 1 - i] = tmp;
        }
    }
}

bool bfs_next_step(Maze *m, int *x, int *y)
{
    (void)m; // avoid unused parameter warning
    if (path_pos >= path_len)
        return false;
    QueueNode node = queue_nodes[path_indices[path_pos++]];
    *x = node.x;
    *y = node.y;
    return path_pos < path_len;
}

void bfs_cleanup(void)
{
    free(queue_nodes);
    free(path_indices);
    maze_ptr = NULL;
}

Solver *create_solver(void)
{
    static Solver solver = {bfs_init, bfs_next_step, bfs_cleanup};
    return &solver;
}
