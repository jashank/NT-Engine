CPP=g++
INCLUDES=-Iinclude -I/usr/local/include  -I/usr/include/lua5.1

CFLAGS += ${INCLUDES} -g -Wall
LDFLAGS += -L/usr/local/lib
LDLIBS = -llua5.1 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

OBJS= AnimData.o AnimSprite.o App.o Button.o Config.o GUI.o GUIManager.o \
        GameObject.o GameObjectManager.o GameState.o Image.o InputHandler.o \
        Key.o KeyManager.o Rect.o SoundManager.o Tile.o TileManager.o \
        Utilities.o main.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o

all:    src include ${OBJS}
	$(CPP) -o ../NTEngine ${OBJS} ${LDFLAGS} ${LDLIBS}

AnimData.o: src/AnimData.cpp include/AnimData.h include/App.h \
 include/AnimData.h include/KeyManager.h include/Key.h \
 include/ResourceManager.h include/Utilities.h include/ResourceManager.hpp \
 include/tinyxml.h include/tinystr.h include/Utilities.h
	${CPP} -c -o $@ ${CFLAGS} $<
AnimSprite.o: src/AnimSprite.cpp include/AnimSprite.h \
 include/App.h include/AnimData.h include/KeyManager.h include/Key.h \
 include/ResourceManager.h include/Utilities.h include/ResourceManager.hpp
	${CPP} -c -o $@ ${CFLAGS} $<
App.o: src/App.cpp include/App.h include/AnimData.h include/KeyManager.h \
 include/Key.h include/ResourceManager.h include/Utilities.h \
 include/ResourceManager.hpp include/GameState.h include/GameObjectManager.h \
 include/GUIManager.h include/SoundManager.h include/TileManager.h \
 include/Tile.h include/GameObjectManager.h include/TileManager.h \
 include/Utilities.h
	${CPP} -c -o $@ ${CFLAGS} $<

Button.o: src/Button.cpp include/Button.h include/GUI.h include/AnimSprite.h \
 include/InputHandler.h include/Key.h include/Lunar.h include/tinyxml.h \
 include/tinystr.h include/App.h include/AnimData.h include/KeyManager.h \
 include/ResourceManager.h include/Utilities.h include/ResourceManager.hpp \
 include/GameState.h include/GameObjectManager.h include/GUIManager.h \
 include/SoundManager.h include/TileManager.h include/Tile.h \
 include/Utilities.h
	${CPP} -c -o $@ ${CFLAGS} $<

Config.o: src/Config.cpp include/Config.h
	${CPP} -c -o $@ ${CFLAGS} $<

GUI.o: src/GUI.cpp include/GUI.h include/AnimSprite.h include/InputHandler.h \
 include/Key.h include/tinyxml.h include/tinystr.h include/App.h \
 include/AnimData.h include/KeyManager.h include/ResourceManager.h \
 include/Utilities.h include/ResourceManager.hpp include/GameState.h \
 include/GameObjectManager.h include/GUIManager.h include/SoundManager.h \
 include/TileManager.h include/Tile.h include/Utilities.h
	${CPP} -c -o $@ ${CFLAGS} $<

GUIManager.o: src/GUIManager.cpp include/GUIManager.h include/tinyxml.h \
 include/tinystr.h include/Button.h include/GUI.h include/AnimSprite.h \
 include/InputHandler.h include/Key.h include/GUI.h include/Image.h \
 include/Utilities.h
	${CPP} -c -o $@ ${CFLAGS} $<

GameObject.o: src/GameObject.cpp include/GameObject.h include/AnimSprite.h \
 include/InputHandler.h include/Key.h include/Lunar.h include/App.h \
 include/AnimData.h include/KeyManager.h include/ResourceManager.h \
 include/Utilities.h include/ResourceManager.hpp include/GameState.h \
 include/GameObjectManager.h include/GUIManager.h include/SoundManager.h \
 include/TileManager.h include/Tile.h include/TileManager.h \
 include/tinyxml.h include/tinystr.h
	${CPP} -c -o $@ ${CFLAGS} $<

