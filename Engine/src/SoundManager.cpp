#include "SoundManager.h"

#include <SFML/Audio.hpp>

#include "App.h"
#include "tinyxml.h"
#include "Utilities.h"

/********************************
Constructor and Destructor
********************************/
SoundManager::SoundManager()
 : m_playlistIndex( 0 ),
   m_currentMusic( NULL ),
   m_loop( false ) {}

/***********************************
Public Methods
***********************************/
bool SoundManager::LoadData( const TiXmlElement *dataRoot ) {
  // Music is optional
  const TiXmlElement *playlist =
    dataRoot->FirstChildElement( "music" );
  if ( playlist ) {
    const TiXmlElement *song = playlist->FirstChildElement( "song" );
    if ( song ) {
      do {
        const char *path = song->Attribute( "path" );
        if ( path ) {
          if ( !AddMusic( path )) {
            return false;
          }
        } else {
          LogErr( "No path specified for song." );
          return false;
        }
      } while ( (song = song->NextSiblingElement( "song" )) );
    } else {
      LogErr( "<music> tag not necessary when no songs specified." );
      return false;
    }
  }
  // SOUND EFFECTS
  return true;
}


void SoundManager::Play() {
  if ( m_currentMusic ) {
    m_currentMusic->Play();
  }
}


void SoundManager::Pause() {
  if ( m_currentMusic ) {
    m_currentMusic->Pause();
  }
}


void SoundManager::SetLoop( bool loop ) {
  m_loop = loop;
}


bool SoundManager::IsPlaying() const {
  return m_currentMusic &&
    ( m_currentMusic->GetStatus() == sf::Sound::Playing );
}


void SoundManager::Update() {
  if( !IsPlaying() ) {
    PlayNextSong();
  }
}

/******************************
Private Methods
******************************/
bool SoundManager::AddMusic( const std::string &musicPath ) {
  sf::Music *music = App::GetApp()->LoadMusic( musicPath );
  if ( music ) {
    m_playlist.push_back( music );
  } else {
    LogErr( "Unable to load music file " + musicPath );
    return false;
  }
  return true;
}


void SoundManager::PlayNextSong() {
  if ( m_playlistIndex < m_playlist.size() || m_loop ) {
    m_playlistIndex = m_playlistIndex % m_playlist.size();
    if ( IsPlaying() ) {
      m_currentMusic->Stop();
    }
    m_currentMusic = m_playlist[m_playlistIndex];
    m_currentMusic->Play();
    m_playlistIndex++;
  }
}
