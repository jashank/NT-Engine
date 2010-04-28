#include "AnimData.h"


#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>

#include "tinyxml.h"

#include "Utilities.h"

/************************************************
Public Methods
************************************************/
AnimData::AnimData()
 : m_anims( 0 ), 
   m_numAnims( 0 ) {

}


AnimData::~AnimData() {
	SAFEDELETEA( m_anims );
}


bool AnimData::LoadFromFile( const std::string &filename ) {

  TiXmlDocument doc ( filename.c_str() );
  
  if ( !doc.LoadFile() ) {
    return false;
  }
  
  std::stringstream ss; //used for converting strings to other data types
  TiXmlHandle handleDoc( &doc ); //handle to the xml doc
  
  TiXmlElement* root = handleDoc.FirstChildElement( "anim_strips" ).Element();
  
  //Load the number of animations into a stringstream
  ss << root->FirstChildElement( "num_anims" )->GetText();
  //Set the number of animations
  ss >> m_numAnims;

  //Allocate the correct number of animation strips
  m_anims = new Animation[m_numAnims];
 
  TiXmlElement* strip = root->FirstChildElement( "strip" );
  unsigned int i = 0;
  for( ;
      (i < m_numAnims) && strip; 
      ++i, strip = strip->NextSiblingElement( "strip" ) ) {


    //Load "playback"
    TiXmlElement* tempElem = strip->FirstChildElement( "playback" );
    if( tempElem ) {
      std::string playback = tempElem->GetText();
      std::transform( 
        playback.begin(), 
        playback.end(), 
        playback.begin(), 
        std::tolower );
      m_anims[i].forward = (playback == "forward") ? true : false;
    }

    //Load "looped"
    tempElem = strip->FirstChildElement( "looped" );
    if( tempElem ) {
      std::string loop = tempElem->GetText();
      std::transform( loop.begin(), loop.end(), loop.begin(), std::tolower );
      m_anims[i].isLooped = (loop == "true") ? true : false;
    }

    //Load "num_frames"
    ss.clear();
    ss << strip->FirstChildElement( "num_frames" )->GetText();
    ss >> m_anims[i].numFrames;
   
 
    //Load "individual_times"
    std::string ind = strip->FirstChildElement( "individual_times" )->GetText();
    std::transform( ind.begin(), ind.end(), ind.begin(), std::tolower );
    m_anims[i].uniqueFrameTimes = (ind == "true") ? true : false;

    //Load "frame_times"
    ss.clear();
    ss << strip->FirstChildElement( "frame_times" )->GetText();

    //Allocate floats to hold the correct number of frame times
    if( m_anims[i].uniqueFrameTimes ) {
      m_anims[i].frameTime = new float[m_anims[i].numFrames];
      for( unsigned int n = 0; n < m_anims[i].numFrames; ++n ) {
        ss >> m_anims[i].frameTime[n];
      }
    }
    else {
      m_anims[i].frameTime = new float(0.0f);
      ss >> m_anims[i].frameTime[0];
    }

    
    //Load "x_pos"
    ss.clear();
    ss << strip->FirstChildElement( "x_pos" )->GetText();
    ss >> m_anims[i].frameRect.Left;

    //Load "y_pos"
    ss.clear();
    ss << strip->FirstChildElement( "y_pos" )->GetText();
    ss >> m_anims[i].frameRect.Top;

    //Load "width"
    ss.clear();
    ss << strip->FirstChildElement( "width" )->GetText();
    ss >> m_anims[i].frameRect.Right;
    m_anims[i].frameRect.Right += m_anims[i].frameRect.Left;

    //Load "height"
    ss.clear();
    ss << strip->FirstChildElement( "height" )->GetText();
    ss >> m_anims[i].frameRect.Bottom;
    m_anims[i].frameRect.Bottom += m_anims[i].frameRect.Top;
  }


  return true;
}


bool AnimData::IsLooped( unsigned int animation ) const {
	return m_anims[animation%m_numAnims].isLooped;
}


float AnimData::GetFrameTime( unsigned int animation, unsigned int frame ) const {
	if( m_anims[animation].uniqueFrameTimes ) {
		return m_anims[animation%m_numAnims].
			frameTime[frame%(m_anims[animation%m_numAnims].numFrames)];
	}
	return m_anims[animation%m_numAnims].frameTime[0];
}


int AnimData::GetAnimX( unsigned int animation ) const {
	return m_anims[animation%m_numAnims].frameRect.Left;
}


int AnimData::GetAnimY( unsigned int animation ) const {
	return m_anims[animation%m_numAnims].frameRect.Top;
}

int AnimData::GetFrameWidth( unsigned int animation ) const {
	return m_anims[animation%m_numAnims].frameRect.GetWidth();
}


int AnimData::GetFrameHeight( unsigned int animation ) const {
	return m_anims[animation%m_numAnims].frameRect.GetHeight();
}


unsigned int AnimData::GetNumAnims() const {
	return m_numAnims;
}


unsigned int AnimData::GetNumFrames( unsigned int animation ) const {
	return m_anims[animation%m_numAnims].numFrames;
}

bool AnimData::GetPlayBack( unsigned int animation ) const {
  return m_anims[animation%m_numAnims].forward;
}

sf::IntRect AnimData::GetFrameRect( unsigned int animation, unsigned int frame ) const {
	static sf::IntRect rect;

  //unsigned int a = (frame%m_anims[animation%m_numAnims].numFrames) * m_anims[animation%m_numAnims].frameRect.Right;
  //unsigned int f = m_anims[animation%m_numAnims].frameRect.Left;
 

	rect.Top = m_anims[animation%m_numAnims].frameRect.Top;
	rect.Left = m_anims[animation%m_numAnims].frameRect.Left + 
    (frame%m_anims[animation%m_numAnims].numFrames) * 
    m_anims[animation%m_numAnims].frameRect.GetWidth();
	rect.Bottom = m_anims[animation%m_numAnims].frameRect.Bottom;
	rect.Right = rect.Left + m_anims[animation%m_numAnims].frameRect.GetWidth();

	return rect;
}


/************************************************
Private Methods
************************************************/
AnimData::Animation::Animation()
: forward( true ),
  uniqueFrameTimes( false ),
  isLooped( false ),
  frameTime( 0 ),
  numFrames( 0 ) {
}


AnimData::Animation::~Animation() {
	SAFEDELETEA( frameTime );
}