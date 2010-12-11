#include "SoundManager.h"

namespace nt {

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
    m_sound.SetBuffer( *buffer );
    m_sound.Play();
  }
}

}

