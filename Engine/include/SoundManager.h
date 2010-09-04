#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <string>
#include <vector>

#include <SFML/Audio/Music.hpp>

class TiXmlElement;

/**
 * Holds and manages all sound in the current state. Currently only manages
 * playlist of music specified for state.
 */
class SoundManager {
 public:
  SoundManager();
  ~SoundManager() {}

  /**
   * Loads sound in from <sound> element of State XML file.
   * @param dataRoot parent element of sounds to be used in this state.
   * @return True if load was successful (no syntax errors).
   */
  bool LoadData( const TiXmlElement *dataRoot );

  /**
   * Plays current song in playlist.
   */
  void Play();

  /**
   * Pauses current song in playlist.
   */
  void Pause();

  /**
   * @param loop whether playlist should loop back to first song when finished.
   */
  void SetLoop( bool loop );

  /**
   * @return True if current song if playlist is playing.
   */
  bool IsPlaying() const;

  /**
   * Checks if current song in playlist is finished. If so, changes to next
   * song or stops music depending on if there is another song in the playlist
   * or the playlist is looped. 
   */
  void Update();

 private:
  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  SoundManager( const SoundManager &soundList );
  SoundManager& operator=( const SoundManager &soundList );
  //@}

  /**
   * Adds music found at file path to playlist.
   * @param filePath path to music file to add.
   * @return True if music loaded successfully.
   */
  bool AddMusic( const std::string &filePath );

  /**
   * Plays the next song in the playlist, switching back to the first if the
   * playlist is looped.
   */
  void PlayNextSong();

  unsigned int m_playlistIndex; /** Index to current music in playlist. */
  sf::Music *m_currentMusic; /** Current music playing in playlist. */
  bool m_loop; /** Whether the playlist repeats after finishing last song. */
  std::vector<sf::Music*> m_playlist; /** Songs to play in current State. */
};

#endif // SOUNDMANAGER_H

