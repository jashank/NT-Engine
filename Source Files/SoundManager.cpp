#include "SoundManager.h"

#include <SFML/Audio.hpp>

#include "App.h"
#include "tinyxml.h"
#include "Utilities.h"

/********************************
Constructor and Destructor
********************************/
SoundManager::SoundManager( const TiXmlElement *dataRoot )
 : m_playlistIndex( 0 ), 
   m_currentMusic( NULL ),
   m_loop( false ) {
  SetLoop( true );
  Pause();
  LoadData( dataRoot );
}


SoundManager::~SoundManager() {
  if ( m_currentMusic ) {
    m_currentMusic->Stop();
  }
}

/***********************************
Public Methods
***********************************/
void SoundManager::LoadData( const TiXmlElement *dataRoot ) {
  const TiXmlElement *playlist = 
    dataRoot->FirstChildElement( "music_playlist" );

  const TiXmlElement *song = playlist->FirstChildElement( "song" );
  do {
    AddMusic( song->Attribute( "path" ));
  } while ( song = song->NextSiblingElement( "song" ));

  // SOUND EFFECTS
}


void SoundManager::AddMusic( const std::string &musicPath ) {
  sf::Music *music = App::GetApp()->LoadMusic( musicPath );
  if ( music ) {
    m_playlist.push_back( music );
  } else {
    LogErr( "Music file not found in SoundList::AddMusic." );
  }
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