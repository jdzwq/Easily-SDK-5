CC = gcc
CFLAGS = -g -Wall -fPIC

SYS_PATH = /usr/local
LIB_PATH = ../lib
SRC_PATH = ../../xdp
OUT_PATH = ../lib

DIRS = $(wildcard $(SRC_PATH)/*.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libxdp.a

%.o : $(SRC_PATH)/%.c
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
