# 老鼠走迷宮

編譯：

```bash
$ make clean && make
```

生成迷宮：

```bash
$ ./.build/maze_gen [-f file_name] [width] [height]
```

走迷宮：

```bash
$ ./.build/maze_solver <maze_file> <algorithm> [frame_delay_ms]
```