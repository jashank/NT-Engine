#include "GameObject.h"

#include <iostream>

#include "AnimData.h"
#include "App.h"

/************************************************
Public Methods
************************************************/
GameObject::GameObject( const std::string &filename )
 : m_play( false ),
   m_animation( 0 ),
   m_frameTime( 0.0f ),
   m_animData( 0 ) {

  //TESTCODE
  sf::Image& temp = App::GetApp()->LoadImageW( "./Content/Core/Sprites/kickle_48x60.png" );
  temp.CreateMaskFromColor( sf::Color( 255, 0, 255 ) );
  SetImage( temp );
  m_animData = new AnimData();
}


GameObject::~GameObject() {
  //TESTCODE
  SAFEDELETE( m_animData );
}


const AnimData *GameObject::GetAnimData() const {
	  return m_animData;
}


Uint GameObject::GetFrame() const {
	return m_frame;
}


Uint GameObject::GetAnimation() const {
	return m_animation;
}


void GameObject::Play() {
  if( m_animData ) {
  	m_play = true;
    //Set the GameObject's clip to the correct animation frame
  	SetSubRect( m_animData->GetFrameRect( m_animation, m_frame ) );
  }
}


void GameObject::Pause() {
	m_play = false;
}


void GameObject::Restart() {
  //If GameObject is animated
	if( m_animData ) {
    //Stop animation setting frame to 0
	  Stop();
    //Reset frame time
	  m_frameTime = m_animData->GetFrameTime( m_animation, 0 );
    //Start animation again
	  Start();
  }
}


void GameObject::SetFrame( Uint frame ) {
	m_frame = frame;
}


void GameObject::SetAnimation( Uint animation ) {
  //If GameObject is animated
	//if( m_animData ) {
		m_animation = animation;
	//}
}


void GameObject::SetAnimData( const AnimData &animData ) {
	m_animData = &animData;
}


void GameObject::Start() {
	Play();
}


void GameObject::Stop() {
	Pause();
  SetFrame( 0 );
}


void GameObject::Update() {
  AnimUpdate();
}


/************************************************
Private Methods
************************************************/
void GameObject::AnimUpdate() {
  //If GameObject is set to play
	if( m_play ) {
  
    //Count down the time to the next frame shift
	  m_frameTime -= App::GetApp()->GetDeltaTime();

    //If animation is ready to go to next frame
	  if( m_frameTime <= 0.0f ) {
      //Move on to the next frame
		  NextFrame();
      //Set the GameObject's clip to the correct animation frame
		  SetSubRect( m_animData->GetFrameRect( m_animation, m_frame ) );
      //Reset frame time
		  m_frameTime = m_animData->GetFrameTime( m_animation, m_frame );
	  }
  }
} 

void GameObject::NextFrame() {
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
