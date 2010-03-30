#include "SoundList.h"

SoundList::SoundList()
 : m_currentSong( 0 ), 
   m_duration( 0.0f ),
   m_loop( false ),
   m_play( false ) {
  SetLoop( true );
  Pause();
}

SoundList::~SoundList() {
}

void SoundList::SetSoundList( TiXmlElement* root ) {
  TiXmlElement* currentSong;
  std::string songData;

  for ( currentSong = root->FirstChildElement( "song" ); currentSong; 
        currentSong = currentSong->NextSiblingElement( "song" ) ) {
    songData = currentSong->Attribute( "data" );
    AddMusic( songData );
  }
}

void SoundList::AddMusic( std::string musicPath ) {
  m_playlist.push_back( musicPath );
}

void SoundList::Play() {
  m_play = true;
}

void SoundList::Pause() {
  m_play = false;
}

void SoundList::SetLoop( bool loop ) {
  m_loop = loop;
}

void SoundList::Update() {
  if ( m_sound.GetStatus() == sf::Sound::Stopped ) {
    if ( m_currentSong < m_playlist.size() || m_loop == true ) {
      m_currentSong = m_currentSong%m_playlist.size();
      PlaySong( m_playlist[m_currentSong] );
      m_currentSong++;
    }
  }

}

void SoundList::PlaySong( std::string musicPath ) {
  if ( !m_buffer.LoadFromFile( musicPath ) ) {
    DEBUG_STATEMENT( std::cout << "Cannot Open Music: " << musicPath << std::endl; );
  } else {
    m_sound.SetBuffer( m_buffer );
    m_duration = m_buffer.GetDuration();
    m_sound.Play();
  }
}

