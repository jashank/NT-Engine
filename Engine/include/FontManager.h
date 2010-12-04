#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <string>

#include <boost/shared_ptr.hpp>
#include <SFML/Graphics/Font.hpp>

#include "NamePath.h"

class TiXmlElement;

/**
 * Keeps track of fonts loaded in and names given to them. Allows
 * retrieval of fonts based on their name or path.
 */
class FontManager {
 public:
  FontManager() {}
  ~FontManager() {}

  /**
   * Loads fonts in from <fonts> element of State XML file.
   * @param root <font> element of file.
   * @return True if no errors in loading.
   */
  bool LoadData( const TiXmlElement *root );

  /**
   * Retrieves font from the FontManager.
   */
  boost::shared_ptr<sf::Font> GetFont( std::string &nameOrPath );

 private:
  typedef std::map<const NamePath, boost::shared_ptr<sf::Font> > map_type;

  map_type m_fonts;
};

#endif // FONTMANAGER_H

