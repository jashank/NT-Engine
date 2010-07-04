CPP=g++
INCLUDES=-IEngine/Header\ Files -I/usr/include/lua5.1
LIBS=-llua5.1 -lsfml-audio -lsfml-graphics

all:
	$(CPP) -g -o NTEngine Engine/Source\ Files/* $(INCLUDES) $(LIBS)