GameObjectManager.o: src/GameObjectManager.cpp include/GameObjectManager.h \
 include/App.h include/AnimData.h include/KeyManager.h include/Key.h \
 include/ResourceManager.h include/Utilities.h include/ResourceManager.hpp \
 include/Config.h include/GameObject.h \
 include/AnimSprite.h include/InputHandler.h include/Lunar.h \
 include/GameState.h include/GameObjectManager.h include/GUIManager.h \
 include/SoundManager.h include/TileManager.h include/Tile.h \
 include/tinyxml.h include/tinystr.h include/Utilities.h
	${CPP} -c -o $@ ${CFLAGS} $<

GameState.o: src/GameState.cpp include/GameState.h include/GameObjectManager.h \
 include/GUIManager.h include/SoundManager.h include/TileManager.h \
 include/Tile.h include/App.h include/AnimData.h \
 include/KeyManager.h include/Key.h include/ResourceManager.h \
 include/Utilities.h include/ResourceManager.hpp include/tinyxml.h \
 include/tinystr.h include/Utilities.h
	${CPP} -c -o $@ ${CFLAGS} $<

Image.o: src/Image.cpp include/Image.h include/GUI.h include/AnimSprite.h \
 include/InputHandler.h include/Key.h
	${CPP} -c -o $@ ${CFLAGS} $<

InputHandler.o: src/InputHandler.cpp include/InputHandler.h include/Key.h \
 include/tinyxml.h include/tinystr.h include/App.h \
 include/AnimData.h include/KeyManager.h include/ResourceManager.h \
 include/Utilities.h include/ResourceManager.hpp include/KeyManager.h \
 include/Utilities.h
	${CPP}  -c -o $@ ${CFLAGS} $<

Key.o: src/Key.cpp include/Key.h
	${CPP} -c -o $@ ${CFLAGS} $<

KeyManager.o: src/KeyManager.cpp include/KeyManager.h include/Key.h \
 include/App.h include/AnimData.h include/ResourceManager.h \
 include/Utilities.h include/ResourceManager.hpp
	${CPP} -c -o $@ ${CFLAGS} $<

Rect.o: src/Rect.cpp include/Rect.h include/Vector.h
	${CPP} -c -o $@ ${CFLAGS} $<

SoundManager.o: src/SoundManager.cpp include/SoundManager.h \
 include/App.h include/AnimData.h include/KeyManager.h include/Key.h \
 include/ResourceManager.h include/Utilities.h \
 include/ResourceManager.hpp include/tinyxml.h include/tinystr.h \
 include/Utilities.h
	${CPP} -c -o $@ ${CFLAGS} $<

Tile.o: src/Tile.cpp include/Tile.h
	${CPP} -c -o $@ ${CFLAGS} $<

TileManager.o: src/TileManager.cpp include/TileManager.h \
 include/Tile.h include/AnimData.h include/AnimSprite.h include/App.h \
 include/AnimData.h include/KeyManager.h include/Key.h \
 include/ResourceManager.h \
 include/Utilities.h include/ResourceManager.hpp include/GameState.h \
 include/GameObjectManager.h include/GUIManager.h include/SoundManager.h \
 include/TileManager.h include/tinyxml.h include/tinystr.h
	${CPP} -c -o $@ ${CFLAGS} $<

Utilities.o: src/Utilities.cpp include/Utilities.h
	${CPP} -c -o $@ ${CFLAGS} $<

main.o: src/main.cpp include/Utilities.h include/App.h \
 include/AnimData.h include/KeyManager.h include/Key.h \
 include/ResourceManager.h include/Utilities.h \
 include/ResourceManager.hpp include/Config.h
	${CPP} -c -o $@ ${CFLAGS} $<

tinystr.o: src/tinystr.cpp include/tinystr.h
	${CPP} -c -o $@ ${CFLAGS} $<

tinyxml.o: src/tinyxml.cpp include/tinyxml.h include/tinystr.h
	${CPP} -c -o $@ ${CFLAGS} $<

tinyxmlerror.o: src/tinyxmlerror.cpp include/tinyxml.h include/tinystr.h
	${CPP} -c -o $@ ${CFLAGS} $<

tinyxmlparser.o: src/tinyxmlparser.cpp include/tinyxml.h include/tinystr.h
	${CPP} -c -o $@ ${CFLAGS} $<
