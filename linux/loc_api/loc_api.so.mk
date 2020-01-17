CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
NET_PATH = ~/Easily-sdk-5/api/loc_api
OUT_PATH = ../sbin/api

DIRS = $(wildcard $(NET_PATH)/*.cc)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.cc, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libloc_api.so.1.0

%.o : $(NET_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(NET_PATH) -L $(LIB_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxdl -lxds
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
	sudo chmod +x $(SRV_PATH)/api/libloc_api.so.1.0;
	sudo ln -bs $(SRV_PATH)/api/libloc_api.so.1.0 $(LNK_PATH)/libloc_api.so;

.PHONY : clean
clean:
	-rm -f $(OBJS)
