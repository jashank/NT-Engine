/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

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

