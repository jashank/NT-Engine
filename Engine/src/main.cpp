/*! \mainpage NTEngine Index Page
* \section intro_section Introduction
* Welcome to the NTEngine API Documentation. NTEngine is a framework and 
* engine for 2D tile based game development that relies heavily on 
* metadata. Basically, you can make two completely different games without
* ever recompiling or touching the source code. I guess you could say it's
* a fancy parser. The core is implemented in C++, and the content is
* written in XML and Lua. 
*/

#include "Utilities.h"
#include <string>
#include <sys/stat.h> // Used to check if input file exists.

#include "Config.h"
#include "StateMachine.h"
#include "Window.h"


int main( int argc, char *argv[] ) {
  ClearLog();
  Config::Load();

  std::string levelPath("Kickle_Pack/States/title_state.xml");
  if ( argc == 2 ) {
    struct stat fileInfoBuffer;
    if ( stat( argv[1], &fileInfoBuffer ) == 0 ) { // If the file exists.
      levelPath = std::string(argv[1]); // Get level from command line.
    }
  }

  nt::window::Create(
    "Kickle!",
    Config::GetScreenWidth(),
    Config::GetScreenHeight(),
    Config::GetFPS()
  );
  nt::window::Refresh();

  StateMachine mach;
  mach.Setup( levelPath );

  while ( nt::window::IsOpen() ) {
    mach.Step();
    nt::window::Refresh();
  }

  nt::window::Destroy();
  return 0;
}
