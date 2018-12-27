CC = gcc
CFLAGS = -g -Wall -fPIC

SYS_PATH = /usr/local
LIB_PATH = ../lib
INC_PATH = ../../include
SRC_PATH = ../../xdl
SUB_PATH = ../../xdl/linux
OUT_PATH = ~/Easily-app-5/linux/sbin/api

ARLIB = -lm -L$(LIB_PATH) -lacp -lbmp -ljpg -lzlib -lpng -lqrcode

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
	if [! -d $(SYS_PATH)/lib]; \
	then \
	sudo mkdir $(SYS_PATH)/lib; \
	fi
	sudo ln -s $(TARGET) $(SYS_PATH)/lib/libxdl.so
	chmod 755 $(TARGET)

.PHONY : clean
clean:
	-rm -f $(OBJS)
