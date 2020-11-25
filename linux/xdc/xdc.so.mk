CC = gcc
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ../../include
SRC_PATH = ../../xdc
SUB_PATH = ../../xdc/linux
OUT_PATH = ~/Easily-app-5/linux/sbin/api

LIBS = -lm -L $(LIB_PATH) -lxds -lxdk -lxdu -lxdl
DIRS = $(wildcard $(SRC_PATH)/*.c $(SUB_PATH)/*.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
MODULE = libxdc.so
TARGET = $(OUT_PATH)/$(MODULE).1.0

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(SRC_PATH)

%.o : $(SUB_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(SRC_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) $(LIBS)
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
	sudo chmod +x $(SRV_PATH)/api/$(MODULE).1.0;
	sudo rm -f $(LNK_PATH)/libxdc*;
	sudo ln -bs $(SRV_PATH)/api/$(MODULE).1.0 $(LNK_PATH)/$(MODULE);

uninstall:
	sudo rm -r $(LNK_PATH)/$(MODULE)*;
	sudo rm -f $(SRV_PATH)/api/$(MODULE).1.0
	
.PHONY : clean
clean:
	-rm -f $(OBJS)
