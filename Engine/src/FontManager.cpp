#include "FontManager.h"

namespace nt {

/*************************
 * Constructor
 ************************/
FontManager::FontManager( const TiXmlElement *root )
  :NamePathManager<sf::Font>( "font" ) {
  LoadData( root );
}

/***********************
 * Public Methods
 **********************/
boost::shared_ptr<sf::Font> FontManager::GetFont( 
  const std::string &nameOrPath 
) const {
  return GetVal( nameOrPath );
}

} // namespace nt

