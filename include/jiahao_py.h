/*
 * jiahao_py.h — 把普通 C 伪装成 Python 风格的宏库
 *
 * 作者：嘉豪（JiaHao）· MIT License
 *
 * 注意：本头文件 **只能用于 C**。
 *   `and` / `or` / `not` 在 C++ 里是替代记号（关键字），
 *   在 C++ 中 `#define and &&` 会直接编译报错，因此下面加了拦截。
 */

#ifndef JIAHAO_PY_H
#define JIAHAO_PY_H

#ifdef __cplusplus
#error "jiahao_py.h 仅为 C 设计，请勿在 C++ 中包含（and/or/not 是 C++ 关键字）"
#endif

#include <stdio.h>
#include <stddef.h>

/* ── 函数装饰 ─────────────────────────────────────────────── */
/*   def greet(...) { ... }   →   void greet(...) { ... }      */
/*   注意：main 请保持 `int main(void)`，别写成 `def main()`，  */
/*         否则会变成 `void main()`，不符合标准且在 -Werror 下报错 */
#define def void

/* ── 打印 ─────────────────────────────────────────────────── */
/*   print("hi");            →   printf("hi"); printf("\n");   */
/*   print("x = %d", x);     →   同上，带换行                   */
/*   print_end("hi");        →   不换行                         */
#define print(...)     do { printf(__VA_ARGS__); printf("\n"); } while (0)
#define print_end(...) printf(__VA_ARGS__)

/* ── 布尔与逻辑运算符 ─────────────────────────────────────── */
/*   若已包含 <iso646.h>，这些记号已是 `&&` / `||` / `!`，      */
/*   替换列表完全相同，属于合法重定义，不会报错。               */
#define True  1
#define False 0
#define and &&
#define or  ||
#define not !

/* ── 空值 ─────────────────────────────────────────────────── */
#define None NULL

#endif /* JIAHAO_PY_H */
