#ifndef UTILITIES_H
#define UTILITIES_H

// To stop Microsofts non-standard C++ warnings.
#define _CRT_SECURE_NO_DEPRECATE 
#include <cstdio>

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

//Clears the logs it is the first thing that is called in the
// main function.
void ClearLogs();
// Writes the error message to the console and logs in EngineLog.txt
void DebugMsg( const char *format , ... );
// Same as above but allows for streams (particularly stderr if it is stderr
// it will log to EngineErrorLog.txt as well.
void DebugMsg( FILE *stream, const char *format, ... );

#endif