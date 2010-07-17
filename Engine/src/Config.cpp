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


void Config::SetScreenWidth( int screenWidth ) {
  m_screenWidth = screenWidth;
}


int Config::GetScreenWidth() {
  return m_screenWidth;
}


void Config::SetScreenHeight( int screenHeight ) {
  m_screenHeight = screenHeight;
}


int Config::GetScreenHeight() {
  return m_screenHeight;
}


void Config::SetFPS( int FPS ) {
  m_FPS = FPS;
}


int Config::GetFPS() {
  return m_FPS;
}

/**********************************
Static Members
***********************************/
int Config::m_screenWidth = 0;
int Config::m_screenHeight = 0;
int Config::m_FPS = 0;
