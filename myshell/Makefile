# 当前目录找不到依赖/目标文件时,去指定路径查找
VPATH ?= .:

SRC := ./main.c ./myshell.c
OBJS := ./main.o ./myshell.o # 加了:前面的变量不能使用后面的变量
TARGET := myshell

CC := gcc
RM := -rm -rf # -表示命令出错.继续执行,忽略错误.
CPPFLAGS := # C预处理参数,一般设置I
CFLAGS := -Wall -O2 # 编译器参数,C使用
override CFLAGS += $(patsubst %,-I%,$(subst :, ,$(VPATH))) # 设置头文件搜索路径(-I)
LDFLAGS :=  # 链接器参数,如ld

rely := $(SRC:.c=.d) # 生成.d文件

####################
# $< : 第一个依赖文件
# $^ : 所有依赖文件(去除重复,$+不去除)
# $@ : 所有目标集合
# D/F :目录/文件 ,如$(<D)
####################
.PHONY: all cleantar clean cleanobj cleangen

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# 自动生成依赖
%.d: %.c
	@set -e; rm -f $@ &> /dev/null; # 设置错误检查,若下面有语句执行错误则直接退出, 删除旧的.d文件\
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; # 生成依赖文件 \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; # 替换临时文件 \
	rm -f $@.$$$$ &> /dev/null # 删除临时文件

# 替换
-include $(rely)

#######################
# 伪目标同样也可成为依赖
cleanall: clean cleanbin

cleanbin:
	$(RM) $(TARGET)

clean:
	$(RM) $(OBJS) $(rely)
