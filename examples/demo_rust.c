/*
 * demo_rust.c — Rust 风格伪装示例
 *
 * 编译： cc -std=c99 -Wall -Wextra -Werror -I../include demo_rust.c -o demo_rust
 * 运行： ./demo_rust
 *
 * 红线：`if` / `else` / `while` / `for` / `return` 是 C 原生关键字，
 *       预处理器**无法**替换它们，所以这里老老实实写 C。
 */

#include "jiahao_rust.h"

fn int add(int a, int b) {
    return a + b;
}

fn int main(void) {
    let_mut(int, x, 10);
    let(int, y, 20);
    let_mut(int, sum, add(x, y));

    println("x   = %d", x);
    println("y   = %d", y);
    println("sum = %d", sum);

    /* if / else 无法伪装，这就是"该语言的特殊语法"（笑） */
    if (sum > 25) {
        println("sum 大于 25");
    } else {
        println("sum 不大于 25");
    }

    return 0;
}
