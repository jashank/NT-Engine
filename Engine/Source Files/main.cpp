#include <memory>
#include "Utilities.h"
#include <string>
#include <sys/stat.h> // Used to check if input file exists.

#include "App.h"
#include "Config.h"


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

	std::auto_ptr< App > Game(
    App::CreateApp(
      "Kickle!",
      Config::GetScreenWidth(),
      Config::GetScreenHeight(),
      Config::GetFPS(),
	  levelPath
    )
  );

	Game->Run();

	return 0;
}
