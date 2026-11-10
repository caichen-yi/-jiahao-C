/*
 * jiahao_go.h — 把普通 C 伪装成 Go 风格的宏库
 *
 * 作者：嘉豪（JiaHao）· MIT License
 *
 * 设计约束同 jiahao_rust.h：
 *   - 声明宏不带尾分号
 *   - 打印宏不用 `##__VA_ARGS__`（非标准扩展）
 *   - 类型用 typedef 而非 #define
 */

#ifndef JIAHAO_GO_H
#define JIAHAO_GO_H

#include <stdio.h>
#include <stdbool.h>

/* ── 变量声明 ─────────────────────────────────────────────── */
/*   var(i32, n, 5);   →   int n = (5);                        */
#define var(type, name, val) type name = (val)

/* ── 函数装饰 ─────────────────────────────────────────────── */
/*   展开为空：`func i32 square(i32 v)` → `i32 square(i32 v)`  */
#define func

/* ── 打印 ─────────────────────────────────────────────────── */
#define fmt_Println(...) do { printf(__VA_ARGS__); printf("\n"); } while (0)
#define fmt_Printf(...)  printf(__VA_ARGS__)

/* ── 类型别名 ─────────────────────────────────────────────── */
/*   用 typedef：`string a, b;` 两个都是指针，不会踩 C 的声明符坑 */
typedef const char *string;
typedef int         i32;
typedef long long   i64;
typedef float       f32;
typedef double      f64;
/*   bool / true / false 直接用 <stdbool.h> 提供的，无需伪装       */

#endif /* JIAHAO_GO_H */
