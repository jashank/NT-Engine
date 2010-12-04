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

template<>
const boost::shared_ptr<sf::Image> &LoadResource<sf::Image>(
  const std::string &relPath
) {
  return images.Load( relPath );
}

template<>
const boost::shared_ptr<sf::Font> &LoadResource<sf::Font>( 
  const std::string &relPath 
) {
  return fonts.Load( relPath );
}

template<>
const boost::shared_ptr<sf::SoundBuffer> &LoadResource<sf::SoundBuffer>( 
  const std::string &relPath
) {
  return sounds.Load( relPath );
}

template<>
const boost::shared_ptr<sf::Music> &LoadResource<sf::Music>( 
  const std::string &relPath 
) {
  return music.Load( relPath );
}

template<>
const boost::shared_ptr<AnimData> &LoadResource<AnimData>( 
  const std::string &relPath 
) {
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
  
