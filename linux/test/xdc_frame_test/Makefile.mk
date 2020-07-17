CC = g++
CFLAGS = -g -Wall

LIB_PATH = /usr/local/lib
INC_PATH = ~/Easily-sdk-5/include
SRC_PATH = ~/Easily-sdk-5/test/xdc_frame_test
OBJ_PATH = ~/Easily-sdk-5/test/xdc_frame_test
OUT_PATH = ~/Easily-sdk-5/linux/bin

SRCS = $(SRC_PATH)/xdc_frame_test.cc
OBJS = $(patsubst %.cc, %.o, $(SRCS))
TARGET = $(OUT_PATH)/xdc_frame_test

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxds -lxdu -lxdl -lxdc
	rm -f $(OBJS)

test:
	@echo $(SRCS)
	@echo $(OBJS)

.PHONY : clean
clean:
	-rm -f $(OBJS)
