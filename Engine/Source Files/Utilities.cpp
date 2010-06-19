#include "Utilities.h"

#include <cmath>

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>


void ToLowerCase(std::string &str) {
  for (unsigned int i=0; i<str.length(); i++) {
    if (str[i] >= 0x41 && str[i] <= 0x5A) {
      str[i] = str[i] + 0x20;
    }
  }
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
