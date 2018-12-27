CC = g++
CFLAGS = -g -Wall

SYS_PATH = /usr/local
LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
NET_PATH = ~/Easily-sdk-5/api/net_srv
LOC_PATH = .
OUT_PATH = ~/Easily-app-5/linux/sbin

DIRS = $(wildcard $(LOC_PATH)/*.cc $(NET_PATH)/*.cc)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.cc, %.o, $(SRCS))
TARGET = $(OUT_PATH)/xportd

%.o : $(LOC_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(NET_PATH) -L $(LIB_PATH)

%.o : $(NET_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(NET_PATH) -L $(LIB_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxdl -lxds
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

install: 
	if [[! -d "$(SYS_PATH)/sbin"]]; \
	then \
	sudo mkdir $(SYS_PATH)/sbin; \
	fi
	sudo ln -s $(TARGET) $(SYS_PATH)/sbin/xportd
	chmod 755 $(TARGET)

.PHONY : clean
clean:
	-rm -f $(OBJS)
