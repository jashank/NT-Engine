#include "AnimData.h"



#include <iostream>
#include <fstream>
#include <sstream>

#include "tinyxml.h"

#include "Utilities.h"

/************************************************
Public Methods
************************************************/
AnimData::AnimData()
 : m_numAnims( 0 ), 
   m_anims( 0 ) {

}


//AnimData::AnimData( const std::string &filename )
// : m_numAnims( 0 ), 
//   m_anims( 0 ) {
//  if( !LoadFromFile( filename ) ) {
//    throw "Unable to load animation data";
//  }
//}


AnimData::~AnimData() {
	SAFEDELETEA( m_anims );
}


bool AnimData::LoadFromFile( const std::string &filename ) {

  //<strip name="up_standing">
  //  <looped>true</looped>
  //  <num_frames>8</num_frames>
  //  <individual_times>false</individual_times>
  //  <frame_times> 0.1 </frame_times>

  //  <x_pos>0</x_pos>
  //  <y_pos>0</y_pos>
  //  <width>48</width>
  //  <height>60</height>
  //</strip>

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
  Uint i = 0;
  for( 
      ;
      (i < m_numAnims) && strip; 
      ++i, strip = strip->NextSiblingElement( "strip" ) 
  ) {

    //Load "looped"
    std::string looped = strip->FirstChildElement( "looped" )->GetText();
    //Set whether or not animation is looped
    m_anims[i].isLooped = (looped == "true") ? true : false;


    //Load "num_frames"
    ss.clear();
    ss << strip->FirstChildElement( "num_frames" )->GetText();
    //Set the number of frames
    ss >> m_anims[i].numFrames;
   
 
    //Load "individual_times"
    std::string ind = strip->FirstChildElement( "individual_times" )->GetText();
    m_anims[i].uniqueFrameTimes = (ind == "true") ? true : false;

    //Load "frame_times"
    ss.clear();
    ss << strip->FirstChildElement( "frame_times" )->GetText();

    //Allocate floats to hold the correct number of frame times
    if( m_anims[i].uniqueFrameTimes ) {
      m_anims[i].frameTime = new float[m_anims[i].numFrames];
      //Set the frame times
      for( Uint n = 0; n < m_anims[i].numFrames; ++n ) {
        ss >> m_anims[i].frameTime[n];
      }
    }
    else {
      m_anims[i].frameTime = new float(0.0f);
      //Set the frame time
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


bool AnimData::IsLooped( Uint animation ) const {
	return m_anims[animation%m_numAnims].isLooped;
}


//float AnimData::GetFrameTime( Uint animation ) const {
//  return GetFrameTime( animation, 0 );
//}


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
	return m_anims[animation%m_numAnims].frameRect.GetWidth();
}


int AnimData::GetFrameHeight( Uint animation ) const {
	return m_anims[animation%m_numAnims].frameRect.GetHeight();
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