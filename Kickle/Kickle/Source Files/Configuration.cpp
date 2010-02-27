#include "Configuration.h"

const Uint Config::SCREEN_WIDTH = 1024;
const Uint Config::SCREEN_HEIGHT = 768;
const Uint Config::FPS = 60;
const Uint Config::MAP_SIZE = 15;
const Uint Config::TILE_SIZE = 48;
const Uint Config::X_PAD = (Config::SCREEN_WIDTH - Config::MAP_SIZE*Config::TILE_SIZE)/2;
const Uint Config::Y_PAD = (Config::SCREEN_HEIGHT - Config::MAP_SIZE*Config::TILE_SIZE)/2;