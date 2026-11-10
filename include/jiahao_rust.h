/*
 * jiahao_rust.h — 把普通 C 伪装成 Rust 风格的宏库
 *
 * 作者：嘉豪（JiaHao）· MIT License
 *
 * 设计约束（重要）：
 *   1. 变量声明宏 **不带尾分号**，由调用点自己写 `;`
 *      —— 否则 `let_mut(int, x, 10);` 会展开成 `int x = 10;;`
 *   2. 打印宏用 `printf(__VA_ARGS__)`，**不使用 `##__VA_ARGS__`**
 *      —— `##__VA_ARGS__` 是 GCC 扩展，MSVC / armcc / 部分嵌入式编译器行为不同
 *   3. 类型别名用 `typedef` 而不是 `#define`
 *      —— `#define string char*` 会让 `string a, b;` 里的 b 变成 char 而非指针
 */

#ifndef JIAHAO_RUST_H
#define JIAHAO_RUST_H

#include <stdio.h>

/* ── 变量声明 ─────────────────────────────────────────────── */
/*   let_mut(int, x, 10);   →   int x = (10);                  */
/*   let(int, y, 20);       →   const int y = (20);            */
#define let_mut(type, name, val) type name = (val)
#define let(type, name, val)     const type name = (val)

/* ── 函数装饰 ─────────────────────────────────────────────── */
/*   展开为空，所以 `fn int add(int a, int b)` → `int add(int a, int b)`  */
/*   （原版写成 `#define fn int`，会让 `fn int main()` 变成             */
/*     `int int main()`，直接编译失败 —— 这里修正掉）                    */
#define fn

/* ── 打印 ─────────────────────────────────────────────────── */
/*   println("x = %d", x);   →   printf("x = %d", x); printf("\n");   */
#define println(...) do { printf(__VA_ARGS__); printf("\n"); } while (0)
#define print(...)   printf(__VA_ARGS__)

/* ── 借用 / 解引用 ────────────────────────────────────────── */
#define borrow &
#define deref  *

/* ── 类型别名 ─────────────────────────────────────────────── */
typedef const char *str;

#endif /* JIAHAO_RUST_H */
