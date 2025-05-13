#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../include/solver.h"
#include "../include/maze.h"

typedef struct
{
    int x, y;
    int g, f;   // g: 起點至此成本，f=g+h
    int parent; // 父節點索引
} Node;

#define MAX_NODES 10000

static Node *nodes;
static bool *in_open, *in_closed;
static int *path;
static int path_len, path_pos;
static Maze *maze_ptr;

// 曼哈頓啟發函數
static int heuristic(int x, int y, int ex, int ey)
{
    return abs(x - ex) + abs(y - ey);
}

// 小型二元最小堆，比較 Node.f
static int heap_size;
static int *heap;
static void heap_push(int idx)
{
    int i = heap_size++;
    heap[i] = idx;
    while (i > 0)
    {
        int p = (i - 1) / 2;
        if (nodes[heap[p]].f <= nodes[heap[i]].f)
            break;
        int tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}
static int heap_pop(void)
{
    int ret = heap[0];
    heap_size--;
    heap[0] = heap[heap_size];
    int i = 0;
    while (true)
    {
        int l = 2 * i + 1, r = 2 * i + 2, smallest = i;
        if (l < heap_size && nodes[heap[l]].f < nodes[heap[smallest]].f)
            smallest = l;
        if (r < heap_size && nodes[heap[r]].f < nodes[heap[smallest]].f)
            smallest = r;
        if (smallest == i)
            break;
        int tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return ret;
}

void astar_init(Maze *m)
{
    maze_ptr = m;
    int cap = m->width * m->height;
    nodes = malloc(cap * sizeof(Node));
    in_open = calloc(cap, sizeof(bool));
    in_closed = calloc(cap, sizeof(bool));
    heap = malloc(cap * sizeof(int));
    path = malloc(cap * sizeof(int));
    heap_size = 0;
    path_len = path_pos = 0;

    // 將起點放入 Open
    int si = m->start_y * m->width + m->start_x;
    nodes[si] = (Node){m->start_x, m->start_y, 0,
                       heuristic(m->start_x, m->start_y, m->end_x, m->end_y),
                       -1};
    in_open[si] = true;
    heap_push(si);

    // A* 搜尋全路徑
    int dirs[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    int end_idx = -1;
    while (heap_size)
    {
        int cur = heap_pop();
        in_closed[cur] = true;
        int cx = nodes[cur].x, cy = nodes[cur].y;
        if (cx == m->end_x && cy == m->end_y)
        {
            end_idx = cur;
            break;
        }
        for (int d = 0; d < 4; d++)
        {
            int nx = cx + dirs[d][0], ny = cy + dirs[d][1];
            if (nx < 0 || nx >= m->width || ny < 0 || ny >= m->height)
                continue;
            if (m->grid[ny][nx] == '*')
                continue;
            int ni = ny * m->width + nx;
            if (in_closed[ni])
                continue;
            int g2 = nodes[cur].g + 1;
            if (!in_open[ni] || g2 < nodes[ni].g)
            {
                nodes[ni] = (Node){nx, ny, g2,
                                   g2 + heuristic(nx, ny, m->end_x, m->end_y),
                                   cur};
                if (!in_open[ni])
                {
                    in_open[ni] = true;
                    heap_push(ni);
                }
            }
        }
    }

    // 回溯路徑 index
    if (end_idx >= 0)
    {
        int idx = end_idx;
        while (idx >= 0)
        {
            path[path_len++] = idx;
            idx = nodes[idx].parent;
        }
        // 反轉
        for (int i = 0; i < path_len / 2; i++)
        {
            int t = path[i];
            path[i] = path[path_len - 1 - i];
            path[path_len - 1 - i] = t;
        }
    }
}

bool astar_next_step(Maze *m, int *x, int *y)
{
    if (path_pos >= path_len)
        return false;
    int idx = path[path_pos++];
    *x = idx % m->width;
    *y = idx / m->width;
    return path_pos < path_len;
}

void astar_cleanup(void)
{
    free(nodes);
    free(in_open);
    free(in_closed);
    free(heap);
    free(path);
}

Solver *create_solver(void)
{
    static Solver s = {astar_init, astar_next_step, astar_cleanup};
    return &s;
}