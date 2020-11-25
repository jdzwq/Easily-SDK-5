CC = g++
CFLAGS = -g -Wall

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
SRC_PATH = ~/Easily-sdk-5/xDump
OUT_PATH = ~/Easily-app-5/linux/bin

LIBS = -L $(LIB_PATH) -lmded -ldicm -L $(LIB_PATH) -lxds -lxdl
DIRS = $(SRC_PATH)/*.cc
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.cc, %.o, $(SRCS))
TARGET = $(OUT_PATH)/xDump

%.o : $(SRC_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(SRC_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

.PHONY : clean
clean:
	-rm -f $(OBJS)
