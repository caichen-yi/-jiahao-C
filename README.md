# 嘉豪多语言伪装引擎 · JiaHao Polyglot Engine

用 C 预处理器把普通 C 代码伪装成 **Rust** / **Go** / **Python** 的风格。

> ⚠️ 这是一个**玩笑项目**。它的价值在于「技术上有点意思」，不在于「工程上有多有用」。
> 但请放心：仓库里的示例**真的能编译、真的能跑**，不是伪代码。CI 会在 gcc / clang × C99 / C11 / C17 / C23 下全部验证。

| 伪装目标 | 头文件 | 写出来长这样 |
|---|---|---|
| Rust | [`include/jiahao_rust.h`](include/jiahao_rust.h) | `let_mut(int, x, 10);` |
| Go | [`include/jiahao_go.h`](include/jiahao_go.h) | `var(i32, n, 5);` |
| Python | [`include/jiahao_py.h`](include/jiahao_py.h) | `print("hello, %s", who);` |

---

## 快速开始

```bash
git clone https://github.com/<your-account>/jiahao-polyglot.git
cd jiahao-polyglot/examples
make run
```

预期输出：

```
--- demo_rust ---
x   = 10
y   = 20
sum = 30
sum 大于 25
--- demo_go ---
who  = jiahao
n    = 5
n^2  = 25
平方后大于 20
--- demo_py ---
hello, jiahao
v * 2 = 42
这行没有换行 —— 用 print_end 拼起来的
```

Windows 下如果没有 `make`，可以手动编译：

```powershell
cd examples
gcc -std=c99 -Wall -Wextra -Werror -I..\include demo_rust.c -o demo_rust.exe
.\demo_rust.exe
```

---

## 三个宏库怎么用

### Rust 风格

```c
#include "jiahao_rust.h"

fn int add(int a, int b) { return a + b; }

fn int main(void) {
    let_mut(int, x, 10);      // → int x = (10);
    let(int, y, 20);          // → const int y = (20);
    let_mut(int, sum, add(x, y));
    println("sum = %d", sum); // → printf("sum = %d", sum); printf("\n");
    return 0;
}
```

可用记号：`let` `let_mut` `fn` `println` `print` `borrow` `deref` `str`

### Go 风格

```c
#include "jiahao_go.h"

func i32 square(i32 v) { return v * v; }

func i32 main(void) {
    var(i32, n, 5);           // → int n = (5);
    string who = "jiahao";    // → const char *who = "jiahao";
    fmt_Println("n = %d", n);
    return 0;
}
```

可用记号：`var` `func` `fmt_Println` `fmt_Printf` `string` `i32` `i64` `f32` `f64` `bool`

### Python 风格

```c
#include "jiahao_py.h"

def greet(const char *who) {   // → void greet(const char *who)
    print("hello, %s", who);
}

int main(void) {
    if (True and not False) {  // → if (1 && !0)
        greet("jiahao");
    }
    return 0;
}
```

可用记号：`def` `print` `print_end` `True` `False` `and` `or` `not` `None`

---

## 作为 AI Agent Skill 使用

仓库根目录的 [`SKILL.md`](SKILL.md) 让这个项目可以**直接当 skill 用**。

