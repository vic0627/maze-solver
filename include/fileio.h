#ifndef FILEIO_H
#define FILEIO_H

#include <stdbool.h>

// 讀取文字檔每一行到字串陣列中
// filepath: 檔案路徑
// lines: 輸出，動態配置的行字串指標陣列
// rows: 輸出，行數
// cols: 輸出，每行的長度（假設固定長度）
// return: 成功為 true，否則 false
bool read_maze_file(const char *filepath, char ***lines, int *rows, int *cols);

// 釋放由 read_maze_file 配置的 lines
void free_lines(char **lines, int rows);

// 符號合法性驗證
// 只允許 '*', 's', 'e', ' ' 四種符號
bool validate_symbols(char **lines, int rows, int cols);

// 結構合法性驗證：
// - 起點 s 與終點 e 各一
// - 起/終 點不在四角
// - 四邊不含空白 ' ' 路徑
bool validate_structure(char **lines, int rows, int cols,
                        int *start_x, int *start_y,
                        int *end_x, int *end_y);

#endif