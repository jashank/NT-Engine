#include <memory>
#include <iostream>
#include "Utilities.h"

#include "tinyxml.h"

#include "App.h"
#include "Config.h"


int main() {
  DEBUG_STATEMENT( ClearLog(); )

  Config::Load();

	std::auto_ptr< App > Game( 
    App::CreateApp( 
      "Kickle!", 
      Config::GetScreenWidth(), 
      Config::GetScreenHeight(), 
      Config::GetFPS() 
    ) 
  );

	Game->Run();
	

	return 0;
}