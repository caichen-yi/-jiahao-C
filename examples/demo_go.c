/*
 * demo_go.c — Go 风格伪装示例
 *
 * 编译： cc -std=c99 -Wall -Wextra -Werror -I../include demo_go.c -o demo_go
 * 运行： ./demo_go
 */

#include "jiahao_go.h"

func i32 square(i32 v) {
    return v * v;
}

func i32 main(void) {
    var(i32, n, 5);
    var(i32, sq, square(n));
    string who = "jiahao";

    fmt_Println("who  = %s", who);
    fmt_Println("n    = %d", n);
    fmt_Println("n^2  = %d", sq);

    /* Go 的 if 不需要括号，但 C 需要 —— 这条红线跨不过去 */
    if (sq > 20) {
        fmt_Println("平方后大于 20");
    }

    return 0;
}
