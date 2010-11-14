#include "ResourceLib.h"

#include "ResourceManager.h"

// All of the resources the engine holds
namespace {
  ResourceManager<sf::Image> images;
  ResourceManager<sf::Font> fonts;
  ResourceManager<sf::SoundBuffer> sounds;
  ResourceManager<sf::Music> music;
  ResourceManager<AnimData> anims;
}


namespace nt {
namespace rsrc {

const boost::shared_ptr<sf::Image> &LoadImg( const std::string &relPath ) {
  return images.Load( relPath );
}

const boost::shared_ptr<sf::Font> &LoadFont( const std::string &relPath ) {
  return fonts.Load( relPath );
}

const boost::shared_ptr<sf::SoundBuffer> &LoadSound( 
  const std::string &relPath
) {
  return sounds.Load( relPath );
}

const boost::shared_ptr<sf::Music> &LoadMusic( const std::string &relPath ) {
  return music.Load( relPath );
}

const boost::shared_ptr<AnimData> &LoadAnim( const std::string &relPath ) {
  return anims.Load( relPath );
}

void ReleaseUnused() {
  // Order matters! (Anims hold images)
  anims.ReleaseUnused();    
  images.ReleaseUnused();
  fonts.ReleaseUnused();
  sounds.ReleaseUnused();
  music.ReleaseUnused();
}
 
} // namespace rsrc 
} // namespace nt
  
