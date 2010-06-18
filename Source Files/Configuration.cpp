#include "Configuration.h"

/********************************************************
Public Member Functions
********************************************************/

Configuration::Configuration() {
}


Configuration::~Configuration() {
}


void Configuration::Load() {
  m_screenWidth = 1024;
  m_screenHeight = 768;
  m_FPS = 60;
  m_mapSize = 15;
  m_tileSize = 48;
  m_xPad = ( m_screenWidth - m_mapSize * m_tileSize ) / 2;
  m_yPad = ( m_screenHeight - m_mapSize * m_tileSize ) / 2;
}


void Configuration::SetScreenWidth( unsigned int screenWidth ) {
  m_screenWidth = screenWidth;
}


unsigned int Configuration::GetScreenWidth() {
  return m_screenWidth;
}


void Configuration::SetScreenHeight( unsigned int screenHeight ) {
  m_screenHeight = screenHeight;
}


unsigned int Configuration::GetScreenHeight() {
  return m_screenHeight;
}


void Configuration::SetFPS( unsigned int FPS ) {
  m_FPS = FPS;
}


unsigned int Configuration::GetFPS() {
  return m_FPS;
}


void Configuration::SetMapSize( unsigned int mapSize ) {
  m_mapSize = mapSize;
}


unsigned int Configuration::GetMapSize() {
  return m_mapSize;
}


void Configuration::SetTileSize( unsigned int tileSize ) {
  m_tileSize = tileSize;
}


unsigned int Configuration::GetTileSize() {
  return m_tileSize;
}


void Configuration::SetXPad( unsigned int xPad ) {
  m_xPad = xPad;
}


unsigned int Configuration::GetXPad() {
  return m_xPad;
}


void Configuration::SetYPad( unsigned int yPad ) {
  m_yPad = yPad;
}


unsigned int Configuration::GetYPad() {
  return m_yPad;
}


unsigned int Configuration::GetMapPixelSize() {
  return (m_tileSize * m_mapSize );
}


bool Configuration::IsTileValid( unsigned int x, unsigned int y ) {
  if ( x < m_mapSize && x >= 0 && y < m_mapSize && y >=0 ) {
    return true;
  } else {
    return false;
  }
}


/********************************************************
Private Members
********************************************************/

unsigned int Configuration::m_screenWidth = 0;
unsigned int Configuration::m_screenHeight = 0;
unsigned int Configuration::m_FPS = 0;
unsigned int Configuration::m_mapSize = 0;
unsigned int Configuration::m_tileSize = 0;
unsigned int Configuration::m_xPad = 0;
unsigned int Configuration::m_yPad = 0;