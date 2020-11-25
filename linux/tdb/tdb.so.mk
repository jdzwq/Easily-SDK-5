CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

INC_TDB = /usr/include/tdb
LIB_TDB = /usr/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
SRC_PATH = ~/Easily-sdk-5/tdb
OUT_PATH = ~/Easily-app-5/linux/sbin/api

LIBS = -L $(LIB_PATH) -lxds -lxdl
DIRS = $(wildcard $(SRC_PATH)/*.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
MODULE = libtdb.so
TARGET = $(OUT_PATH)/$(MODULE).1.0

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(INC_TDB)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) $(LIBS)
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

uninstall:
	sudo rm -r $(LNK_PATH)/$(MODULE)*;
	sudo rm -f $(SRV_PATH)/api/$(MODULE).1.0
	
.PHONY : clean
clean:
	-rm -f $(OBJS)
