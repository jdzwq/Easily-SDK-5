CC = gcc
CFLAGS = -g -Wall -fPIC

SYS_PATH = /usr/local
LIB_PATH = ../lib/x64
SRC_PATH = ../../xdk
SUB_PATH = ../../xdk/linux
OUT_PATH = ../lib/x64
OBJ_PATH = ./obj/x64

DIRS = $(wildcard $(SRC_PATH)/*.c $(SUB_PATH)/*.c)
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libxdk.so.1.0

%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(SRC_PATH) -L $(LIB_PATH)

%.o : $(SUB_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(SRC_PATH) -L $(LIB_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -o $(TARGET) $(OBJS) -L $(LIB_PATH)
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

install: 
	if [[! -d "$(SYS_PATH)/lib"]]; \
	then \
	mkdir $(SYS_PATH)/lib; \
	fi
	cp -f $(TARGET) $(SYS_PATH)/lib
	chmod 755 $(TARGET)

.PHONY : clean
clean:
	-rm -f $(OBJS)
