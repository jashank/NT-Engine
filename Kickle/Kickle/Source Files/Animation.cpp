#include "Animation.h"
#include <algorithm>
#include <sstream>
#include "tinyxml.h"

/**************************
Public Methods
**************************/
Animation::Animation( const TiXmlElement *strip ) {
  LoadStrip( strip );
}


void Animation::LoadStrip( const TiXmlElement *strip ) {
  m_name = strip->Attribute( "name" );
  m_forward = ( strip->FirstChildElement( "playback" )->GetText() == "forward");
  m_looped = ( strip->FirstChildElement( "looped" )->GetText() == "true" );

  std::stringstream ss;
  ss << strip->FirstChildElement( "num_frames" )->GetText();
  ss >> m_numFrames;

  ss.clear();
  ss << strip->FirstChildElement( "individual_times" )->GetText();
  ss >> m_frameTimes;
 
  std::string ind = strip->FirstChildElement( "individual_times" )->GetText();
  std::transform( ind.begin(), ind.end(), ind.begin(), std::tolower );
  m_uniqueFrameTimes = (ind == "true");

  ss.clear();
  ss << strip->FirstChildElement( "frame_times" )->GetText();
  if( m_uniqueFrameTimes ) {
    for( unsigned int n = 0; n < m_numFrames; ++n ) {
      ss >> m_frameTimes[n];
    }
  } else {
    ss >> m_frameTimes[0];
  }

  ss.clear();
  ss << strip->FirstChildElement( "x_pos" )->GetText();
  ss >> m_frameRect.Left;

  ss.clear();
  ss << strip->FirstChildElement( "y_pos" )->GetText();
  ss >> m_frameRect.Top;

  ss.clear();
  ss << strip->FirstChildElement( "width" )->GetText();
  ss >> m_frameRect.Right;
  m_frameRect.Right += loadedAnim.frameRect.Left;

  ss.clear();
  ss << strip->FirstChildElement( "height" )->GetText();
  ss >> loadedAnim.frameRect.Bottom;
  m_frameRect.Bottom += loadedAnim.frameRect.Top;
}
