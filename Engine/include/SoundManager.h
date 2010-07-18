#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <string>
#include <vector>

class TiXmlElement;
namespace sf {
  class Music;
}

class SoundManager {
 public:
  SoundManager();
  ~SoundManager() {}

  /// Parses data from <sound> section of state file.
  /// Returns whether data load was successful.
  bool LoadData( const TiXmlElement *dataRoot );

  /// Sets m_play to true
  void Play();

  /// Sets m_play to false
  void Pause();

  /// Sets m_loop to loop
  void SetLoop( bool loop );

  /// Returns m_play
  bool IsPlaying() const;

  /// Checks if the song is finished and if it should play the next song.
  void Update();

 private:
  /// Restricts copy constructor, and assignment.
  SoundManager( const SoundManager &soundList );
  SoundManager& operator=( const SoundManager &soundList );

  /// Adds music found at file path to the playlist
  bool AddMusic( const std::string &musicPath );

  /// Plays the next song in the playlist
  void PlayNextSong();

  unsigned int m_playlistIndex; // Index of the current music.
  sf::Music *m_currentMusic; // Music currently playing
  bool m_loop;
  std::vector<sf::Music*> m_playlist;
};

#endif // SOUNDMANAGER_H
