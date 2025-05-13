#ifndef SOLVER_H
#define SOLVER_H

#include <stdbool.h>
#include "maze.h"

// Strategy Pattern: 定義 Solver 介面
typedef struct Solver
{
    // 初始化演算法 (設定 Maze 狀態、結構等)
    void (*init)(Maze *m);
    // 下一步探索: 更新老鼠位置到 (*x,*y)，
    // 回傳 true 表示可繼續，false 表示已完成或無路
    bool (*next_step)(Maze *m, int *x, int *y);
    // 資源釋放
    void (*cleanup)(void);
} Solver;

// Plugin 工廠函式 (由各 plugin 實作並 export)
// 例如在 dfs.c:
//   Solver* create_solver(void) {
//       static Solver s = { dfs_init, dfs_next, dfs_cleanup };
//       return &s;
//   }
Solver *create_solver(void);

// 動態載入與卸載 plugin
// algo: 演算法名稱, e.g. "dfs" 對應 libdfs.so
// dl_handle: 用於後續卸載
Solver *load_solver(const char *algo, void **dl_handle);
void unload_solver_plugin(void *dl_handle);

#endif