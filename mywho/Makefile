SRC =  main.c mywho.c
TARGET := mywho 

CC := gcc
RM = rm -rf
LOCAL_CFLAGS = -o #编译器参数
LOCAL_LDFLAGS =  #链接器参数

#############################

.PHONY:all
all: 
	$(CC) $(LOCAL_CFLAGS) $(TARGET) $(SRC)


# 显示指明伪目标,避免和文件重名
.PHONY: clean
clean:
	$(RM) *.o a.out
