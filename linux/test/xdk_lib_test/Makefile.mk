CC = g++
CFLAGS = -g -Wall

LIB_PATH = /usr/local/lib
INC_PATH = ~/Easily-sdk-5/include
SRC_PATH = ~/Easily-sdk-5/test/xdk_lib_test
OUT_PATH = ~/Easily-app-5/linux/bin

SRCS = $(SRC_PATH)/main.cpp
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
TARGET = $(OUT_PATH)/xdk_lib_test

$(SRC_PATH)%.o : $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxds -lxdk
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

.PHONY : clean
clean:
	-rm -f $(OBJS)
