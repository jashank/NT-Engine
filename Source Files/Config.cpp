#include "Config.h"

/********************************************************
Public Member Functions
********************************************************/

Config::Config() {
}


Config::~Config() {
}


void Config::Load() {
  m_screenWidth = 1024;
  m_screenHeight = 768;
  m_FPS = 60;
  m_mapSize = 15;
  m_tileSize = 48;
  m_xPad = ( m_screenWidth - m_mapSize * m_tileSize ) / 2;
  m_yPad = ( m_screenHeight - m_mapSize * m_tileSize ) / 2;
}


void Config::SetScreenWidth( unsigned int screenWidth ) {
  m_screenWidth = screenWidth;
}


unsigned int Config::GetScreenWidth() {
  return m_screenWidth;
}


void Config::SetScreenHeight( unsigned int screenHeight ) {
  m_screenHeight = screenHeight;
}


unsigned int Config::GetScreenHeight() {
  return m_screenHeight;
}


void Config::SetFPS( unsigned int FPS ) {
  m_FPS = FPS;
}


unsigned int Config::GetFPS() {
  return m_FPS;
}


void Config::SetMapSize( unsigned int mapSize ) {
  m_mapSize = mapSize;
}


unsigned int Config::GetMapSize() {
  return m_mapSize;
}


void Config::SetTileSize( unsigned int tileSize ) {
  m_tileSize = tileSize;
}


unsigned int Config::GetTileSize() {
  return m_tileSize;
}


void Config::SetXPad( unsigned int xPad ) {
  m_xPad = xPad;
}


unsigned int Config::GetXPad() {
  return m_xPad;
}


void Config::SetYPad( unsigned int yPad ) {
  m_yPad = yPad;
}


unsigned int Config::GetYPad() {
  return m_yPad;
}


unsigned int Config::GetMapPixelSize() {
  return (m_tileSize * m_mapSize );
}


bool Config::IsTileValid( unsigned int x, unsigned int y ) {
  return ( x < m_mapSize && x >= 0 && y < m_mapSize && y >= 0 );
}


/********************************************************
Private Members
********************************************************/

unsigned int Config::m_screenWidth = 0;
unsigned int Config::m_screenHeight = 0;
unsigned int Config::m_FPS = 0;
unsigned int Config::m_mapSize = 0;
unsigned int Config::m_tileSize = 0;
unsigned int Config::m_xPad = 0;
unsigned int Config::m_yPad = 0;