#include "Utilities.h"

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>


std::string ToLowerCase( std::string str ) {
  int len = str.length();
  for ( unsigned int i = 0; i < len; ++i ) {
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


