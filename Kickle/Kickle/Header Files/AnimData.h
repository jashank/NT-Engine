#ifndef ANIMDATA_H
#define ANIMDATA_H

#include <SFML/Graphics/Image.hpp>
#include <string>

#include "BasicTypeDefs.h"

class AnimData {
public:
  AnimData();
  ~AnimData();

  // Returns true if given animation is looped
  bool IsLooped( Uint animation ) const;

  // Returns frame time for given animation and frame
  float GetFrameTime( Uint animation, Uint frame ) const;

  // Returns first frame's x and y for given animation
  int GetAnimX( Uint animation ) const;
  int GetAnimY( Uint animation ) const;

  // Returns frame height from given animation
  int GetFrameHeight( Uint animation ) const;

  // Returns frame width for given animation
  int GetFrameWidth( Uint animation ) const;

  //Returns number of animations held by AnimData
  Uint GetNumAnims() const;

  // Returns number of frames for given animation
  Uint GetNumFrames( Uint animation ) const;	

  // Returns true if played forward false if played backward
  bool GetPlayBack( Uint animation ) const;

  // Returns the Rect for the given animation and frame
  sf::IntRect GetFrameRect( Uint animation,  Uint frame ) const;

  // Loads AnimData from an xml file
  bool LoadFromFile( const std::string &filename );

private:
  struct Animation {
    Animation();
    ~Animation();
    bool forward;
    bool uniqueFrameTimes; //True if each frame has it's own time
    bool isLooped; //True if animation is looped
    float *frameTime; //Individual times for each frame
    Uint numFrames; //Total frames in animation
    sf::IntRect	frameRect; //(x,y)-first frame (w,h)-frame sizes
  };

  Animation *m_anims; //Holds information on all animations in animsheet
  Uint m_numAnims; //The number of Animations
};

#endif