CC = gcc
CFLAGS = -g -Wall -fPIC

SYS_PATH = /usr/local
LIB_PATH = ../lib
SRC_PATH = ../../xdk
SUB_PATH = ../../xdk/linux
OUT_PATH = ~/Easily-app-5/linux/sbin/api

DIRS = $(wildcard $(SRC_PATH)/*.c $(SUB_PATH)/*.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libxdk.so.1.0

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(SRC_PATH) -L $(LIB_PATH)

%.o : $(SUB_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(SRC_PATH) -L $(LIB_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) -lm -ldl -lutil -lrt -L $(LIB_PATH)
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
	sudo ln -s $(TARGET) $(SYS_PATH)/lib/libxdk.so
	chmod 755 $(TARGET)

.PHONY : clean
clean:
	-rm -f $(OBJS)
