# NTEngine Makefile
# Dependencies: SFML-1.5, Boost-1_43 Headers (Bind, Function), liblua5.1.4

CPP=g++
INCLUDES=-I../include -I/usr/local/include  -I/usr/include/lua5.1

CFLAGS = ${INCLUDES} -g -Wall -O2 -pipe
LDFLAGS = -L/usr/local/lib
LDLIBS = -llua5.1 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

SRC := $(shell echo *.cpp)
OBJS := $(addprefix obj/, $(SRC:.cpp=.o))

all:    ${OBJS}
	$(CPP) -o ../../NTEngine ${OBJS} ${LDFLAGS} ${LDLIBS}

obj/%.o: %.cpp
	${CPP} -c -o $@ ${CFLAGS} $<

clean:
	rm obj/*.o
