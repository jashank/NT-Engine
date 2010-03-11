#ifndef ANIMDATA_H
#define ANIMDATA_H

#include <SFML/Graphics/Image.hpp>
#include <string>

#include "BasicTypeDefs.h"

class AnimData {
public:
  AnimData();
  //AnimData( const std::string &filename );
  ~AnimData();

  bool IsLooped( Uint animation ) const;

  //float GetFrameTime( Uint animation ) const;
  float GetFrameTime( Uint animation, Uint frame ) const;

  int GetAnimX( Uint animation ) const;
  int GetAnimY( Uint animation ) const;
  int GetFrameHeight( Uint animation ) const;
  int GetFrameWidth( Uint animation ) const;
  Uint GetNumAnims() const;
  Uint GetNumFrames( Uint animation ) const;	

  sf::IntRect GetFrameRect( Uint animation,  Uint frame ) const;

  bool LoadFromFile( const std::string &filename );

private:
  struct Animation {
    Animation();
    ~Animation();
    bool uniqueFrameTimes; //True if each frame has it's own time
    bool isLooped; //True if animation is looped
    float *frameTime; //Individual times for each frame
    Uint numFrames; //Total frames in animation
    sf::IntRect	frameRect; //(x,y)-first frame (w,h)-frame sizes
  };

  Uint m_numAnims; //The number of Animations

  Animation *m_anims; //Holds information on all animations in animsheet
};

#endif