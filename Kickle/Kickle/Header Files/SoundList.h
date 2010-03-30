#ifndef SOUND_LIST_H
#define SOUND_LIST_H

#include <iostream>
#include <SFML/Audio.hpp>
#include <vector>

#include "App.h"
#include "Utilities.h"
#include "tinyxml.h"

class SoundList {
 public:
   SoundList();
   ~SoundList();

   // Parses a section of the level xml document <music_playlist>
   void SetSoundList( TiXmlElement* root );
   // Adds a path of music to the playlist, this is called by SetSoundList
   void AddMusic( std::string musicPath );
   
   // Sets m_play to true
   void Play();
   // Sets m_play to false
   void Pause();
   // Sets m_loop to loop
   void SetLoop( bool loop );

   //Checks if the song is finished and if it should play the next song.
   void Update();

   // Plays the song at that path. Called by Update
   void PlaySong( std::string musicPath );

 private:
  unsigned int m_currentSong; // Index of the current song.
  float m_duration;
  
  bool m_play;
  bool m_loop;
  bool m_nextBuffered;
  sf::Sound m_sound;
  sf::SoundBuffer m_buffer;
  
  std::vector<std::string> m_playlist;
};

#endif // SOUND_MAP_H