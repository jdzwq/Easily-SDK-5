CC = g++
CFLAGS = -g -Wall -fPIC

SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib

INC_MYSQL = /usr/include/mysql
LIB_MYSQL = /usr/lib

LIB_PATH = ../lib
INC_PATH = ~/Easily-sdk-5/include
NET_PATH = ~/Easily-sdk-5/xdb
OUT_PATH = ../sbin/api
LOC_PATH = .

SRCS = $(NET_PATH)/xdb_mysql.c
OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = $(OUT_PATH)/libxdb_mysql.so.1.0

%.o : $(LOC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_PATH) -I $(NET_PATH) -I $(INC_MYSQL) -L $(LIB_PATH)

all : $(OBJS)
	rm -f $@
	$(CC) -shared -fPIC -pthread -o $(TARGET) $(OBJS) -L $(LIB_PATH) -lxdp -lxdl -lmysqlclient -L $(LIB_MYSQL) 
	rm -f $(OBJS)

test:
	@echo $(DIRS)
	@echo $(SRCS)
	@echo $(OBJS)

install:
	sudo cp -f $(TARGET) $(SRV_PATH)/api;
	sudo chmod +x $(SRV_PATH)/api/libxdb_mysql.so.1.0;
	sudo rm -f $(LNK_PATH)/libxdb_mysql*;
	sudo ln -bs $(SRV_PATH)/api/libxdb_mysql.so.1.0 $(LNK_PATH)/libxdb_mysql.so;

.PHONY : clean
clean:
	-rm -f $(OBJS)
