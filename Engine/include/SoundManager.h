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

#ifndef NT_SOUNDMANAGER_H
#define NT_SOUNDMANAGER_H

#include <list>
#include <string>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "NamePathManager.h"

class TiXmlElement;

namespace nt {

/**
 * Keeps track of sounds loaded in and names given to them. Allows sound
 * to be played based on their name or path.
 */
class SoundManager {
 public:
  SoundManager();

  /**
   * Loads data from <sound_effects> element of Object file.
   */
  void LoadData( const TiXmlElement *root );

  /**
   * Plays sound associated with name or path passed.
   */
  void Play( const std::string &nameOrPath );

 private:
  /** Holds all sound buffers loaded in. */
  NamePathManager<sf::SoundBuffer> m_soundBuffers;

  /** Sounds currently playing. */
  std::list<sf::Sound> m_sounds;
};

}

#endif // NT_SOUNDMANAGER_H

