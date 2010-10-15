#ifndef ANIMSPITE_H
#define ANIMSPRITE_H

#include <string>

#include <SFML/Graphics/Sprite.hpp>

class AnimData;

/**
 * sf::Sprite with methods to enable easier animation based on AnimData
 * loaded in.
 */
class AnimSprite : public sf::Sprite {
 public:
  /**
   * Animation and frame are initialized to invalid values. User must set
   * these (note that SetAnimation will set the correct starting frame), 
   * otherwise sprite will display nothing. 
   */
  AnimSprite();

  /**
   * Goes to next frame in sprite's animation sequence if it is time to do so.
   * @param dt delta time value - amount of time to step forward
   */
  void Update( float dt );

  /**
   * @return AnimData for this sprite's animations.
   */
  const AnimData* GetAnimData() const;

  /**
   * @return Number of current frame sprite is on.
   */
  int GetFrame() const;

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
  void SetAnimData( const AnimData *animData );
  
 protected:
  /**
   * @return whether sprite's animation is playing.
   */
  bool IsAnimating();

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

  const AnimData *m_animData; /** Animation data stored for this sprite. */
  bool m_playing; /** Whether animation is playing. */
  bool m_reversed; /** Whether animation is to be played in reverse. */
  bool m_setToReverse; /** Enables newly set animation to start in reverse. */
  float m_frameTimeLeft; /** Time left for current frame to play. */
  int m_animNum; /** Number of animation sprite is currently on. */
  int m_frameNum; /** Number of frame sprite is currently on. */ 
};

#endif // ANIMSPRITE_H
