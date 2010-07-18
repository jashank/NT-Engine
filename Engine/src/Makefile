CPP=g++
INCLUDES=-I../include -I/usr/local/include  -I/usr/include/lua5.1

CFLAGS += ${INCLUDES} -g -Wall
LDFLAGS += -L/usr/local/lib
LDLIBS = -llua5.1 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

SRC := $(shell echo *.cpp)
OBJS := $(SRC:.cpp=.o) 

all:    ${OBJS}
	$(CPP) -o ../../NTEngine ${OBJS} ${LDFLAGS} ${LDLIBS}
.cpp.o:
	${CPP} -c -o $@ ${CFLAGS} $<

