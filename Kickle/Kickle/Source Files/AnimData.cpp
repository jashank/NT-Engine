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

AnimData::AnimData( const AnimData& ad )
 : m_anims( 0 ), 
   m_numAnims( ad.m_numAnims ) {
  m_anims = new Animation[m_numAnims];
  for( unsigned int i = 0; i < m_numAnims; ++i ) {
    m_anims[i] = ad.m_anims[i];
  }
}

AnimData::~AnimData() {
	SAFEDELETEA( m_anims );
}

AnimData& AnimData::operator=( const AnimData& ad ) {
  if( this != &ad ) {
    m_numAnims = ad.m_numAnims;
    m_anims = new Animation[m_numAnims];
    for( unsigned int i = 0; i < m_numAnims; ++i ) {
      m_anims[i] = ad.m_anims[i];
    }
  }

  return *this;
}

bool AnimData::LoadFromFile( const std::string &filename ) {

  TiXmlDocument doc ( filename.c_str() );
  
  if ( !doc.LoadFile() ) {
    return false;
  }
  
  std::stringstream ss; //used for converting strings to other data types
  TiXmlHandle handleDoc( &doc ); //handle to the xml doc
  
  TiXmlElement* root = handleDoc.FirstChildElement( "anim_strips" ).Element();
   
  std::vector<Animation> tempArray;

  TiXmlElement* strip = root->FirstChildElement( "strip" );
  unsigned int i = 0;
  for( ; strip; strip = strip->NextSiblingElement( "strip" ) ) {
    Animation temp;

    //Load "playback"
    TiXmlElement* tempElem = strip->FirstChildElement( "playback" );
    if( tempElem ) {
      std::string playback = tempElem->GetText();
      std::transform( 
        playback.begin(), 
        playback.end(), 
        playback.begin(), 
        std::tolower );
      temp.forward = (playback == "forward") ? true : false;
    }

    //Load "looped"
    tempElem = strip->FirstChildElement( "looped" );
    if( tempElem ) {
      std::string loop = tempElem->GetText();
      std::transform( loop.begin(), loop.end(), loop.begin(), std::tolower );
      temp.isLooped = (loop == "true") ? true : false;
    }

    //Load "num_frames"
    ss.clear();
    ss << strip->FirstChildElement( "num_frames" )->GetText();
    ss >> temp.numFrames;
   
 
    //Load "individual_times"
    //std::string ind = strip->FirstChildElement( "individual_times" )->GetText();
    //std::transform( ind.begin(), ind.end(), ind.begin(), std::tolower );
    //temp.uniqueFrameTimes = (ind == "true") ? true : false;

    //Load "frame_times"
    ss.clear();
    ss << strip->FirstChildElement( "frame_times" )->GetText();

    //Converts frame_times to floats
    std::vector<float> tempTimes;
    float t = 0.0f;
    for( ;!ss.eof(); ) {        
      ss >> t;
      tempTimes.push_back( t );
    }

    //Determine if there is a time for every frame or a single constant time
    //then move the time(s) from the temporary vector to the array
    unsigned int numTimes = static_cast<unsigned int>(tempTimes.size());
    if( numTimes != 1 ) {
      if( numTimes != temp.numFrames ) {
        // Incorrect number of time entries for the frame_times tag 
        // There needs to be either 1 time or numFrames number of times
        return false;
      }
      temp.uniqueFrameTimes = true;
      temp.frameTime = new float[temp.numFrames];
      for( unsigned int i = 0; i < temp.numFrames; ++i ) {
        temp.frameTime[i] = tempTimes[i];
      }
    }
    else {
      temp.uniqueFrameTimes = false;
      temp.frameTime = new float;
      temp.frameTime[0] = tempTimes[0];
    }

    
    //Load "x_pos"
    ss.clear();
    ss << strip->FirstChildElement( "x_pos" )->GetText();
    ss >> temp.frameRect.Left;

    //Load "y_pos"
    ss.clear();
    ss << strip->FirstChildElement( "y_pos" )->GetText();
    ss >> temp.frameRect.Top;

    //Load "width"
    ss.clear();
    ss << strip->FirstChildElement( "width" )->GetText();
    ss >> temp.frameRect.Right;
    temp.frameRect.Right += temp.frameRect.Left;

    //Load "height"
    ss.clear();
    ss << strip->FirstChildElement( "height" )->GetText();
    ss >> temp.frameRect.Bottom;
    temp.frameRect.Bottom += temp.frameRect.Top;

    tempArray.push_back(temp);
  }


  m_numAnims = static_cast<unsigned int>(tempArray.size());
  //Allocate the correct number of animation strips
  m_anims = new Animation[m_numAnims];

  for( unsigned int i = 0; i < m_numAnims; ++i ) {
    m_anims[i] = tempArray[i];
  }
  
  

  return true;
}


bool AnimData::IsLooped( unsigned int animation ) const {
	return m_anims[animation%m_numAnims].isLooped;
}


float AnimData::GetFrameTime( unsigned int animation, unsigned int frame ) const {
  static Animation* a = 0;
  a = &m_anims[animation%m_numAnims];

	if( a->uniqueFrameTimes ) {
		return a->frameTime[frame%(a->numFrames)];
	}
	return a->frameTime[0];
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

sf::IntRect AnimData::GetFrameRect( 
  unsigned int animation, 
  unsigned int frame ) const {
	static sf::IntRect rect;
  static Animation* a = 0;

  a = &m_anims[animation%m_numAnims];
	rect.Top = a->frameRect.Top;
	rect.Left = a->frameRect.Left + (frame%a->numFrames) * 
    a->frameRect.GetWidth();
	rect.Bottom = a->frameRect.Bottom;
	rect.Right = rect.Left + a->frameRect.GetWidth();

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

AnimData::Animation::Animation( const AnimData::Animation& a )
: forward( a.forward ),
  uniqueFrameTimes( a.uniqueFrameTimes ),
  isLooped( a.isLooped ),
  frameTime( 0 ),
  numFrames( a.numFrames ),
  frameRect( a.frameRect ) {
    if( uniqueFrameTimes ) {
      frameTime = new float[numFrames];
      for( unsigned int i = 0; i < numFrames; ++i ) {
        frameTime[i] = a.frameTime[i];
      }
    }
    else {
      frameTime = new float( a.frameTime[0] );
    }
}

AnimData::Animation::~Animation() {
	SAFEDELETEA( frameTime );
}

AnimData::Animation& AnimData::Animation::operator=( const AnimData::Animation& a ) {
  if( this != &a ){
    forward = a.forward;
    uniqueFrameTimes = a.uniqueFrameTimes;
    isLooped = a.isLooped; 
    numFrames = a.numFrames;
    frameRect = a.frameRect;

    SAFEDELETEA(frameTime);
    if( uniqueFrameTimes ) {
      frameTime = new float[numFrames];
      for( unsigned int i = 0; i < numFrames; ++i ) {
        frameTime[i] = a.frameTime[i];
      }
    }
    else {
      frameTime = new float( a.frameTime[0] );
    }
  }

  return *this;
}
