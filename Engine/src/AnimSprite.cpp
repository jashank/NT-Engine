/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

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

#include "AnimSprite.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window.hpp>

#include "ResourceLib.h"
#include "Utilities.h"

namespace nt {

/************************************************
Public Methods
************************************************/
AnimSprite::AnimSprite()
 : m_playing( false ),
   m_reversed( false ),
   m_setToReverse( false ),
   m_frameTimeLeft( 0.0f ),
   m_animNum( -1 ),
   m_frameNum( -1 ) {}


void AnimSprite::Update( float dt ) {
  if( m_playing ) {
    m_frameTimeLeft -= dt;
    if( m_frameTimeLeft <= 0.0f ) {
      NextFrame();
    }
  }
  m_setToReverse = false;
}


int AnimSprite::GetFrame() const {
  return m_frameNum;
}


int AnimSprite::GetFrameHeight() const {
  if ( m_animData ) {
    return m_animData->GetFrameHeight( m_animNum );
  }
  return 0;
}


int AnimSprite::GetAnimation() const {
  return m_animNum;
}


void AnimSprite::LoadAnimData( const std::string &filepath ) {
  const boost::shared_ptr<AnimData> &anim = 
    rsrc::LoadResource<AnimData>( filepath );
  if ( anim ) {
    SetAnimData( anim );
  } else {
    LogErr( "Animation file: " + filepath +
            " failed to load in AnimSprite::LoadAnimData" );
  }
}


void AnimSprite::Play() {
  // Only play if there is animation data to play
  m_playing = m_animData;
}


void AnimSprite::Pause() {
  m_playing = false;
}


void AnimSprite::Restart() {
  Stop();
  Play(); 
}


void AnimSprite::Stop() {
  Pause();
  if( !m_reversed ) {
    SetFrame( 0 );
  }
  else {
    SetFrame( m_animData->GetNumFrames( m_animNum ) - 1 );
  }
  m_frameTimeLeft = m_animData->GetFrameTime( m_animNum, m_frameNum );
}


void AnimSprite::SetReverse( bool reversed ) {
  m_setToReverse = reversed;
  m_reversed = reversed;
}


void AnimSprite::SetFrame( int frameNum ) {
  m_frameNum = frameNum;
}


void AnimSprite::SetAnimation( int animIndex ) {
  if( m_animData && m_animNum != animIndex ) {
    m_animNum = animIndex;   

    m_reversed = m_setToReverse;
    m_setToReverse = false;

    Stop();
  }
}


void AnimSprite::SetAnimData( const boost::shared_ptr<AnimData> &anim ) {
  m_animData = anim;
  m_frameTimeLeft = m_animData->GetFrameTime( m_animNum, m_frameNum );
}


bool AnimSprite::IsAnimating() {
  return m_playing;
}


void AnimSprite::SetAlpha( unsigned int alpha ) {
  SetColor( sf::Color( 255, 255, 255, alpha ));
}


/************************************************
Private Methods
************************************************/
void AnimSprite::NextFrame() {
  int frameNum = m_frameNum;
  if( !m_reversed ) {
    ++frameNum;
    if( frameNum >= m_animData->GetNumFrames( m_animNum ) ) {
      if( m_animData->IsLooped( m_animNum ) ) {
        frameNum = 0;
      }
      else {
        --frameNum;
        Pause();
      }
    }
  }
  else {
    --frameNum;
    if( frameNum == 0 ) {
      if( m_animData->IsLooped( m_animNum ) ) {
        frameNum = m_animData->GetNumFrames( m_animNum )-1;
      }
      else {
        Pause();
      }
    }
  }
  SetFrame( frameNum );
  m_frameTimeLeft = m_animData->GetFrameTime( m_animNum, m_frameNum );
}


// Follows basically the same algorithm as sf::Sprite. See the SFML
// documentation for details. Stuff has been cut out because engine
// doesn't include it.
void AnimSprite::Render( sf::RenderTarget &target ) const {
  // If there is animation data then there is an image.
  if ( m_animData ) {
    const boost::shared_ptr<sf::Image> &img = 
      m_animData->GetImage( m_animNum );

    glTranslatef( 0.375, 0.375, 0.0 );
    img->Bind();

    IntRect ntRect =
      m_animData->GetFrameRect( m_animNum, m_frameNum );
    float frameWidth = static_cast<float>( ntRect.GetWidth() );
    float frameHeight = static_cast<float>( ntRect.GetHeight() );

    // For now just convert to sfml stuff unless this becomes a
    // performance problem.
    sf::IntRect sfRect(
      ntRect.topLeft.x, 
      ntRect.topLeft.y, 
      ntRect.bottomRight.x,
      ntRect.bottomRight.y
    );

    sf::FloatRect texCoords = img->GetTexCoords( sfRect );

    glBegin(GL_QUADS);
      glTexCoord2f( texCoords.Left, texCoords.Top );
      glVertex2f( 0.0, 0.0 );

      glTexCoord2f( texCoords.Left, texCoords.Bottom );
      glVertex2f( 0.0, frameHeight );

      glTexCoord2f( texCoords.Right, texCoords.Bottom );
      glVertex2f( frameWidth, frameHeight );

      glTexCoord2f( texCoords.Right, texCoords.Top );
      glVertex2f( frameWidth, 0.0 );
    glEnd();
  }
}

} // namespace nt


