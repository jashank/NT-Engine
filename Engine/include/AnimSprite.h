#ifndef ANIMSPITE_H
#define ANIMSPRITE_H

#include <string>

#include <SFML/Graphics.hpp>

class AnimData;

class AnimSprite : public sf::Sprite {
 public:
  AnimSprite();

  /// Returns the data used for the animation
  const AnimData* GetAnimData() const;

  /// Returns the current animation frame
  int GetFrame() const;

  /// Returns the current animation
  int GetAnimation() const;

  /// Loads animation data from xml file passed and calls SetAnimData.
  void LoadAnimData( const std::string &filepath );

  /// Freezes animation on current frame
  void Pause();

  /// Resumes animation from current frame
  void Play();

  /// Stops animation and resets time
  void Restart();

  /// Sets the current animation's frame
  void SetFrame( int frame );

  /// Sets the current animation. Set reverse to true for animation to
  /// start on the last clip. Does not play the animation.
  void SetAnimation( int animation, bool reverse = false );

  /// Sets the data to be used for the animation
  void SetAnimData( const AnimData *animData );

  /// Stops animation, setting the frame to the first clip if the animation
  /// is playing forwards, and the last if the animation is playing backwards 
  void Stop();

  /// Updates the AnimSprite
  void Update();

 protected:
  /// Returns whether sprite is still animating
  bool IsAnimating();

 private:
  /// Restricts copy constructor and assignment.
  AnimSprite( const AnimSprite &sprite );
  AnimSprite& operator=( const AnimSprite &sprite );

  /// Updates the animation
  void AnimUpdate();

  /// Selects the next frame
  void NextFrame();

  const AnimData *m_animData; //Pointer to constant animation data
  bool m_play; //If true, animate AnimSprite
  bool m_reverse; // If true, play animation in reverse
  float m_frameTime; //Time left on current frame
  int m_animation; //Current animation selections
  int m_frame; //Current frame selection
};

#endif
