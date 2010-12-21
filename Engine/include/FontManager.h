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

#ifndef NT_FONTMANAGER_H
#define NT_FONTMANAGER_H

#include <string>

#include <SFML/Graphics/Font.hpp>

#include "NamePathManager.h"

class TiXmlElement;

namespace nt {

/**
 * Keeps track of fonts loaded in and names given to them. Allows
 * retrieval of fonts based on their name or path.
 */
class FontManager {
 public:
  /**
   * Gets data from <font> element of State file.
   */
  explicit FontManager( const TiXmlElement *root );

  /**
   * Retrieves font from the FontManager.
   */
  static boost::shared_ptr<sf::Font> GetFont( const std::string &nameOrPath );

 private:
  /** Holds all fonts loaded in. */
  static NamePathManager<sf::Font> m_fonts;
};

} // namespace nt

#endif // NT_FONTMANAGER_H

