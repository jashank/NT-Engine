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

/// Converts string to lower case
std::string ToLowerCase( std::string str );

/// Returns name of passed xml file path.
/// Returns empty string if path is invalid.
std::string GetXmlFileName( std::string filepath );

/// Rounds values up. For example 15 or 15.5 goes to 16
int RoundUp( float toRoundUp );

/// Returns whether 2 floating point values are "equal"
bool Equals( float a, float b );

/// Returns whether float a is greater than float b
bool Greater( float a, float b );

/// Clears the logs it is the first thing that is called in the
/// main function.
void ClearLog();

/// Writes a message to Engine.log
/// and in debug mode the msg is also displayed in the console
void LogMsg( const std::string &msg );

/// Writes an error message to Engine.log
/// and in debug mode the error also displayed in the console
void LogErr( const std::string &msg );
void LogLuaErr( const std::string &msg );
#endif

