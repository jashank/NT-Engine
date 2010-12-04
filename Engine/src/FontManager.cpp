#include "FontManager.h"

/*************************
 * Constructor
 ************************/
FontManager::FontManager()
  :NamePathManager<sf::Font>( "font" ) {}

/***********************
 * Public Methods
 **********************/
boost::shared_ptr<sf::Font> FontManager::GetFont( 
  const std::string &nameOrPath 
) const {
  return GetVal( nameOrPath );
}

