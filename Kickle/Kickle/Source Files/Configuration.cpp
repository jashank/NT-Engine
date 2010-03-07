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


void Configuration::SetScreenWidth( Uint screenWidth ) {
  m_screenWidth = screenWidth;
}


Uint Configuration::GetScreenWidth() {
  return m_screenWidth;
}


void Configuration::SetScreenHeight( Uint screenHeight ) {
  m_screenHeight = screenHeight;
}


Uint Configuration::GetScreenHeight() {
  return m_screenHeight;
}


void Configuration::SetFPS( Uint FPS ) {
  m_FPS = FPS;
}


Uint Configuration::GetFPS() {
  return m_FPS;
}


void Configuration::SetMapSize( Uint mapSize ) {
  m_mapSize = mapSize;
}


Uint Configuration::GetMapSize() {
  return m_mapSize;
}


void Configuration::SetTileSize( Uint tileSize ) {
  m_tileSize = tileSize;
}


Uint Configuration::GetTileSize() {
  return m_tileSize;
}


void Configuration::SetXPad( Uint xPad ) {
  m_xPad = xPad;
}


Uint Configuration::GetXPad() {
  return m_xPad;
}


void Configuration::SetYPad( Uint yPad ) {
  m_yPad = yPad;
}


Uint Configuration::GetYPad() {
  return m_yPad;
}


Uint Configuration::GetMapPixelSize() {
  return (m_tileSize * m_mapSize );
}


bool Configuration::IsTileValid( Uint x, Uint y ) {
  if ( x < m_mapSize && x >= 0 && y < m_mapSize && y >=0 ) {
    return true;
  } else {
    return false;
  }
}


/********************************************************
Private Members
********************************************************/

Uint Configuration::m_screenWidth = 0;
Uint Configuration::m_screenHeight = 0;
Uint Configuration::m_FPS = 0;
Uint Configuration::m_mapSize = 0;
Uint Configuration::m_tileSize = 0;
Uint Configuration::m_xPad = 0;
Uint Configuration::m_yPad = 0;