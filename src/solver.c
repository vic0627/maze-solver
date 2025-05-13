#include "../include/solver.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

// 根據演算法名稱決定 plugin 路徑 (e.g., "dfs" -> ".build/plugins/libdfs.so")
static const char *plugin_filename(const char *algo)
{
    static char pathbuf[256];
    // 對應 Makefile 輸出 .build/plugins/lib<algo>.so
    snprintf(pathbuf, sizeof(pathbuf), ".build/plugins/lib%s.so", algo);
    return pathbuf;
}

// 動態載入 solver plugin (.so)，並回傳 Solver 指標
// algo: 演算法名稱
// dl_handle: 輸出，用於後續卸載
Solver *load_solver(const char *algo, void **dl_handle)
{
    const char *plugin_path = plugin_filename(algo);
    void *handle = dlopen(plugin_path, RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "Error loading plugin '%s': %s\n", plugin_path, dlerror());
        return NULL;
    }
    dlerror(); // 清除舊錯誤

    Solver *(*factory)(void) = (Solver * (*)(void)) dlsym(handle, "create_solver");
    char *err = dlerror();
    if (err)
    {
        fprintf(stderr, "Error finding symbol create_solver: %s\n", err);
        dlclose(handle);
        return NULL;
    }

    Solver *solver = factory();
    if (!solver)
    {
        fprintf(stderr, "Factory returned NULL for plugin '%s'\n", plugin_path);
        dlclose(handle);
        return NULL;
    }
    *dl_handle = handle;
    return solver;
}

// 卸載 plugin
void unload_solver_plugin(void *dl_handle)
{
    if (dl_handle)
        dlclose(dl_handle);
}