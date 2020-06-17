CC = g++
CFLAGS = -g -Wall

LIB_PATH = ~/Easily-sdk-5/linux/lib
INC_PATH = ~/Easily-sdk-5/include
SRC_PATH = ~/Easily-sdk-5/test/xdu_lib_test
OBJ_PATH = ~/Easily-sdk-5/test/xdu_lib_test
OUT_PATH = ~/Easily-sdk-5/linux/bin

SRCS = $(SRC_PATH)/main.cpp
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
TARGET = $(OUT_PATH)/xdu_lib_test

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -o $(TARGET) $(OBJS) -lxds -lxdu -L $(LIB_PATH)
	rm -f $(OBJS)

test:
	@echo $(SRCS)
	@echo $(OBJS)

.PHONY : clean
clean:
	-rm -f $(OBJS)
