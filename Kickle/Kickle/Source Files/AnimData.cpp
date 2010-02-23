#include <iostream>
#include <fstream>

#include "AnimData.h"
#include "Utilities.h"

/************************************************
Public Methods
************************************************/
AnimData::AnimData()
 : m_numAnims( 0 ), 
   m_anims( 0 ) {

  //struct Animation {
  //  Animation();
  //  ~Animation();
  //  bool uniqueFrameTimes; //True if each frame has it's own time
  //  bool isLooped; //True if animation is looped
  //  float *frameTime; //Individual times for each frame
  //  Uint numFrames; //Total frames in animation
  //  sf::IntRect	frameRect; //(x,y)-first frame (w,h)-frame sizes
  //};

  //int m_numAnims; //The number of Animations

  //Animation *m_anims; //Holds information on all animations in animsheet

  //HARDCODED -  just until lua is embedded
  m_numAnims = 20;
  m_anims = new Animation[m_numAnims];
  for( int i = 0; i < m_numAnims; ++i ) {
    m_anims[i].numFrames = 8;
    m_anims[i].frameRect.Left = 0;
    m_anims[i].frameRect.Right = 48;
    m_anims[i].frameRect.Top = i*60;
    m_anims[i].frameRect.Bottom = (i+1)*60;

    m_anims[i].isLooped = true;
    m_anims[i].uniqueFrameTimes = false;
    m_anims[i].frameTime = new float(0.1f);
  }
}


AnimData::AnimData( const std::string &filename )
 : m_numAnims( 0 ), 
   m_anims( 0 ) {
  LoadFromFile( filename );
}


AnimData::~AnimData() {
	SAFEDELETEA( m_anims );
}


void AnimData::LoadFromFile( const std::string &filename ) {
}


bool AnimData::IsLooped( Uint animation ) const {
	return m_anims[animation%m_numAnims].isLooped;
}


float AnimData::GetFrameTime( Uint animation, Uint frame ) const {
	if( m_anims[animation].uniqueFrameTimes ) {
		return m_anims[animation%m_numAnims].
			frameTime[frame%(m_anims[animation%m_numAnims].numFrames)];
	}
	return m_anims[animation%m_numAnims].frameTime[0];
}


int AnimData::GetAnimX( Uint animation ) const {
	return m_anims[animation%m_numAnims].frameRect.Left;
}


int AnimData::GetAnimY( Uint animation ) const {
	return m_anims[animation%m_numAnims].frameRect.Top;
}

int AnimData::GetFrameWidth( Uint animation ) const {
	return m_anims[animation%m_numAnims].frameRect.Right;
}


int AnimData::GetFrameHeight( Uint animation ) const {
	return m_anims[animation%m_numAnims].frameRect.Bottom;
}


Uint AnimData::GetNumAnims() const {
	return m_numAnims;
}


Uint AnimData::GetNumFrames( Uint animation ) const {
	return m_anims[animation%m_numAnims].numFrames;
}


sf::IntRect AnimData::GetFrameRect( Uint animation, Uint frame ) const {
	static sf::IntRect rect;

  //Uint a = (frame%m_anims[animation%m_numAnims].numFrames) * m_anims[animation%m_numAnims].frameRect.Right;
  //Uint f = m_anims[animation%m_numAnims].frameRect.Left;
 

	rect.Top = m_anims[animation%m_numAnims].frameRect.Top;
	rect.Left = m_anims[animation%m_numAnims].frameRect.Left + 
    (frame%m_anims[animation%m_numAnims].numFrames) * 
    m_anims[animation%m_numAnims].frameRect.Right;
	rect.Bottom = m_anims[animation%m_numAnims].frameRect.Bottom;
	rect.Right = rect.Left + m_anims[animation%m_numAnims].frameRect.Right;

	return rect;
}


/************************************************
Private Methods
************************************************/
AnimData::Animation::Animation()
: frameTime( 0 ) {
}


AnimData::Animation::~Animation() {
	SAFEDELETEA( frameTime );
}