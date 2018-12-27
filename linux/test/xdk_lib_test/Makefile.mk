CC = g++
CFLAGS = -g -Wall

SYS_PATH = /usr/local
LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
LOC_PATH = .
OUT_PATH = ~/Easily-app-5/linux/bin

SRCS = $(LOC_PATH)/main.cpp
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
TARGET = $(OUT_PATH)/xdk_lib_test

%.o : $(LOC_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxdk
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

.PHONY : clean
clean:
	-rm -f $(OBJS)
