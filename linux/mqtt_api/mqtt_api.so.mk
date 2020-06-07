CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include

SRC_PATH = ~/Easily-sdk-5/api/mqtt_api
OUT_PATH = ../sbin/api

INS_PATH = ~/Easily-sdk-5/linux/setup

DIRS = $(wildcard $(SRC_PATH)/*.cc)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.cc, %.o, $(SRCS))
MODULE = libmqtt_api.so
TARGET = $(OUT_PATH)/$(MODULE).1.0

%.o : $(SRC_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -L $(LIB_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxds -lxdl -lhdfs
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

	sudo cp -f $(INS_PATH)/cfg/mqtt.config $(SRV_PATH)/cfg;

	if ! test -d $(SRV_PATH)/mqtt; then \
	sudo mkdir $(SRV_PATH)/mqtt; \
	fi
	sudo cp -f $(INS_PATH)/mqtt/mqtt.ini $(SRV_PATH)/mqtt;

.PHONY : clean
clean:
	-rm -f $(OBJS)
