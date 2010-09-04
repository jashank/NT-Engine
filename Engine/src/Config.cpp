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


int Config::GetScreenWidth() {
  return m_screenWidth;
}


int Config::GetScreenHeight() {
  return m_screenHeight;
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
