/*! \mainpage NTEngine Index Page
* \section intro_section Introduction
* Welcome to the NTEngine API Documentation. NTEngine is a framework and 
* engine for 2D tile based game development that relies heavily on 
* metadata. Basically, you can make two completely different games without
* ever recompiling or touching the source code. I guess you could say it's
* a fancy parser. The core is implemented in C++, and the content is
* written in XML and Lua. 
*/

#include <string>
#include <sys/stat.h> // Used to check if input file exists.

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>

#include "Config.h"
#include "StateMachine.h"
#include "Utilities.h"
#include "Window.h"

int main( int argc, char *argv[] ) {
  ClearLog();
  Config::Load();

  std::string levelPath("Kickle_Pack/States/NTIntro.xml");
  if ( argc == 2 ) {
    struct stat fileInfoBuffer;
    if ( stat( argv[1], &fileInfoBuffer ) == 0 ) { // If the file exists.
      levelPath = std::string(argv[1]); // Get level from command line.
    }
  }

  std::string title = "Kickle!";
  nt::window::Create(
    title,
    Config::GetScreenWidth(),
    Config::GetScreenHeight()
  );
  StateMachine mach;
  mach.Setup( levelPath );

  float fixeddt = 1.0 / Config::GetFPS();
  float frameTime = 0.0;
  sf::Clock timer;
  timer.Reset();

  while ( nt::window::IsOpen() ) {
    while ( frameTime > 0.0 ) {
      float dt = std::min( frameTime, fixeddt );  
      mach.Step( dt );
      frameTime -= dt;
    }

    mach.Render();
    nt::window::Display();
    nt::window::Clear();

    frameTime = timer.GetElapsedTime();
    timer.Reset();
    if ( frameTime < fixeddt ) {
      sf::Sleep( fixeddt - frameTime );
    } 
  }

  return 0;
}
