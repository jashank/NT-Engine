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

void Configuration::SetScreenWidth( Uint screenWidth ) {
  m_screenWidth = screenWidth;
}

Uint Configuration::GetScreenWidth() const {
  return m_screenWidth;
}

void Configuration::SetScreenHeight( Uint screenHeight ) {
  m_screenHeight = screenHeight;
}

Uint Configuration::GetScreenHeight() const {
  return m_screenHeight;
}

void Configuration::SetFPS( Uint FPS ) {
  m_FPS = FPS;
}

Uint Configuration::GetFPS() const {
  return m_FPS;
}

void Configuration::SetMapSize( Uint mapSize ) {
  m_mapSize = mapSize;
}

Uint Configuration::GetMapSize() const {
  return m_mapSize;
}

void Configuration::SetTileSize( Uint tileSize ) {
  m_tileSize = tileSize;
}

Uint Configuration::GetTileSize() const {
  return m_tileSize;
}

void Configuration::SetXPad( Uint xPad ) {
  m_xPad = xPad;
}

Uint Configuration::GetXPad() const {
  return m_xPad;
}

void Configuration::SetYPad( Uint yPad ) {
  m_yPad = yPad;
}

Uint Configuration::GetYPad() const {
  return m_yPad;
}

Uint Configuration::GetMapPixelSize() {
  return (m_tileSize*m_mapSize);
}

bool Configuration::IsTileValid( Uint x, Uint y ) {
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