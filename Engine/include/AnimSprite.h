/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef NT_ANIMSPRITE_H
#define NT_ANIMSPRITE_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "AnimData.h"
#include "Lerpable.h"

namespace nt {

/**
 * Lerpable sprite with methods to enable easier animation based on AnimData
 * loaded in.
 */
class AnimSprite : public Lerpable {
 public:
  /**
   * Animation and frame are initialized to invalid values. User must set
   * these (note that SetAnimation will set the correct starting frame), 
   * otherwise sprite will display nothing. 
   */
  AnimSprite();

  /**
   * Goes to next frame in sprite's animation sequence if it is time to do so.
   * This is also the time when the sprite retrieves its position for lastPos
   * so move it after calling Update.
   * @param dt delta time value - amount of time to step forward
   */
  void Update( float dt );

  /**
   * @return Number of current frame sprite is on.
   */
  int GetFrame() const;

  /**
   * Returns frame height of current frame sprite is on. If no animation
   * data stored returns 0.
   */
  int GetFrameHeight() const;

  /**
   * @return Index where sprite's current animation is stored in its AnimData.
   */
  int GetAnimation() const;

  /**
   * Loads AnimData from file located at path passed and sets sprite's
   * AnimData to what is loaded in.
   * @param filepath path to file to be loaded in.
   */ 
  void LoadAnimData( const std::string &filepath );

  /** 
   * Plays animation sprite is currently set to. If sprite is paused in middle
   * of animation, resumes from that point.
   */
  void Play();

  /**
   * Pauses animation sprite is current playing.
   */
  void Pause();

  /**
   * Stops animation and then plays it. 
   */
  void Restart();

  /** 
   * Pauses animation, then sets frame to first clip if animation is playing
   * forwards, and last if the animation is playing backwards. 
   */
  void Stop();

  /**
   * @param reversed whether sprite's animation is to be played in reverse.
   */
  void SetReverse( bool reversed );

  /**
   * Sets frame number, adjusting sheet clip accordingly. 
   * @param frameNum nth frame in sprite's current animation to set sprite's
   * current frame to. Note that frame count starts at 0.
   */
  void SetFrame( int frameNum );

  /**
   * If new animation is set, animation will play forward unless SetReverse
   * was called before SetAnimation, but after the last Update. Frame is set
   * to correct starting position. Does nothing if animation passed is
   * current animation. 
   * @param animIndex index into animations for this sprite.
   */
  void SetAnimation( int animIndex );

  /**
   * Sets sprite's AnimData. Current animation index is not changed. 
   * @param animData new AnimData for this sprite.
   */
  void SetAnimData( const boost::shared_ptr<AnimData> &anim );
  
  /**
   * @return whether sprite's animation is playing.
   */
  bool IsAnimating();

  /**
   * Sets sprite's alpha channel to argument passed.
   */
  void SetAlpha( unsigned int alpha );

 private:
  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  AnimSprite( const AnimSprite &sprite );
  AnimSprite& operator=( const AnimSprite &sprite );
  //@}

  /**
   * Sets sprite's current clip to the next frame in the animation sequence.
   */
  void NextFrame();

  /**
   * Overrides Lerpable's Render to deal with this being an
   * animated sprite.
   */
  void Render( sf::RenderTarget &target ) const;

  /** Animation data stored for this sprite. */
  boost::shared_ptr<AnimData> m_animData;

  bool m_playing; /** Whether animation is playing. */
  bool m_reversed; /** Whether animation is to be played in reverse. */
  bool m_setToReverse; /** Enables newly set animation to start in reverse. */
  float m_frameTimeLeft; /** Time left for current frame to play. */
  int m_animNum; /** Number of animation sprite is currently on. */
  int m_frameNum; /** Number of frame sprite is currently on. */ 
};

} // namespace nt

#endif // NT_ANIMSPRITE_H

