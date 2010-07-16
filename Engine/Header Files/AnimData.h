#ifndef ANIMDATA_H
#define ANIMDATA_H

#include <string>

#include <SFML/Graphics/Image.hpp>
class TiXmlElement;

class AnimData {
public:
  AnimData() {}
  ~AnimData() {}

  /// Loads AnimData from an xml file
  bool LoadFromFile( const std::string &filename );

  /// Returns true if given animation is looped
  bool IsLooped( int animation ) const;

  /// Returns frame time for given animation and frame
  float GetFrameTime( int animation, int frame ) const;

  /// Returns first frame's x and y for given animation
  int GetAnimX( int animation ) const;
  int GetAnimY( int animation ) const;

  /// Returns frame height from given animation
  int GetFrameHeight( int animation ) const;

  /// Returns frame width for given animation
  int GetFrameWidth( int animation ) const;

  /// Returns number of animations held by AnimData
  int GetNumAnims() const;

  /// Returns number of frames for given animation
  int GetNumFrames( int animation ) const;

  /// Returns the Rect for the given animation and frame
  sf::IntRect GetFrameRect( int animation,  int frame ) const;

  /// Returns the animation's image
  sf::Image& GetImage( int animation ) const;

private:
  enum CommonTag { ON, OFF };

  struct Animation {
    Animation();
    ~Animation() {}

    bool uniqueFrameTimes; /// True if each frame has it's own time
    bool isLooped; ///True if animation is looped
    std::vector<float> frameTimes;
    int numFrames; ///Total frames in animation
    sf::IntRect	frameRect; ///(x,y)-first frame (w,h)-frame sizes
    sf::Image *image; /// Holds pointer to image designated to animation (stored in ResourceManager)
    std::string name; /// Name of animation loaded in
  };

  /// Restricts copy constructor, and assignment.
  AnimData( const AnimData &data );
  AnimData& operator=( const AnimData &data );

  /// Parses strip passed, adding animation to m_anims. If flag is ON, function
  /// assumes that strip has parent <common>. Otherwise it doesn't
  /// (direct child of <sheet>). Must also pass sheet that strip is under.
  /// Returns whether parsing was successful.
  bool ParseStrip( const TiXmlElement *strip, sf::Image *sheet, CommonTag flag );

  Animation m_common; /// Data from last <common> tag loaded in
  std::vector<Animation> m_anims; /// Holds information on all animations
};

#endif
