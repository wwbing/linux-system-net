#获取当前目录下所有的.c文件
SRC=$(wildcard ./*.c)

#将SRC中所有出现.c的替换成.o
OBJS=$(patsubst %.c, %.o, $(SRC))

TARGET=test


#这些变量不能单独使用 必须在命令中使用
#$@	表示目标
#$^	表示所有的依赖
#$<	表示第一个依赖

$(TARGET):$(OBJS)
	gcc $^ -o $@ 


#模式匹配 所有的.o都依赖对应的.c
#将所有的.c 生成对应的.o
%.o:%.c
	@gcc  -c $< -o $@


#声明clean为伪目标 伪目标不去判断目标文件是否存在或者已经更新 
#无条件执行命令
.PHONY:clean
clean:
	-rm -rf $(OBJS) $(TARGET) 

