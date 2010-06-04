#include "Utilities.h"

#include <cmath>

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

float  round( float num ) {
    return ( num > 0.0 ) ? (float) floor( num + 0.5 ) : (float) ceil( num - 0.5 );
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