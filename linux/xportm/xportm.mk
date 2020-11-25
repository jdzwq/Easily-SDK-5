CC = g++
CFLAGS = -g -Wall

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
NET_PATH = ~/Easily-sdk-5/api/net_srv
LOC_PATH = .
OUT_PATH = ~/Easily-app-5/linux/sbin

LIBS = -L $(LIB_PATH) -lxds -lxdl
DIRS = $(LOC_PATH)/xportmMain.cc $(NET_PATH)/xportm.cc
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.cc, %.o, $(SRCS))
MODULE = xportm
TARGET = $(OUT_PATH)/$(MODULE)

%.o : $(LOC_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(NET_PATH)

%.o : $(NET_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(NET_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

install:
	if ! test -d $(SRV_PATH); then \
	sudo mkdir $(SRV_PATH); \
	fi

	sudo cp -f $(TARGET) $(SRV_PATH);
	sudo chmod +x $(SRV_PATH)/$(MODULE);

uninstall:
	sudo rm -f $(SRV_PATH)/$(MODULE)

.PHONY : clean
clean:
	-rm -f $(OBJS)
