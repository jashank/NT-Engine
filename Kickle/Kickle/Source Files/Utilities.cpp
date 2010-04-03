#include "Utilities.h"

#include <cmath>

#include <cstdarg>
#include <cstdio>

float  round( float num ) {
    return ( num > 0.0 ) ? (float) floor( num + 0.5 ) : (float) ceil( num - 0.5 );
}

void ClearLogs() {
  fopen( "EngineLog.txt", "w" );
  fopen( "EngineErrorLog.txt", "w" );
}

void DebugMsg( const char *format , ... ) {
  va_list arguments;
  va_start( arguments, format );

  vprintf( format, arguments );

  FILE *fout;
  fout = fopen( "EngineLog.txt", "a+" );
  
  vfprintf( fout, format, arguments );
  fclose( fout );

  va_end( arguments );
}

void DebugMsg( FILE *stream, const char *format, ... ) {
  va_list arguments;
  va_start( arguments, format );

  vfprintf( stream, format, arguments );

  if ( stream == stderr ) {
      FILE *foutError;
      foutError = fopen( "EngineErrorLog.txt", "a+" );
      vfprintf( foutError, format, arguments );
      fclose( foutError );
  }

  FILE *fout;
  fout = fopen( "EngineLog.txt", "a+" );
  vfprintf( fout, format, arguments );
  fclose( fout );

  va_end( arguments );
}
