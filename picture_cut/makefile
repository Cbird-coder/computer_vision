CCP:=g++
CC:=gcc

INCLUDE:=$(shell pkg-config opencv --cflags)
LIBS:=$(shell pkg-config opencv --libs)

CPP_SRC:=img_roi.cpp
C_SRC:=file.c
OBJ:=file.o
DIR:= ./cutpicture
BIN:= cut_img

.PHONY:clean

all:
	$(CC) -c $(C_SRC) 
	$(CC)  $(CPP_SRC) $(OBJ) $(INCLUDE) $(LIBS)  -o $(BIN)
clean:
	rm -rf $(OBJ) $(BIN) 

