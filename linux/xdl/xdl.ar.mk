CC = gcc
CFLAGS = -g -Wall

INC_PATH = ../../include
LIB_PATH = ../lib/x64
SRC_PATH = ../../xdl
OUT_PATH = ../lib/x64
OBJ_PATH = ./obj/x64

DIRS = $(wildcard $(SRC_PATH)/*.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libxdl.a

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(SRC_PATH) -L $(LIB_PATH)

$(TARGET) : $(OBJS)
	rm -f $@
	ar cr $@ $(OBJS)
	rm -f $(OBJS)

all: $(TARGET)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

clean:
	rm -f $(OBJS)