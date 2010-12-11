#include "SoundManager.h"

#include <algorithm>

namespace nt {

namespace {
  bool Stopped( const sf::Sound &sound ) {
    return sound.GetStatus() == sf::Sound::Stopped;
  }
}

/*********************
 * Constructor
 ********************/
SoundManager::SoundManager() 
  :m_soundBuffers( "sound" ) {}

/**********************
 * Public Methods
 *********************/
void SoundManager::LoadData( const TiXmlElement *root ) {
  m_soundBuffers.LoadData( root );
}


void SoundManager::Play( const std::string &nameOrPath ) {
  sf::SoundBuffer *buffer = m_soundBuffers.GetVal( nameOrPath ).get();
  if ( buffer ) {
    std::remove_if( m_sounds.begin(), m_sounds.end(), Stopped );

    m_sounds.push_back( sf::Sound() );
    m_sounds.back().SetBuffer( *buffer );
    m_sounds.back().Play();
  }
}

}

