/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef NT_UTILITIES_H
#define NT_UTILITIES_H

#include <cmath>

#include <string>

//@{
/**
 * Safely delete pointer passed by setting it to NULL after deleting.
 */
#define SAFEDELETE(x) delete x; x = 0;
#define SAFEDELETEA(x) delete []x; x = 0;
//@}

#if defined(DEBUG)|defined(_DEBUG)
#define DEBUG_STATEMENT(x) x
#else
#define DEBUG_STATEMENT(x)
#endif

/**
 * @param str string to get lower-case version of.
 * @return String passed in all lower-case.
 */
std::string ToLowerCase( std::string str );

/**
 * @param filePath path to file to get name from.
 * @return Name of file at end of file path without extension name. 
 * For example, passing the file path 'Foo/Bar/joojoo.xml' would 
 * return 'joojoo'. Returns empty string if path doesn't exist.
 */
std::string GetFileName( std::string filePath );

/**
 * @param toRoundUp value to round up.
 * @return Value passed rounded up to the nearest integer. For example,
 * passing 15 or 15.5 goes to 16.
 */
int RoundUp( float toRoundUp );

/**
 * @return True if a is greater than b.
 */
template< typename T >
bool Greater( T a, T b ) {
  return (( a - b ) > 0 );
}  


/**
 * Clears Engine log.
 */
void ClearLog();

/**
 * Writes a message to the Engine log followed by a newline.
 * @param msg Message to write to the Engine log.
 */
void LogMsg( const std::string &msg );

/**
 * Writes message to Engine log with error tag ([ERR]) preceding the message
 * with a message followed by a newline.
 * @param msg Message to write to the Engine log.
 */
void LogErr( const std::string &msg );

/**
 * Writes message to Engine log with Lua error tag ([LUA_ERR]) preceding
 * the message with the message followed by a newline.
 * @param msg message to write to the Engine log.
 */
void LogLuaErr( const std::string &msg );

#endif // NT_UTILITIES_H

