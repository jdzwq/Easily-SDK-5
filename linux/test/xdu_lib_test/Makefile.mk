CC = g++
CFLAGS = -g -Wall

LIB_PATH = /usr/local/lib
API_PATH = ~/Easily-sdk-5/linux/bin
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
	$(CC) -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxds -lxdu -lxdk
	rm -f $(OBJS)

test:
	@echo $(SRCS)
	@echo $(OBJS)

.PHONY : clean
clean:
	-rm -f $(OBJS)
