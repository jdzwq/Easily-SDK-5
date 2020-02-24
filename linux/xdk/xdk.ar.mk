CC = gcc
CFLAGS = -g -Wall 

SYS_PATH = /usr/local
LIB_PATH = ../lib/x64
SRC_PATH = ../../xdk
SUB_PATH = ../../xdk/linux
OUT_PATH = ../lib/x64
OBJ_PATH = ./obj/x64

DIRS = $(wildcard $(SRC_PATH)/*.c $(SUB_PATH)/*.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libxdk.a

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(SRC_PATH) -L $(LIB_PATH)

%.o : $(SUB_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(SRC_PATH) -L $(LIB_PATH)

$(TARGET) : $(OBJS)
	rm -f $@
	ar cr $@ $(OBJS)
	rm -f $(OBJS)

all : $(TARGET)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

.PHONY : clean
clean:
	-rm -f $(OBJS)
