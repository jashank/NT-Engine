#include "Utilities.h"

#include <cmath>

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>


void ToLowerCase( std::string &str ) {
  for (unsigned int i=0; i<str.length(); i++) {
    if (str[i] >= 0x41 && str[i] <= 0x5A) {
      str[i] = str[i] + 0x20;
    }
  }
}


std::string& GetXmlFileName( const std::string &filepath ) {
  static std::string filename = filepath;

  size_t lastPeriod = filepath.find_last_of( '.' );
  size_t lastSlash = filepath.find_last_of( '\\' );
  if( lastSlash == std::string::npos ) {
    lastSlash = filepath.find_last_of( '/' );
  }

  if( lastPeriod == std::string::npos ||
      lastSlash == std::string::npos ||
      lastPeriod < lastSlash ) {
    // Improper filepath, unable to determine type
    filename = "";
  } else {
    // Grab the filename substring between lastSlash and lastPeriod
    filename = filepath.substr( lastSlash+1, (lastPeriod-lastSlash)-1 );
  }

  return filename;
}


void ClearLog() {
  //ios::trunc erases previous content contained by file opened.
  std::ofstream file( "Engine.log", std::ios::out|std::ios::trunc );
}


void LogMsg( const std::ostringstream &msg ) {
  DEBUG_STATEMENT( std::cout << msg.str(); )
  std::ofstream file( "Engine.log", std::ios::out|std::ios::app );
  file << msg.str() << "\n";
  file.close();
}


void LogMsg( const std::string &msg ) {
  LogMsg( std::ostringstream( msg ));
}


void LogErr( const std::ostringstream &msg ) {
  DEBUG_STATEMENT( std::cout << "[ERR] - " << msg.str(); )
  std::ofstream file( "Engine.log", std::ios::out|std::ios::app );
  file << "[ERR] - " << msg.str() << "\n";
  file.close();
}


void LogErr( const std::string &msg ) {
  LogErr( std::ostringstream( msg ));
}
