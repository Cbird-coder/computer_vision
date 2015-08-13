CC:=g++
INCLUDE:=$(shell pkg-config opencv --cflags)
LIBS:=$(shell pkg-config opencv --libs)

CPP_SRC:=change.cpp
BIN:= change

.PHONY:clean
all:
	$(CC) $(CPP_SRC)  $(INCLUDE) $(LIBS)  -o $(BIN)
clean:
	rm -rf  $(BIN) 

