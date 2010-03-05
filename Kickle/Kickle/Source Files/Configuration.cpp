#include "Configuration.h"


Configuration::Configuration()
:  m_screenWidth(1024),
   m_screenHeight(768),
   m_FPS(60),
   m_mapSize(15),
   m_tileSize(48),
   m_xPad((m_screenWidth - m_mapSize*m_tileSize)/2),
   m_yPad((m_screenHeight - m_mapSize*m_tileSize)/2) {
}

Configuration::~Configuration() {
}

void Configuration::SetScreenWidth( unsigned int screenWidth ) {
  m_screenWidth = screenWidth;
}

unsigned int Configuration::GetScreenWidth() const {
  return m_screenWidth;
}

void Configuration::SetScreenHeight( unsigned int screenHeight ) {
  m_screenHeight = screenHeight;
}

unsigned int Configuration::GetScreenHeight() const {
  return m_screenHeight;
}

void Configuration::SetFPS( unsigned int FPS ) {
  m_FPS = FPS;
}

unsigned int Configuration::GetFPS() const {
  return m_FPS;
}

void Configuration::SetMapSize( unsigned int mapSize ) {
  m_mapSize = mapSize;
}

unsigned int Configuration::GetMapSize() const {
  return m_mapSize;
}

void Configuration::SetTileSize( unsigned int tileSize ) {
  m_tileSize = tileSize;
}

unsigned int Configuration::GetTileSize() const {
  return m_tileSize;
}

void Configuration::SetXPad( unsigned int xPad ) {
  m_xPad = xPad;
}

unsigned int Configuration::GetXPad() const {
  return m_xPad;
}

void Configuration::SetYPad( unsigned int yPad ) {
  m_yPad = yPad;
}

unsigned int Configuration::GetYPad() const {
  return m_yPad;
}

unsigned int Configuration::GetMapPixelSize() {
  return (m_tileSize*m_mapSize);
}

bool Configuration::IsTileValid( unsigned int x, unsigned int y ) {
  if ( x < m_mapSize && x >= 0 && y < m_mapSize && y >=0 ) {
    return true;
  } else {
    return false;
  }
}

const Uint Config::SCREEN_WIDTH = 1024;
const Uint Config::SCREEN_HEIGHT = 768;
const Uint Config::FPS = 60;
const Uint Config::MAP_SIZE = 15;
const Uint Config::TILE_SIZE = 48;
const Uint Config::X_PAD = (Config::SCREEN_WIDTH - Config::MAP_SIZE*Config::TILE_SIZE)/2;
const Uint Config::Y_PAD = (Config::SCREEN_HEIGHT - Config::MAP_SIZE*Config::TILE_SIZE)/2;