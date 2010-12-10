#include "FontManager.h"

namespace nt {

/********************
 * Static Members
 *******************/
NamePathManager<sf::Font> FontManager::m_fonts( "font" );

/*************************
 * Constructor
 ************************/
FontManager::FontManager( const TiXmlElement *root ) {
  m_fonts.LoadData( root );
}


FontManager::~FontManager() {
  m_fonts.Clear();
}

/***********************
 * Public Methods
 **********************/
boost::shared_ptr<sf::Font> FontManager::GetFont( 
  const std::string &nameOrPath 
) {
  return m_fonts.GetVal( nameOrPath );
}

} // namespace nt

