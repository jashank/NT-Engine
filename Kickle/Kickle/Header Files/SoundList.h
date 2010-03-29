#ifndef SOUND_LIST_H
#define SOUND_LIST_H

#include <iostream>
#include <SFML/Audio.hpp>
#include <vector>

#include "Utilities.h"
#include "tinyxml.h"

// Feel free to change the name
class SoundList {
 public:
   SoundList();
   ~SoundList();

   void SetSoundList( TiXmlElement* root );
   void AddMusic( std::string musicPath );
   
   void Play();
   void Pause();
   void SetLoop( bool loop );

   void Update();

   void PlaySong( std::string musicPath );

 private:
  unsigned int m_currentSong; // Index of the current song.
  float m_duration;
  
  bool m_play;
  bool m_loop;

  sf::Sound m_sound;
  sf::SoundBuffer m_buffer;
  
  std::vector<std::string> m_playlist;
};

#endif // SOUND_MAP_H