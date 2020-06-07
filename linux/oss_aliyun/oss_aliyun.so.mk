CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include

SRC_PATH = ~/Easily-sdk-5/oss/aliyun
LOC_PATH = ~/Easily-sdk-5/oss

OUT_PATH = ../sbin/api

INS_PATH = ~/Easily-sdk-5/linux/setup

DIRS = $(wildcard $(SRC_PATH)/oss_aliyun.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
MODULE = liboss_aliyun.so
TARGET = $(OUT_PATH)/$(MODULE).1.0

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -L $(LIB_PATH) -I $(LOC_PATH) 

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxds -lxdl
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

install:
	sudo cp -f $(TARGET) $(SRV_PATH)/api;
	sudo chmod +x $(SRV_PATH)/api/$(MODULE).1.0;
	sudo rm -f $(LNK_PATH)/$(MODULE)*;
	sudo ln -bs $(SRV_PATH)/api/$(MODULE).1.0 $(LNK_PATH)/$(MODULE);

	if ! test -d $(SRV_PATH)/oss/aliyun; then \
	sudo mkdir $(SRV_PATH)/oss/aliyun; \
	fi
	sudo cp -rf $(INS_PATH)/oss/aliyun $(SRV_PATH)/oss;

.PHONY : clean
clean:
	-rm -f $(OBJS)
