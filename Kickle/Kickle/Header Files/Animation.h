#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>

class TiXmlElement;

/****************************************************************
 Animation
 - An animation, specified by the 'strip' attribute in XML files.
*****************************************************************/
class Animation {
 public:
  // Constructs animation from strip passed (calls LoadStrip)
  Animation( const TiXmlElement *strip );

  // Loads in information from animation strip passed
  void LoadStrip( const TiXmlElement *strip ); 

 private:
  std::string m_name; // Name of animation
  bool m_forward; // True if animation is playing forwards
  bool m_looped; //True if animation is looped
  unsigned int m_numFrames; //Total frames in animation
  bool m_uniqueFrameTimes; //True if each frame has it's own time
  std::vector<float> m_frameTimes; //Individual times for each frame
  sf::IntRect	m_frameRect; //(x,y)-first frame (w,h)-frame sizes
};

#endif