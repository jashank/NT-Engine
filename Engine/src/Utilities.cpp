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

#include "Utilities.h"

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>


std::string ToLowerCase( std::string str ) {
  int len = str.length();
  for ( int i = 0; i < len; ++i ) {
    if ( str[i] >= 0x41 && str[i] <= 0x5A ) {
      str[i] = str[i] + 0x20;
    }
  }
  return str;
}


std::string GetFileName( std::string filePath ) {
  size_t lastPeriod = filePath.find_last_of( '.' );
  size_t lastSlash = filePath.find_last_of( '\\' );
  if( lastSlash == std::string::npos ) {
    lastSlash = filePath.find_last_of( '/' );
  }

  if( lastPeriod == std::string::npos ||
      lastSlash == std::string::npos ||
      lastPeriod < lastSlash ) {
    // Improper filePath, unable to determine type
    return "";
  } else {
    // Grab the filename substring between lastSlash and lastPeriod
    filePath = filePath.substr( lastSlash+1, (lastPeriod-lastSlash)-1 );
  }

  return filePath;
}


int RoundUp( float toRoundUp ) {
  return int(toRoundUp+1.0f);
}


void ClearLog() {
  //ios::trunc erases previous content contained by file opened.
  std::ofstream file( "Engine.log", std::ios::out|std::ios::trunc );
}

void LogMsg( const std::string &msg ) {
  DEBUG_STATEMENT( std::cout << msg; );
  std::ofstream file( "Engine.log", std::ios::out|std::ios::app );
  file << msg << "\n";
  file.close();
}


void LogErr( const std::string &msg ) {
  DEBUG_STATEMENT( std::cout << "[ERR] - " << msg; )
  std::ofstream file( "Engine.log", std::ios::out|std::ios::app );
  file << "[ERR] - " << msg << "\n";
  file.close();
}


void LogLuaErr( const std::string &msg ) {
  std::ofstream file( "Engine.log", std::ios::out|std::ios::app );
  file << "[LUA_ERR] - " << msg << "\n";
  file.close();
}