以 [DeepSeek Harness](https://github.com/deepseek-ai) 为例，克隆到共享 skill 目录即可自动加载：

```powershell
git clone https://github.com/<your-account>/jiahao-polyglot.git "$env:USERPROFILE\.agents\skills\jiahao-polyglot"
```

之后在对话里说「把这段 C 代码伪装成 Rust」，agent 就会自动套用这套宏库和规范。

`SKILL.md` 也可以直接复制到 Coze / Dify / ChatGPT 的自定义指令里使用。

---

## 伪装能力的边界（跨不过去的红线）

预处理器只能做**记号替换**，以下这些东西在任何宏库里都骗不过去：

| 跨不过去的 | 原因 |
|---|---|
| `if` `else` `while` `for` `return` `switch` `break` | C 原生关键字，不是标识符 |
| 缩进代替大括号 | C 语法靠 `{}`，与空白无关 |
| Python 的 `elif` | 无法把 `elif` 展开成 `else if` 后仍保持语句结构 |
| Go 的 `if x > 0 {` 不要括号 | C 强制要求括号 |
| 真正的所有权 / 生命周期 / GC | 这是语言语义，不是语法糖 |
| 分号省略 | C 语句必须以 `;` 结尾 |

所以你会看到示例里 `if (sum > 25) { ... }` 还是老老实实的 C —— 这就是「该语言的特殊语法」（笑）。

---

## 设计说明：为什么不用最直觉的写法

这版在实现上踩过并修掉了几个真坑，值得单独说：

### 1. `#define fn int` 是错的

原版这么写，于是 `fn int main()` 展开成 `int int main()` —— 直接编译失败。

正确做法是让 `fn` 展开为**空**：

```c
#define fn          // 对：fn int add(...) → int add(...)
```

### 2. 声明宏不能自带尾分号

```c
#define let_mut(type, name, val) type name = val;   // 错
```

调用点 `let_mut(int, x, 10);` 会展开成 `int x = 10;;`。
更糟的是遇到 `if (c) let_mut(...)` 会变成 `if (c) int x = 10;;`，语法错误。

正确做法是宏里不带分号，由调用点提供：

```c
#define let_mut(type, name, val) type name = (val)  // 对
```

### 3. 不要用 `##__VA_ARGS__`

原版写 `printf(fmt, ##__VA_ARGS__)`。`##__VA_ARGS__` 是 **GCC 扩展**，MSVC、armcc、部分嵌入式编译器行为不同甚至不支持。

改成正统写法，把格式串一起丢进变参里：

```c
#define println(...) do { printf(__VA_ARGS__); printf("\n"); } while (0)
```

顺带用 `do { } while (0)` 保证宏在 `if` 里也安全。

### 4. 类型别名要用 `typedef`，别用 `#define`

```c
#define string char*        // 错：string a, b; → char* a, b; 只有 a 是指针
typedef const char *string; // 对：string a, b; 两个都是指针
```

这是 C 声明符的经典坑，`typedef` 才是正确工具。

### 5. Python 风格只能用于 C

`and` / `or` / `not` 在 C++ 里是**替代记号（关键字）**，`#define and &&` 在 C++ 中会直接报错。
所以 `jiahao_py.h` 里加了 `#ifdef __cplusplus #error` 拦截。

---

## 项目结构

```
.
├── SKILL.md                     # AI Agent skill 定义
├── README.md
├── LICENSE                      # MIT
├── include/
│   ├── jiahao_rust.h
│   ├── jiahao_go.h
│   └── jiahao_py.h
├── examples/
│   ├── demo_rust.c
│   ├── demo_go.c
│   ├── demo_py.c
│   └── Makefile
├── docs/
│   └── original-prompt.md       # 最初的版本，保留作对照
└── .github/workflows/ci.yml     # gcc/clang × C99/C11/C17/C23
```

---

## 开发

```bash
make -C examples run     # 编译 + 运行
make -C examples clean   # 清理
```

换编译器或标准：

```bash
make -C examples CC=clang CFLAGS="-std=c17 -O2 -Wall -Wextra -Werror -I../include" run
```

---

## 免责声明

> ⚠️ **嘉豪警告**
>
> 本项目依赖高度抽象的预处理器逻辑，产出的代码**完全可以通过 C 编译器**。
> 但请在提交到公司代码库之前三思 —— 以免被同事或主管暴打。
>
> 本项目仅供娱乐与教学。作者不对任何因使用本项目导致的代码评审事故、
> 绩效面谈、或被逐出项目组承担责任。

---

## English

**JiaHao Polyglot Engine** — a joke project that uses the C preprocessor to disguise plain C as Rust, Go, or Python.

The macros are technically sound: examples compile cleanly under `-Wall -Wextra -Werror` on gcc and clang across C99/C11/C17/C23 (see CI). No `##__VA_ARGS__` extensions, no broken type macros, no `int int main()`.

Clone it into your agent's skill directory and `SKILL.md` makes it directly usable as an AI agent skill.

**This is for fun.** Do not ship it.

---

## License

[MIT](LICENSE) © 2026 JiaHao (ycc)
