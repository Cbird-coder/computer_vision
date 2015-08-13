CCP=g++
CC=gcc

INCLUDE=$(shell pkg-config opencv --cflags)
LIBS=$(shell pkg-config opencv --libs)

#C_SRC=$(wildcard *.c)
#C_OBJ=$(patsubst %c,%o,$(C_SRC))
CPP_SRC=kalman.cpp kalman_flmouse.cpp 
#CPP_OBJ=$(patsubst %cpp,%o,$(CPP_SRC))

.PHONY:clean

all:$(CPP_SRC)

	@$(CCP) kalman.cpp $(INCLUDE) $(LIBS)  -o kalman
	@$(CC)  kalman_flmouse.cpp  $(INCLUDE) $(LIBS)  -o kalman_flmouse
	@$(CC)  kalman_zj.cpp  $(INCLUDE) $(LIBS)  -o kalman_zj
clean:
	rm -rf *~ *o

