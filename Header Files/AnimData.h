#ifndef ANIMDATA_H
#define ANIMDATA_H

#include <string>

#include <SFML/Graphics/Image.hpp>

class AnimData {
public:
  AnimData();
  AnimData( const AnimData &ad );
  ~AnimData();

  AnimData& operator=( const AnimData &ad );

  // Loads AnimData from an xml file
  bool LoadFromFile( const std::string &filename );

  // Returns true if given animation is looped
  bool IsLooped( unsigned int animation ) const;

  // Returns frame time for given animation and frame
  float GetFrameTime( unsigned int animation, unsigned int frame ) const;

  // Returns first frame's x and y for given animation
  int GetAnimX( unsigned int animation ) const;
  int GetAnimY( unsigned int animation ) const;

  // Returns frame height from given animation
  int GetFrameHeight( unsigned int animation ) const;

  // Returns frame width for given animation
  int GetFrameWidth( unsigned int animation ) const;

  //Returns number of animations held by AnimData
  unsigned int GetNumAnims() const;

  // Returns number of frames for given animation
  unsigned int GetNumFrames( unsigned int animation ) const;	

  // Returns true if played forward false if played backward
  bool GetPlayBack( unsigned int animation ) const;

  // Returns the Rect for the given animation and frame
  sf::IntRect GetFrameRect( unsigned int animation,  unsigned int frame ) const;

  // Returns the animation's image
  sf::Image& GetImage( unsigned int animation ) const;

private:
  struct Animation {
    Animation();
    Animation( const Animation &a );
    ~Animation();

    Animation& operator=( const Animation &a );

    bool forward;
    bool uniqueFrameTimes; //True if each frame has it's own time
    bool isLooped; //True if animation is looped
    float *frameTime; //Individual times for each frame
    unsigned int numFrames; //Total frames in animation
    sf::IntRect	frameRect; //(x,y)-first frame (w,h)-frame sizes
    sf::Image *image; // Holds pointer to image designated to animation (stored in ResourceManager)
  };

  Animation *m_anims; //Holds information on all animations in animsheet
  unsigned int m_numAnims; //The number of Animations
};

#endif