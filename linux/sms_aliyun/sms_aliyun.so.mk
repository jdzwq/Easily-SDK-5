CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include

SRC_PATH = ~/Easily-sdk-5/sms/aliyun
LOC_PATH = ~/Easily-sdk-5/sms

OUT_PATH = ../sbin/api

SRCS = $(SRC_PATH)/sms_aliyun.c
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libsms_aliyun.so.1.0

$(SRC_PATH)%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -L $(LIB_PATH) -I $(SRC_PATH) -I $(LOC_PATH) 

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxdp -lxdl
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

install:
	sudo cp -f $(TARGET) $(SRV_PATH)/api;
	sudo chmod +x $(SRV_PATH)/api/libsms_aliyun.so.1.0;
	sudo rm -f $(LNK_PATH)/libsms_aliyun*;
	sudo ln -bs $(SRV_PATH)/api/libsms_aliyun.so.1.0 $(LNK_PATH)/libsms_aliyun.so;

.PHONY : clean
clean:
	-rm -f $(OBJS)
