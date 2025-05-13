#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "maze.h"
#include "solver.h"
#include "animation.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <maze_file> <algorithm>\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char *maze_path = argv[1];
    const char *algo_name = argv[2];

    // 1. Read and validate maze file
    char **lines = NULL;
    int rows = 0, cols = 0;
    if (!read_maze_file(maze_path, &lines, &rows, &cols))
    {
        fprintf(stderr, "Error: Failed to read maze file '%s'\n", maze_path);
        return EXIT_FAILURE;
    }
    if (!validate_symbols(lines, rows, cols))
    {
        fprintf(stderr, "Error: Maze contains invalid symbols\n");
        free_lines(lines, rows);
        return EXIT_FAILURE;
    }
    int start_x, start_y, end_x, end_y;
    if (!validate_structure(lines, rows, cols, &start_x, &start_y, &end_x, &end_y))
    {
        fprintf(stderr, "Error: Maze structure invalid\n");
        free_lines(lines, rows);
        return EXIT_FAILURE;
    }

    // 2. Populate Maze struct
    Maze maze;
    maze.width = cols;
    maze.height = rows;
    maze.grid = malloc(rows * sizeof(char *));
    if (!maze.grid)
    {
        perror("malloc");
        free_lines(lines, rows);
        return EXIT_FAILURE;
    }
    for (int y = 0; y < rows; y++)
    {
        maze.grid[y] = malloc(cols + 1);
        if (!maze.grid[y])
        {
            perror("malloc");
            // free previous rows
            for (int i = 0; i < y; i++)
                free(maze.grid[i]);
            free(maze.grid);
            free_lines(lines, rows);
            return EXIT_FAILURE;
        }
        strcpy(maze.grid[y], lines[y]);
    }
    maze.start_x = start_x;
    maze.start_y = start_y;
    maze.end_x = end_x;
    maze.end_y = end_y;
    free_lines(lines, rows);

    // 3. Load solver plugin
    void *dl_handle = NULL;
    Solver *solver = load_solver(algo_name, &dl_handle);
    if (!solver)
    {
        fprintf(stderr, "Error: Unsupported algorithm '%s'\n", algo_name);
        free_maze(&maze);
        return EXIT_FAILURE;
    }

    // 4. Initialize solver
    solver->init(&maze);

    // 5. Animate solving process
    int x = maze.start_x;
    int y = maze.start_y;
    animate_step(&maze, x, y); // initial position
    while (solver->next_step(&maze, &x, &y))
    {
        animate_step(&maze, x, y);
    }
    // 顯示最終位置 (如終點)
    animate_step(&maze, x, y);

    // 6. Cleanup
    solver->cleanup();
    unload_solver_plugin(dl_handle);
    free_maze(&maze);

    return EXIT_SUCCESS;
}
