#ifndef RESOURCELIB_H
#define RESOURCELIB_H

#include <string>

// Forward declarations of SFML resources
namespace sf {
  class Image;
  class Font;
  class SoundBuffer;
  class Music;  
}

class AnimData;

/**
 * nt::rsrc contains functions to load resources in the application.
 * Resources include sf::Image, sf::Font, sf::SoundBuffer, sf::Music, and
 * AnimData.   
 *
 * All of the functions ensure that duplicates are not loaded.
 */
namespace nt {
namespace rsrc {
  //@{
  /**
   * The following functions allow loading of various resources.
   *
   * See the SFML documentation to see file formats supported.
   * @param relPath path to file to load that is relative to location of engine.
   * @return Resource if the load was successful or it has already been loaded,
   *         NULL otherwise.
   */
  sf::Image *LoadImg( const std::string &relPath );
  sf::Font *LoadFont( const std::string &relPath );
  sf::SoundBuffer *LoadSound( const std::string &relPath );
  sf::Music *LoadMusic( const std::string &relPath );
  AnimData *LoadAnim( const std::string &relPath );
  //@}

  //@{
  /**
   * The following functions release the various resources.
   */
  void ClearImgs();
  void ClearFonts();
  void ClearSounds();
  void ClearMusic();
  void ClearAnims();
  void ClearAll();
  //@}

} // namespace rsrc
} // namespace nt

#endif // RESOURCELIB_H
