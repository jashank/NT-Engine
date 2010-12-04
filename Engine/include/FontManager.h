#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <string>

#include <SFML/Graphics/Font.hpp>

#include "NamePathManager.h"

/**
 * Keeps track of fonts loaded in and names given to them. Allows
 * retrieval of fonts based on their name or path.
 */
class FontManager : public NamePathManager<sf::Font> {
 public:
  FontManager();

  /**
   * Retrieves font from the FontManager.
   */
  boost::shared_ptr<sf::Font> GetFont( const std::string &nameOrPath ) const;
};

#endif // FONTMANAGER_H

