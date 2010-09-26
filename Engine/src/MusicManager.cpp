#include "MusicManager.h"

#include "App.h"
#include "tinyxml.h"
#include "Utilities.h"

/********************************
Constructor and Destructor
********************************/
MusicManager::MusicManager()
 : m_playlistIndex( 0 ),
   m_currentMusic( NULL ),
   m_loop( false ) {}

/***********************************
Public Methods
***********************************/
bool MusicManager::LoadData( const TiXmlElement *dataRoot ) {
  const TiXmlElement *song = dataRoot->FirstChildElement( "song" );
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
  }
  return true;
}


void MusicManager::Play() {
  if ( m_currentMusic ) {
    m_currentMusic->Play();
  }
}


void MusicManager::Pause() {
  if ( m_currentMusic ) {
    m_currentMusic->Pause();
  }
}


void MusicManager::SetLoop( bool loop ) {
  m_loop = loop;
}


bool MusicManager::IsPlaying() const {
  return m_currentMusic &&
    ( m_currentMusic->GetStatus() == sf::Sound::Playing );
}


void MusicManager::Update() {
  if( !IsPlaying() ) {
    PlayNextSong();
  }
}

/******************************
Private Methods
******************************/
bool MusicManager::AddMusic( const std::string &filePath ) {
  sf::Music *music = App::GetApp()->LoadMusic( filePath );
  if ( music ) {
    m_playlist.push_back( music );
  } else {
    LogErr( "Unable to load music file " + filePath );
    return false;
  }
  return true;
}


void MusicManager::PlayNextSong() {
  if ( (unsigned int) m_playlistIndex < m_playlist.size() || m_loop ) {
    m_playlistIndex = m_playlistIndex % m_playlist.size();
    if ( IsPlaying() ) {
      m_currentMusic->Stop();
    }
    m_currentMusic = m_playlist[m_playlistIndex];
    m_currentMusic->Play();
    m_playlistIndex++;
  }
}
