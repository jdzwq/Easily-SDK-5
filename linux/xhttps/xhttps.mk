CC = g++
CFLAGS = -g -Wall

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
NET_PATH = ~/Easily-sdk-5/api/net_srv
LOC_PATH = .
OUT_PATH = ../sbin

SRCS = $(NET_PATH)/srvcert.cc $(NET_PATH)/srvlog.cc $(NET_PATH)/xhttps.cc $(LOC_PATH)/xhttpsMain.cc
OBJS = $(patsubst %.cc, %.o, $(SRCS))
TARGET = $(OUT_PATH)/xhttps

%.o : $(LOC_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(NET_PATH)

%.o : $(NET_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(NET_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxdl -lxds
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
	sudo chmod +x $(SRV_PATH)/xhttps;

.PHONY : clean
clean:
	-rm -f $(OBJS)