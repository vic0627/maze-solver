#include "../include/fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 100

bool read_maze_file(const char *filepath, char ***lines, int *rows, int *cols)
{
    FILE *fp = fopen(filepath, "r");
    if (!fp)
        return false;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;
    int width = -1;
    char **buffer = malloc((MAX_DIM + 1) * sizeof(char *));
    if (!buffer)
    {
        fclose(fp);
        return false;
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (line[read - 1] == '\n')
        {
            line[--read] = '\0';
        }
        if (width < 0)
            width = read;
        else if (read != width || width < 3 || width > MAX_DIM)
        {
            free(line);
            fclose(fp);
            free_lines(buffer, count);
            return false;
        }
        if (count >= MAX_DIM)
        {
            free(line);
            fclose(fp);
            free_lines(buffer, count);
            return false;
        }
        buffer[count] = malloc(width + 1);
        if (!buffer[count])
        {
            free(line);
            fclose(fp);
            free_lines(buffer, count);
            return false;
        }
        strcpy(buffer[count], line);
        count++;
    }
    free(line);
    fclose(fp);

    if (count < 3 || count > MAX_DIM)
    {
        free_lines(buffer, count);
        return false;
    }

    *lines = buffer;
    *rows = count;
    *cols = width;
    return true;
}

void free_lines(char **lines, int rows)
{
    if (!lines)
        return;
    for (int i = 0; i < rows; i++)
        free(lines[i]);
    free(lines);
}

bool validate_symbols(char **lines, int rows, int cols)
{
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            char c = lines[y][x];
            if (!(c == '*' || c == 's' || c == 'e' || c == ' '))
                return false;
        }
    }
    return true;
}

bool validate_structure(char **lines, int rows, int cols,
                        int *start_x, int *start_y,
                        int *end_x, int *end_y)
{
    int sx = -1, sy = -1, ex = -1, ey = -1;
    int start_count = 0, end_count = 0;

    // 找起終點並計數
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            if (lines[y][x] == 's')
            {
                sx = x;
                sy = y;
                start_count++;
            }
            if (lines[y][x] == 'e')
            {
                ex = x;
                ey = y;
                end_count++;
            }
        }
    }
    if (start_count != 1 || end_count != 1)
        return false;

    // 不可在四角
    if ((sy == 0 && sx == 0) || (sy == 0 && sx == cols - 1) ||
        (sy == rows - 1 && sx == 0) || (sy == rows - 1 && sx == cols - 1))
        return false;
    if ((ey == 0 && ex == 0) || (ey == 0 && ex == cols - 1) ||
        (ey == rows - 1 && ex == 0) || (ey == rows - 1 && ex == cols - 1))
        return false;

    // 四邊不可為空白路徑
    for (int x = 0; x < cols; x++)
    {
        if (lines[0][x] == ' ' || lines[rows - 1][x] == ' ')
            return false;
    }
    for (int y = 0; y < rows; y++)
    {
        if (lines[y][0] == ' ' || lines[y][cols - 1] == ' ')
            return false;
    }

    *start_x = sx;
    *start_y = sy;
    *end_x = ex;
    *end_y = ey;
    return true;
}
