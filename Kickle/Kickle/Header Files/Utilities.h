#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

//Macros
#define SAFEDELETE(x) delete x; x = 0;
#define SAFEDELETEA(x) delete []x; x = 0;

#if defined(DEBUG)|defined(_DEBUG)
#define DEBUG_STATEMENT(x) x
#else
#define DEBUG_STATEMENT(x) 
#endif

// Rounds a floating point number to an integer
float round( float num );

// Converts a vector

//Clears the logs it is the first thing that is called in the
// main function.
void ClearLog();
// Writes a message to Engine.log 
// and in debug mode the msg is also displayed in the console
void LogMsg( const std::ostringstream &msg );
void LogMsg( const std::string &msg );
// Writes an error message to Engine.log
// and in debug mode the error also displayed in the console
void LogErr( const std::ostringstream &msg );
void LogErr( const std::string &msg );
#endif