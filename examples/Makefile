# 编译并运行全部示例
#
# POSIX / CI：   make -C examples run
# Windows：     mingw32-make -C examples run   （需先安装 MinGW-w64 或 MSYS2）
#
# 想换编译器 / 标准：
#   make -C examples CC=clang CFLAGS="-std=c17 -O2 -Wall -Wextra -Werror -I../include" run

CC     ?= cc
CFLAGS ?= -std=c99 -O1 -Wall -Wextra -Werror -I../include

PROGS = demo_rust demo_go demo_py

.PHONY: all run clean

all: $(PROGS)

$(PROGS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

run: all
	@for p in $(PROGS); do echo "--- $$p ---"; ./$$p; done

clean:
	-rm -f $(PROGS) demo_rust.exe demo_go.exe demo_py.exe
