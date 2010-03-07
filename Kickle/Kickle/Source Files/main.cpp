#include <memory>
#include <iostream>

#include "tinyxml.h"

#include "App.h"
#include "Configuration.h"


int main() {
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