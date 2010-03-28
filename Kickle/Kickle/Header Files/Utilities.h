#ifndef UTILITIES_H
#define UTILITIES_H

//Macros
#define SAFEDELETE(x) delete x; x = 0;
#define SAFEDELETEA(x) delete []x; x = 0;

#if defined(DEBUG)|defined(_DEBUG)
#define DEBUG_STATEMENT(x) x
#else
#define DEBUG_STATEMENT(x) 
#endif

// Rounds a floating point number to an integer
int round( float num );

#endif