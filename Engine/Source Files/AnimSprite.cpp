#include "AnimSprite.h"

#include "App.h"
/************************************************
Private Methods
************************************************/
AnimSprite::AnimSprite()
 : m_animData( NULL ),
   m_play( false ),
   m_reverse( false ),
   m_frameTime( 0.0f ),
   m_animation( 0 ),
   m_frame( 0 ) {
}

const AnimData *AnimSprite::GetAnimData() const {
          return m_animData;
}


unsigned int AnimSprite::GetFrame() const {
        return m_frame;
}


unsigned int AnimSprite::GetAnimation() const {
        return m_animation;
}

void AnimSprite::LoadAnimData( const std::string &filepath ) {
  App* app = App::GetApp();

  AnimData *anim = app->LoadAnim( filepath.c_str() );
  if ( anim ) {
    SetAnimData( anim );
  } else {
    LogErr( "Animation file: " + filepath +
            " failed to load in AnimSprite::LoadAnimData" );
  }
}

void AnimSprite::Play() {
  if( m_animData ) {
        m_play = true;
        SetSubRect( m_animData->GetFrameRect( m_animation, m_frame ) );
  }
}


void AnimSprite::Pause() {
        m_play = false;
}


void AnimSprite::Restart() {
  if( m_animData ) {
    Stop();
    m_frameTime = m_animData->GetFrameTime( m_animation, m_frame );
    Play();
  }
}


void AnimSprite::SetFrame( unsigned int frame ) {
        m_frame = frame;
}


void AnimSprite::SetAnimation( unsigned int animation, bool reverse ) {
  if( m_animData ) {
    if ( m_reverse ) {
      m_reverse = false;
    }
    m_reverse = reverse;
    m_animation = animation;
    SetImage( m_animData->GetImage( m_animation ));
    Restart();
  }
}


void AnimSprite::SetAnimData( const AnimData *animData ) {
  m_animData = animData;
  m_frameTime = m_animData->GetFrameTime( m_animation, m_frame );
  SetImage( m_animData->GetImage( m_animation ));
}


void AnimSprite::Stop() {
        Pause();
  if( !m_reverse ) {
    SetFrame( 0 );
  }
  else {
    SetFrame( m_animData->GetNumFrames( m_animation )-1 );
  }
}


void AnimSprite::Update() {
        if( m_play ) {
          m_frameTime -= App::GetApp()->GetDeltaTime();
          if( m_frameTime <= 0.0f ) {
                  NextFrame();
                  SetSubRect( m_animData->GetFrameRect( m_animation, m_frame ) );
                  m_frameTime = m_animData->GetFrameTime( m_animation, m_frame );
          }
  }
}


/************************************************
Protected Methods
************************************************/
bool AnimSprite::IsAnimating() {
  return m_play;
}
/************************************************
Private Methods
************************************************/

void AnimSprite::NextFrame() {
  if( !m_reverse ) {
          ++m_frame;
          if( m_frame >= m_animData->GetNumFrames( m_animation ) ) {
                  if( m_animData->IsLooped( m_animation ) ) {
                          m_frame = 0;
                  }
                  else {
        --m_frame;
                          Pause();
                  }
          }
  }
  else {
          --m_frame;
          if( m_frame == 0 ) {
                  if( m_animData->IsLooped( m_animation ) ) {
                          m_frame = m_animData->GetNumFrames( m_animation )-1;
                  }
                  else {
                          Pause();
                  }
          }
  }
}

