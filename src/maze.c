#include "../include/maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 內部輔助：檢查符號合法性
static bool valid_char(char c)
{
    return c == '*' || c == 's' || c == 'e' || c == ' ';
}

bool load_maze(const char *filepath, Maze *m)
{
    FILE *fp = fopen(filepath, "r");
    if (!fp)
        return false;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int row = 0;
    int w = -1;
    int sx = -1, sy = -1, ex = -1, ey = -1;

    // 第一遍：讀取行數並驗證每行長度與字元
    while ((read = getline(&line, &len, fp)) != -1)
    {
        // 去除行尾換行
        if (line[read - 1] == '\n')
        {
            line[--read] = '\0';
        }
        if (w < 0)
            w = read;
        else if (read != w)
        {
            fclose(fp);
            free(line);
            return false;
        }
        if (w < 3 || w > MAX_DIM)
        {
            fclose(fp);
            free(line);
            return false;
        }
        row++;
    }
    if (row < 3 || row > MAX_DIM)
    {
        fclose(fp);
        free(line);
        return false;
    }
    int h = row;

    // 分配 grid
    m->width = w;
    m->height = h;
    m->grid = malloc(h * sizeof(char *));
    if (!m->grid)
    {
        fclose(fp);
        free(line);
        return false;
    }
    for (int i = 0; i < h; i++)
    {
        m->grid[i] = malloc(w + 1);
        if (!m->grid[i])
        {
            fclose(fp);
            free(line);
            return false;
        }
    }

    // 第二遍：填入 grid、定位起終點
    rewind(fp);
    row = 0;
    int start_count = 0, end_count = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (line[read - 1] == '\n')
        {
            line[--read] = '\0';
        }
        for (int col = 0; col < w; col++)
        {
            char c = line[col];
            if (!valid_char(c))
            {
                fclose(fp);
                free(line);
                return false;
            }
            m->grid[row][col] = c;
            if (c == 's')
            {
                sx = col;
                sy = row;
                start_count++;
            }
            else if (c == 'e')
            {
                ex = col;
                ey = row;
                end_count++;
            }
        }
        m->grid[row][w] = '\0';
        row++;
    }
    free(line);
    fclose(fp);

    // 起終點檢查
    if (start_count != 1 || end_count != 1)
        return false;
    // 不可在四角
    if ((sy == 0 && sx == 0) || (sy == 0 && sx == w - 1) || (sy == h - 1 && sx == 0) || (sy == h - 1 && sx == w - 1))
        return false;
    if ((ey == 0 && ex == 0) || (ey == 0 && ex == w - 1) || (ey == h - 1 && ex == 0) || (ey == h - 1 && ex == w - 1))
        return false;
    // 四邊不可為空白
    for (int x = 0; x < w; x++)
    {
        if (m->grid[0][x] == ' ' || m->grid[h - 1][x] == ' ')
            return false;
    }
    for (int y = 0; y < h; y++)
    {
        if (m->grid[y][0] == ' ' || m->grid[y][w - 1] == ' ')
            return false;
    }

    m->start_x = sx;
    m->start_y = sy;
    m->end_x = ex;
    m->end_y = ey;
    return true;
}

void free_maze(Maze *m)
{
    for (int i = 0; i < m->height; i++)
    {
        free(m->grid[i]);
    }
    free(m->grid);
}

void print_maze(const Maze *m)
{
    for (int y = 0; y < m->height; y++)
    {
        printf("%s\n", m->grid[y]);
    }
}