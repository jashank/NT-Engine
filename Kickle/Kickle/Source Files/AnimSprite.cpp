#include "AnimSprite.h"

#include "App.h"

/************************************************
Private Methods
************************************************/
AnimSprite::AnimSprite() 
 : m_play( false ),
   m_animation( 0 ),
   m_frameTime( 0.0f ),
   m_animData( 0 ),
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
  App* app= App::GetApp();

  //Load/Set the animation data
  AnimData *anim = app->LoadAnim( filepath.c_str() );
  if ( anim ) {
    SetAnimData( anim );
  } else {
    LogErr( "Animation file failed to load in AnimSprite::LoadAnimData" );
  }
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
    //Stop animation
	  Stop();
    //Reset frame time
	  m_frameTime = m_animData->GetFrameTime( m_animation, m_frame );
    //Start animation again
	  Start();
  }
}


void AnimSprite::SetFrame( unsigned int frame ) {
	m_frame = frame;
}


void AnimSprite::SetAnimation( unsigned int animation ) {
  if( m_animData ) {
    if( m_animation != animation ) {
		  m_animation = animation;
      SetImage( m_animData->GetImage( m_animation ));
      Restart();
    }
  }
}


void AnimSprite::SetAnimData( const AnimData *animData ) {
	m_animData = animData;
  m_frameTime = m_animData->GetFrameTime( m_animation, m_frame );
  SetImage( m_animData->GetImage( m_animation ));
}


void AnimSprite::Start() {
	Play();
}


void AnimSprite::Stop() {
	Pause();
  if( m_animData->GetPlayBack( m_animation ) ) {
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
  if( m_animData->GetPlayBack( m_animation ) ) {
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
        --m_frame;
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
        //Pause animation at last frame
			  Pause();
		  }
	  }
  }
}