#include "AnimSprite.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window.hpp>

#include "AnimData.h"
#include "Rect.h"
#include "ResourceLib.h"
#include "Utilities.h"

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
  m_lastPos = GetPosition();

  if( m_playing ) {
    m_frameTimeLeft -= dt;
    if( m_frameTimeLeft <= 0.0f ) {
      NextFrame();
    }
  }
  m_setToReverse = false;
}


void AnimSprite::SetInitialPosition( float x, float y ) {
  m_lastPos.x = x;
  m_lastPos.y = y;
  SetPosition( x, y );
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
  const boost::shared_ptr<AnimData> &anim = nt::rsrc::LoadAnim( filepath );
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


void AnimSprite::Interpolate( float alpha ) {
  sf::Vector2f current;
  current.x = GetPosition().x * alpha;
  current.y = GetPosition().y * alpha;

  sf::Vector2f last;
  float diff = 1.0 - alpha;
  last.x = m_lastPos.x * ( diff );
  last.y = m_lastPos.y * ( diff );

  SetPosition( current.x + last.x, current.y + last.y );
}


// Follows basically the same algorithm as sf::Sprite. See the SFML
// documentation for details. Stuff has been cut out because engine
// doesn't include it.
void AnimSprite::Render( sf::RenderTarget &target ) const {
  // If there is animation data then there is an image.
  if ( m_animData ) {
    // Hold physics based position to go back to after rendering
    const sf::Vector2f &logicPosition = GetPosition();
    const boost::shared_ptr<sf::Image> &img = 
      m_animData->GetImage( m_animNum );

    glTranslatef( 0.375, 0.375, 0.0 );
    img->Bind();

    nt::core::IntRect ntRect =
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


