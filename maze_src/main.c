#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

static int WIDTH, HEIGHT;
static char **maze;
static bool **visited;

// 四個方向
const int DX[4] = {-1, 1, 0, 0};
const int DY[4] = {0, 0, -1, 1};

// 隨機打亂方向順序
void shuffle_dirs(int *dirs, int n)
{
    for (int i = n - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        int tmp = dirs[i];
        dirs[i] = dirs[j];
        dirs[j] = tmp;
    }
}

// 遞迴 carve 函式
void carve(int x, int y)
{
    visited[y][x] = true;
    maze[2 * y + 1][2 * x + 1] = ' ';

    int dirs[4] = {0, 1, 2, 3};
    shuffle_dirs(dirs, 4);

    for (int i = 0; i < 4; ++i)
    {
        int dir = dirs[i];
        int nx = x + DX[dir], ny = y + DY[dir];
        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && !visited[ny][nx])
        {
            // 打通牆壁
            maze[2 * y + 1 + DY[dir]][2 * x + 1 + DX[dir]] = ' ';
            carve(nx, ny);
        }
    }
}

// 生成迷宮
void generate_maze(int width, int height)
{
    WIDTH = width;
    HEIGHT = height;

    // 分配 maze 與 visited
    maze = malloc((2 * HEIGHT + 1) * sizeof(char *));
    for (int i = 0; i < 2 * HEIGHT + 1; ++i)
    {
        maze[i] = malloc((2 * WIDTH + 1) * sizeof(char));
        for (int j = 0; j < 2 * WIDTH + 1; ++j)
        {
            maze[i][j] = '*';
        }
    }
    visited = malloc(HEIGHT * sizeof(bool *));
    for (int i = 0; i < HEIGHT; ++i)
    {
        visited[i] = calloc(WIDTH, sizeof(bool));
    }

    // 隨機起點
    int sx = rand() % WIDTH;
    int sy = rand() % HEIGHT;
    carve(sx, sy);

    // 入口與出口
    maze[1][0] = 's';                      // 入口標記為 s
    maze[2 * HEIGHT - 1][2 * WIDTH] = 'e'; // 出口標記為 e
}

// 列印到 stdout
void print_maze()
{
    for (int i = 0; i < 2 * HEIGHT + 1; ++i)
    {
        fwrite(maze[i], 1, 2 * WIDTH + 1, stdout);
        putchar('\n');
    }
}

// 儲存到文字檔
void save_maze(const char *filepath)
{
    FILE *f = fopen(filepath, "w");
    if (!f)
    {
        perror("開啟檔案失敗");
        return;
    }
    for (int i = 0; i < 2 * HEIGHT + 1; ++i)
    {
        fwrite(maze[i], 1, 2 * WIDTH + 1, f);
        fputc('\n', f);
    }
    fclose(f);
    printf("迷宮已儲存到：%s\n", filepath);
}

// 釋放記憶體
void free_maze()
{
    for (int i = 0; i < 2 * HEIGHT + 1; ++i)
    {
        free(maze[i]);
    }
    free(maze);
    for (int i = 0; i < HEIGHT; ++i)
    {
        free(visited[i]);
    }
    free(visited);
}

int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));

    char *filepath = "maze.txt";
    int W = 20, H = 10;

    if (argc == 3)
    {
        W = atoi(argv[1]);
        H = atoi(argv[2]);
    }
    else if (argc == 5 && (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0))
    {
        filepath = argv[2];
        W = atoi(argv[3]);
        H = atoi(argv[4]);
    }

    generate_maze(W, H);
    print_maze();
    save_maze(filepath);
    free_maze();

    return 0;
}