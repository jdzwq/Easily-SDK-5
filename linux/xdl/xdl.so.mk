CC = gcc
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ../../include
SRC_PATH = ../../xdl
SUB_PATH = ../../xdl/linux
OUT_PATH = ../sbin/api

ARLIB = -lm -L$(LIB_PATH) -lacp -lbmp -ljpg -lzlib -lpng -lqrcode -lxdp

DIRS = $(wildcard $(SRC_PATH)/*.c $(SUB_PATH)/*.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libxdl.so.1.0

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(SRC_PATH) -L $(LIB_PATH)

%.o : $(SUB_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(SRC_PATH) -L $(LIB_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) $(ARLIB) -lxdk
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

install:
	if ! test -d $(SRV_PATH); then \
	sudo mkdir $(SRV_PATH); \
	fi
	if ! test -d $(SRV_PATH)/api; then \
	sudo mkdir $(SRV_PATH)/api; \
	fi

	sudo cp -f $(TARGET) $(SRV_PATH)/api;
	sudo chmod +x $(SRV_PATH)/api/libxdl.so.1.0;
	sudo rm -f $(LNK_PATH)/libxdl*;
	sudo ln -bs $(SRV_PATH)/api/libxdl.so.1.0 $(LNK_PATH)/libxdl.so;

.PHONY : clean
clean:
	-rm -f $(OBJS)
