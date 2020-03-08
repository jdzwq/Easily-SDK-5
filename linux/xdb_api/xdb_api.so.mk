CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
NET_PATH = ~/Easily-sdk-5/api/xdb_api
OUT_PATH = ../sbin/api

INS_PATH = ~/Easily-sdk-5/linux/setup

DIRS = $(wildcard $(NET_PATH)/*.cc)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.cc, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libxdb_api.so.1.0

%.o : $(NET_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(NET_PATH) -L $(LIB_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxdp -lxdl -lxds
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

install:
	sudo cp -f $(TARGET) $(SRV_PATH)/api;
	sudo chmod +x $(SRV_PATH)/api/libxdb_api.so.1.0;
	sudo rm -f $(LNK_PATH)/libxdb_api*;
	sudo ln -bs $(SRV_PATH)/api/libxdb_api.so.1.0 $(LNK_PATH)/libxdb_api.so;

	sudo cp -f $(INS_PATH)/cfg/xdb.config $(SRV_PATH)/cfg;

	if ! test -d $(SRV_PATH)/lic/xdb; then \
	sudo mkdir $(SRV_PATH)/lic/xdb; \
	fi
	sudo cp -rf $(INS_PATH)/lic/xdb $(SRV_PATH)/lic;

	if ! test -d $(SRV_PATH)/xdb; then \
	sudo mkdir $(SRV_PATH)/xdb; \
	fi
	sudo cp -rf $(INS_PATH)/xdb $(SRV_PATH);

.PHONY : clean
clean:
	-rm -f $(OBJS)
