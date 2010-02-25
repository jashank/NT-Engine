#include "GameObject.h"

#include <iostream>

#include "tinyxml.h"

#include "AnimData.h"
#include "App.h"

/************************************************
Public Methods
************************************************/
GameObject::GameObject( const std::string &xmlGameObjectPath )
 : m_play( false ),
   m_animation( 0 ),
   m_frameTime( 0.0f ),
   m_animData( 0 ) {

  TiXmlDocument doc ( xmlGameObjectPath.c_str() );
  
  if ( !doc.LoadFile() ) {
    throw "Cannot load GameObject XML file";
  }

  TiXmlHandle handleDoc( &doc );
  TiXmlElement* root = handleDoc.FirstChildElement( "game_object" ).Element();

  //Load in path to GameObject's spritesheet
  std::string spritePath( root->FirstChildElement( "sprite_path" )->GetText() );

  //Load in path to animation's xml
  std::string animPath( root->FirstChildElement( "animation_path" )->GetText() );

  //Load in path to GameObject's lua script
  std::string luaScript( root->FirstChildElement( "script_path" )->GetText() );


  App* app= App::GetApp();

  //Load the spritesheet image
  sf::Image& img = app->LoadImageW( spritePath.c_str() );
  //Set all pink pixels to be treated as clear
  img.CreateMaskFromColor( sf::Color( 255, 0, 255 ) );
  //Set the spritesheet image
  SetImage( img );

  //Load/Set the animation data
  AnimData& anim = app->LoadAnim( animPath.c_str() );
  SetAnimData( anim );

  //TODO - stick loading lua script here
}


GameObject::~GameObject() {

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
		m_animation = animation;
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
