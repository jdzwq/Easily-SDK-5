CC = gcc
CFLAG = -g -Wall -fPIC

INC_PATH = /usr/local/include
LIB_PATH = /usr/local/lib
SRC_PATH = ../../jpg
OUT_PATH = ../lib
OBJ_PATH = ./obj

DIRS = $(foreach val, $(SRC_PATH), $(wildcard $(val)/*.c))
SRCS = $(notdir $(DIRS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
LIB = $(OUT_PATH)/libjpg.a

all : $(LIB)

$(OBJS) : %.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAG) -c $< -o $@ -I $(INC_PATH) -L $(LIB_PATH)

$(LIB) : $(OBJS)
	rm -f $@
	ar cr $@ $(OBJS)
	rm -f $(OBJS)

clean:
	rm -f $(OBJS)