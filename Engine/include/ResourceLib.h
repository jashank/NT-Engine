#ifndef RESOURCELIB_H
#define RESOURCELIB_H

#include <string>

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>

#include <boost/shared_ptr.hpp>

#include "AnimData.h"

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
   * @return Resource if the load was successful or it has already been
   *         loaded. Will contain NULL if resource failed to load.
   */
  const boost::shared_ptr<sf::Image> &LoadImg( const std::string &relPath );
  const boost::shared_ptr<sf::Font> &LoadFont( const std::string &relPath );
  const boost::shared_ptr<sf::SoundBuffer> &LoadSound( 
    const std::string &relPath
  );
  const boost::shared_ptr<sf::Music> &LoadMusic( const std::string &relPath );
  const boost::shared_ptr<AnimData> &LoadAnim( const std::string &relPath );
  //@}

  /**
   * Releases unused resources. An unused resource means that nothing
   * currently holds the resource besides the manager.
   */
  void ReleaseUnused();

} // namespace rsrc
} // namespace nt

#endif // RESOURCELIB_H

