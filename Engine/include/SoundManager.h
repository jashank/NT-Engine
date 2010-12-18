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

