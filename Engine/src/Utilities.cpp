#include "Utilities.h"

#include <cmath>

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>


std::string ToLowerCase( std::string str ) {
  for ( int i = 0; i < str.length(); ++i ) {
    if ( str[i] >= 0x41 && str[i] <= 0x5A ) {
      str[i] = str[i] + 0x20;
    }
  }
  return str;
}


std::string GetXmlFileName( std::string filepath ) {
  size_t lastPeriod = filepath.find_last_of( '.' );
  size_t lastSlash = filepath.find_last_of( '\\' );
  if( lastSlash == std::string::npos ) {
    lastSlash = filepath.find_last_of( '/' );
  }

  if( lastPeriod == std::string::npos ||
      lastSlash == std::string::npos ||
      lastPeriod < lastSlash ) {
    // Improper filepath, unable to determine type
    return "";
  } else {
    // Grab the filename substring between lastSlash and lastPeriod
    filepath = filepath.substr( lastSlash+1, (lastPeriod-lastSlash)-1 );
  }

  return filepath;
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

void LogErr( const std::ostringstream &msg ) {
  DEBUG_STATEMENT( std::cout << "[ERR] - " << msg.str(); )
  std::ofstream file( "Engine.log", std::ios::out|std::ios::app );
  file << "[ERR] - " << msg.str() << "\n";
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
  file << "[LUA] - " << msg << "\n";
  file.close();
}
