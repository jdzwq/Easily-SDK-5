CC = gcc
CFLAG = -g -Wall -fPIC

INC_PATH = /usr/local/include
LIB_PATH = /usr/local/lib
SRC_PATH = ../../bmp
OUT_PATH = ../lib
OBJ_PATH = .

OBJS = $(OBJ_PATH)/bmplib.o
LIB = $(OUT_PATH)/libbmp.a

all : $(LIB)

$(OBJS) : $(OBJ_PATH)/%.o : $(SRC_PATH)/*.c
	$(CC) $(CFLAG) -c $< -o $@ -I $(INC_PATH) -L $(LIB_PATH)

$(LIB) : $(OBJS)
	rm -f $@
	ar cr $@ $(OBJS)
	rm -f $(OBJS)

clean:
	rm -f $(OBJS)