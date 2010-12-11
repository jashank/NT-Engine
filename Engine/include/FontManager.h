#ifndef FONTMANAGER_H
#define FONTMANAGER_H

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
  FontManager( const TiXmlElement *root );

  /**
   * Retrieves font from the FontManager.
   */
  static boost::shared_ptr<sf::Font> GetFont( const std::string &nameOrPath );

 private:
  /** Holds all fonts loaded in. */
  static NamePathManager<sf::Font> m_fonts;
};

} // namespace nt

#endif // FONTMANAGER_H

