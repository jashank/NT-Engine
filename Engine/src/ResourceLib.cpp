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

#include "ResourceLib.h"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>

#include "ResourceManager.h"

#include "AnimData.h"

namespace nt {
namespace rsrc {

// All of the resources the engine holds
namespace {
  ResourceManager<sf::Image> images;
  ResourceManager<sf::Font> fonts;
  ResourceManager<sf::SoundBuffer> sounds;
  ResourceManager<sf::Music> music;
  ResourceManager<AnimData> anims;
}


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
  
