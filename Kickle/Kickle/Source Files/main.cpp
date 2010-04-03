#include <memory>
#include <iostream>
#include "Utilities.h"

#include "tinyxml.h"

#include "App.h"
#include "Configuration.h"


int main() {
  DEBUG_STATEMENT( ClearLogs(); )

  Configuration::Load();

	std::auto_ptr< App > Game( 
    App::CreateApp( 
      "Kickle!", 
      Configuration::GetScreenWidth(), 
      Configuration::GetScreenHeight(), 
      Configuration::GetFPS() 
    ) 
  );

	Game->Run();
	
	//std::cin.get();

	return 0;
}