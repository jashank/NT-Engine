/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

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
  m_FPS = 120.0;
}


int Config::GetScreenWidth() {
  return m_screenWidth;
}


int Config::GetScreenHeight() {
  return m_screenHeight;
}


float Config::GetFPS() {
  return m_FPS;
}

/**********************************
Static Members
***********************************/
int Config::m_screenWidth = 0;
int Config::m_screenHeight = 0;
float Config::m_FPS = 120.0;
