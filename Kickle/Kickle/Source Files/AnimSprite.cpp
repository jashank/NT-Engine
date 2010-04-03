#include "AnimSprite.h"

#include "App.h"

/************************************************
Private Methods
************************************************/
AnimSprite::AnimSprite() 
 : m_play( false ),
   m_reverse( false ),
   m_animation( 0 ),
   m_frameTime( 0.0f ),
   m_animData( 0 ),
   m_frame( 0 ) {
}

void AnimSprite::AnimateBackward() {
  m_reverse = true;
  Play();
}

void AnimSprite::AnimateForward() {
  m_reverse = false;
  Play();
}

const AnimData *AnimSprite::GetAnimData() const {
	  return m_animData;
}


Uint AnimSprite::GetFrame() const {
	return m_frame;
}


Uint AnimSprite::GetAnimation() const {
	return m_animation;
}

void AnimSprite::LoadAnimData( const std::string &filepath ) {
  App* app= App::GetApp();

  //Load/Set the animation data
  AnimData& anim = app->LoadAnim( filepath.c_str() );
  SetAnimData( anim );
}

void AnimSprite::Play() {
  if( m_animData ) {
  	m_play = true;
    //Set the AnimSprite's clip to the correct animation frame
  	SetSubRect( m_animData->GetFrameRect( m_animation, m_frame ) );
  }
}


void AnimSprite::Pause() {
	m_play = false;
}


void AnimSprite::Restart() {
  //If AnimSprite is animated
	if( m_animData ) {
    //Stop animation setting frame to 0
	  Stop();
    //Reset frame time
	  m_frameTime = m_animData->GetFrameTime( m_animation, m_frame );
    //Start animation again
	  Start();
  }
}


void AnimSprite::SetFrame( Uint frame ) {
	m_frame = frame;
}


void AnimSprite::SetAnimation( Uint animation ) {
  if( m_animation != animation ) {
		m_animation = animation;
    Restart();
  }
}


void AnimSprite::SetAnimData( const AnimData &animData ) {
	m_animData = &animData;
}


void AnimSprite::Start() {
	Play();
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
  //If AnimSprite is set to play
	if( m_play ) {
  
    //Count down the time to the next frame shift
	  m_frameTime -= App::GetApp()->GetDeltaTime();

    //If animation is ready to go to next frame
	  if( m_frameTime <= 0.0f ) {
      //Move on to the next frame
		  NextFrame();
      //Set the AnimSprite's clip to the correct animation frame
		  SetSubRect( m_animData->GetFrameRect( m_animation, m_frame ) );
      //Reset frame time
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
    //Increment frame
	  ++m_frame;

    //If the animation has reached it's end
	  if( m_frame >= m_animData->GetNumFrames( m_animation ) ) {
      //If animation is looped
		  if( m_animData->IsLooped( m_animation ) ) {
        //Reset animation to first frame
			  m_frame = 0;
		  }
		  else {
        //Set animation to last frame
        m_frame = m_animData->GetNumFrames( m_animation )-1;
        //Pause animation at last frame
			  Pause();
		  }
	  }
  }
  else {
    //Decrement frame
	  --m_frame;

    //If the animation has reached it's end
	  if( m_frame == 0 ) {
      //If animation is looped
		  if( m_animData->IsLooped( m_animation ) ) {
        //Reset animation to last frame
			  m_frame = m_animData->GetNumFrames( m_animation )-1;
		  }
		  else {
        //Set animation to last frame
        m_frame = 0;
        //Pause animation at last frame
			  Pause();
		  }
	  }
  }
}