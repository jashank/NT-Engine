#include <memory>
#include <iostream>

#include "tinyxml.h"

#include "App.h"
#include "Configuration.h"


int main() {
	std::auto_ptr< App > Game( 
    App::CreateApp( 
      "Kickle!", 
      Configuration::SCREEN_WIDTH, 
      Configuration::SCREEN_HEIGHT, 
      Configuration::FPS 
    ) 
  );

	Game->Run();
	
	std::cin.get();

	return 0;
}