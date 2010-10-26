#include "AnimSprite.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

#include "AnimData.h"
#include "ResourceLib.h"

/************************************************
Public Methods
************************************************/
AnimSprite::AnimSprite()
 : m_animData( NULL ),
   m_playing( false ),
   m_reversed( false ),
   m_setToReverse( false ),
   m_frameTimeLeft( 0.0f ),
   m_animNum( -1 ),
   m_frameNum( -1 ) {
}


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

const AnimData *AnimSprite::GetAnimData() const {
  return m_animData;
}


int AnimSprite::GetFrame() const {
  return m_frameNum;
}


int AnimSprite::GetAnimation() const {
  return m_animNum;
}


void AnimSprite::LoadAnimData( const std::string &filepath ) {
  AnimData *anim = nt::rsrc::LoadAnim( filepath.c_str() );
  if ( anim ) {
    SetAnimData( anim );
  } else {
    LogErr( "Animation file: " + filepath +
            " failed to load in AnimSprite::LoadAnimData" );
  }
}


void AnimSprite::Play() {
  m_playing = true;
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
  
  nt::core::IntRect frameRect = 
    m_animData->GetFrameRect( m_animNum, m_frameNum );
  sf::IntRect subRect(
    frameRect.topLeft.x,
    frameRect.topLeft.y,
    frameRect.bottomRight.x,
    frameRect.bottomRight.y
  );

  SetSubRect( subRect );
}


void AnimSprite::SetAnimation( int animIndex ) {
  if( m_animData && m_animNum != animIndex ) {
    m_animNum = animIndex;   

    m_reversed = m_setToReverse;
    m_setToReverse = false;

    sf::Image *sheet = m_animData->GetImage( m_animNum );
    if ( sheet ) {
      SetImage( *sheet );
    }
    
    Stop();
  }
}


void AnimSprite::SetAnimData( const AnimData *animData ) {
  m_animData = animData;
  m_frameTimeLeft = m_animData->GetFrameTime( m_animNum, m_frameNum );

  sf::Image *sheet = m_animData->GetImage( m_animNum );
  if ( sheet ) {
    SetImage( *sheet );
  }
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

