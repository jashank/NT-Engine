#ifndef NT_ANIMDATA_H
#define NT_ANIMDATA_H

#include <string>

#include <boost/shared_ptr.hpp>
#include <SFML/Graphics/Image.hpp>

#include "Rect.h"

class TiXmlElement;

namespace nt {
  
/**
 * Stores and provides information on animations parsed in from animation file.
 */
class AnimData {
public:
  AnimData() {}
  ~AnimData() {}

  /**
   * Loads all animations from an animation file.
   * @param filepath path to the file to be loaded. 
   * @return True if all of file is successfully loaded.
   */
  bool LoadFromFile( const std::string &filepath );

  /**
   * @param animIndex index into animations stored by AnimData.
   * Validity of index is checked.
   * @return True if index is valid and animation at index is looped.
   */
  bool IsLooped( int animIndex ) const;

  /**
   * @param animIndex index into animations stored by AnimData. Validity
   * of index is checked.
   * @param frameIndex index into frame times that Animation is storing.
   * Validity of index is checked.
   * @return Time (in seconds) to be spent rendering frame at frameIndex. 
   * Time of first frame if frameIndex is invalid or there are no unique frame 
   * times, and 0 if animIndex is invalid.  
   */
  float GetFrameTime( int animIndex, int frameIndex ) const;
 
  /**
   * @param animIndex index into animations stored by AnimData. Validity  
   * of index is checked.
   * @return (x,y) pixel coordinates of where the animation is located on the 
   * sheet it was drawn on. (0,0) if index is invalid.
   */
  IntPoint GetAnimPosition( int animIndex ) const;

  //@{
  /**
   * @param animIndex index into animations stored by AnimData. Validity 
   * of index is checked.
   * @return Height and width in pixels of single frame in animation. 0 if 
   * index is invalid.
   */
  int GetFrameWidth( int animIndex ) const;
  int GetFrameHeight( int animIndex ) const;
  //@}
  
  /**
   * @return Number of animations held.
   */
  int GetNumAnims() const;

  /**
   * @param animIndex index into animations stored by AnimData. Validity 
   * of index is checked.
   * @return Number of frames in animation. 0 if index is invalid. 
   */
  int GetNumFrames( int animIndex ) const;

  /**
   * @param animIndex index into animations stored by AnimData. Validity
   * of index is checked.
   * @param frameIndex index into frames of animation. Validity of index is
   * checked.
   * @return Rectangle containing top, left, right, and bottom pixel coordinates 
   * of frame on sheet it is located on. Rect of first frame if frame index is
   * invalid and (0,0,0,0) rect if animation index is invalid.
   */
  IntRect GetFrameRect( int animIndex, int frameIndex ) const;

  /**
   * @param animIndex index into animations stored by AnimData. Validity
   * of index is checked. Throws range_error exception if index is invalid.
   * @return Sheet that animation was drawn on.
   */
  const boost::shared_ptr<sf::Image> &GetImage( int animIndex ) const;

private:
  /**
   * Holds data for an animation.
   */
  struct Animation {
    Animation();
    ~Animation() {}

    bool uniqueFrameTimes; /** True if each frame has its own time. */ 
    bool isLooped; /** True if animation loops back on itself. */ 
    std::vector<float> frameTimes; /** Only stores 1 time if no unique times. */
    int numFrames; /** Number of frames in animation. */
    IntRect frameRect; /** (x,y)-first frame (w,h)-frame sizes */
    boost::shared_ptr<sf::Image> image; /** Image that animation is on. */
    std::string name; /** Name assigned to animation in animation file. */
  };

  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  AnimData( const AnimData &data );
  AnimData& operator=( const AnimData &data );
  //@}

  /**
   * Parses strip passed, adding animation to m_anims.
   * @param strip XML strip element to be parsed.
   * @param sheet image sheet that animation is located on.
   * @return Whether strip was successfully parsed.
   */
  bool ParseStrip(
    const TiXmlElement *strip,
    const boost::shared_ptr<sf::Image> &sheet
  );

  std::vector<Animation> m_anims; /** Stores all animations. */
};

} // namespace nt

#endif // NT_ANIMDATA_H

