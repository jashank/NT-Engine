#include "Config.h"

/********************************************************
Public Member Functions
********************************************************/

Config::Config() {
}


Config::~Config() {
}


void Config::Load() {
  m_screenWidth = 720;
  m_screenHeight = 720;
  m_FPS = 60;
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

/**********************************
Static Members
***********************************/
unsigned int Config::m_screenWidth = 0;
unsigned int Config::m_screenHeight = 0;
unsigned int Config::m_FPS = 0;
