CC = gcc
CFLAGS = -g -Wall -fPIC

SYS_PATH = /usr/local
LIB_PATH = ../lib
INC_PATH = ../../include
SRC_PATH = ../../xds
OUT_PATH = ~/Easily-app-5/linux/sbin/api

DIRS = $(wildcard $(SRC_PATH)/*.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libxds.so.1.0

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(SRC_PATH) -L $(LIB_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxdl
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

install: 
	if [[! -d "$(SYS_PATH)/lib"]]; \
	then \
	sudo mkdir $(SYS_PATH)/lib; \
	fi
	sudo ln -s $(TARGET) $(SYS_PATH)/lib/libxds.so
	chmod 755 $(TARGET)

.PHONY : clean
clean:
	-rm -f $(OBJS)
