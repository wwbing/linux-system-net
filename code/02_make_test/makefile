src=$(wildcard ./src/*.c)	
obj=$(patsubst %.c, %.o, $(src))

ALL:hello


hello:$(obj) hello.o
	gcc $(obj) hello.o -o $@


hello.o:hello.c
	gcc -c $< -o $@
add.o:add.c
	gcc -c src/add.c -o src/add.o
sub.o:sub.c
	gcc -c src/sub.c -o src/sub.o
mul.o:mul.c
	gcc -c src/mul.c -o src/mul.o


# make clean 主动触发，执行clean的命令
clean:
	rm $(obj) hello hello.o
