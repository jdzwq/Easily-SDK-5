CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
SRC_PATH = ~/Easily-sdk-5/api/xdb_api
INI_PATH = ~/Easily-sdk-5/linux/setup
OUT_PATH = ~/Easily-app-5/linux/sbin/api

LIBS = -L $(LIB_PATH) -lxds -lxdl
DIRS = $(wildcard $(SRC_PATH)/*.cc)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.cc, %.o, $(SRCS))
MODULE = libxdb_api.so
TARGET = $(OUT_PATH)/$(MODULE).1.0

%.o : $(SRC_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH)

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

	sudo cp -f $(INI_PATH)/cfg/xdb.config $(SRV_PATH)/cfg;

	if ! test -d $(SRV_PATH)/lic/xdb; then \
	sudo mkdir $(SRV_PATH)/lic/xdb; \
	fi
	sudo cp -rf $(INI_PATH)/lic/xdb $(SRV_PATH)/lic;

	if ! test -d $(SRV_PATH)/xdb; then \
	sudo mkdir $(SRV_PATH)/xdb; \
	fi
	sudo cp -rf $(INI_PATH)/xdb $(SRV_PATH);

uninstall:
	sudo rm -r $(LNK_PATH)/$(MODULE)*;
	sudo rm -f $(SRV_PATH)/api/$(MODULE).1.0
	
.PHONY : clean
clean:
	-rm -f $(OBJS)
