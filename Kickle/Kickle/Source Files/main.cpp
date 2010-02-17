#include <memory>
#include <iostream>
#include "App.h"


int main() {
	std::auto_ptr< App > Game( App::CreateApp( "Kickle!", 1024, 768, 60 ) );

	Game->Run();
	
	std::cin.get();
	return 0;
}