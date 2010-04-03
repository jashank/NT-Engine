#include "SoundList.h"

SoundList::SoundList()
 : m_currentSong( 0 ), 
   m_duration( 0.0f ),
   m_loop( false ),
   m_play( false ),
   m_music( NULL ) {
  SetLoop( true );
  Pause();
}

SoundList::~SoundList() {
  m_music->Stop();
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
  if ( m_music != NULL ) {
    m_music->Play();
  }
}

void SoundList::Pause() {
  m_play = false;
  if ( m_music != NULL ) {
    m_music->Pause();
  }
}

void SoundList::SetLoop( bool loop ) {
  m_loop = loop;
}

bool SoundList::IsPlaying() {
  return m_play;
}

void SoundList::Update() {
  if ( m_music != NULL ) {
    if ( m_music->GetStatus() == sf::Sound::Stopped ) {
      PlayNextSong();
    }
  } else {
    PlayNextSong();
  }

}

void SoundList::PlaySong( std::string musicPath ) {
  try {
    m_music = &( App::GetApp()->LoadMusic( musicPath ) );
  } catch ( const char* msg ) {
    // Needs to be changed to a std::exception but for now
    // it will throw a warning.
    m_music = NULL;
  }
  Play();
}

void SoundList::PlayNextSong() {
  if ( m_currentSong < m_playlist.size() || m_loop == true ) {
    m_currentSong = m_currentSong%m_playlist.size();
    PlaySong( m_playlist[m_currentSong] );
    m_currentSong++;
  }
}