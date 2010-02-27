#include <memory>
#include <iostream>

#include "tinyxml.h"

#include "App.h"
#include "Configuration.h"


int main() {
	std::auto_ptr< App > Game( 
    App::CreateApp( 
      "Kickle!", 
      Config::SCREEN_WIDTH, 
      Config::SCREEN_HEIGHT, 
      Config::FPS 
    ) 
  );

	Game->Run();
	
	//std::cin.get();

	return 0;
}