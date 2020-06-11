CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include

SRC_PATH = ~/Easily-sdk-5/iot/telecom
LOC_PATH = ~/Easily-sdk-5/iot

OUT_PATH = ../sbin/api

INS_PATH = ~/Easily-sdk-5/linux/setup

DIRS = $(wildcard $(SRC_PATH)/iot_telecom.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
MODULE = libiot_telecom.so
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

	if ! test -d $(SRV_PATH)/iot/telecom; then \
	sudo mkdir $(SRV_PATH)/iot/telecom; \
	fi
	sudo cp -rf $(INS_PATH)/iot/telecom $(SRV_PATH)/iot;

.PHONY : clean
clean:
	-rm -f $(OBJS)
