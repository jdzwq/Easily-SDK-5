CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
API_PATH = ../sbin/api
INC_PATH = ~/Easily-sdk-5/include
SRC_PATH = ~/Easily-sdk-5/sms/aliyun
LOC_PATH = ~/Easily-sdk-5/sms
INI_PATH = ~/Easily-sdk-5/linux/setup
OUT_PATH = ../sbin/api

LIBS = -L $(API_PATH) -lxds -lxdl
DIRS = $(wildcard $(SRC_PATH)/sms_aliyun.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
MODULE = libsms_aliyun.so
TARGET = $(OUT_PATH)/$(MODULE).1.0

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(LOC_PATH) 

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

	if ! test -d $(SRV_PATH)/sms/aliyun; then \
	sudo mkdir $(SRV_PATH)/sms/aliyun; \
	fi
	sudo cp -rf $(INI_PATH)/sms/aliyun $(SRV_PATH)/sms;

uninstall:
	sudo rm -r $(LNK_PATH)/$(MODULE)*;
	sudo rm -f $(SRV_PATH)/api/$(MODULE).1.0
	
.PHONY : clean
clean:
	-rm -f $(OBJS)
