/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

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
  m_fonts.Clear();
  m_fonts.LoadData( root );
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

