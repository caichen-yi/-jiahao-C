/*
 * demo_py.c — Python 风格伪装示例
 *
 * 编译： cc -std=c99 -Wall -Wextra -Werror -I../include demo_py.c -o demo_py
 * 运行： ./demo_py
 *
 * 注意：靠缩进来表达代码块是 Python 的灵魂，而 C 靠大括号 ——
 *       这一层没有任何宏能骗过去，属于"伪装的上限"。
 */

#include "jiahao_py.h"

def greet(const char *who) {
    print("hello, %s", who);
}

def twice(int v) {
    print("v * 2 = %d", v * 2);
}

int main(void) {
    if (True and not False) {
        greet("jiahao");
        twice(21);
    }

    print_end("这行没有换行");
    print_end(" —— 用 print_end 拼起来的\n");

    return 0;
}
