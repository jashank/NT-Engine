#ifndef ANIMSPITE_H
#define ANIMSPRITE_H
#include <SFML/Graphics.hpp>
#include <string>

#include "tinyxml.h"

#include "AnimData.h"
#include "BasicTypeDefs.h"

class AnimData;

class AnimSprite : public sf::Sprite {
 public:
  AnimSprite();

  //Change the order of animation
  void AnimateBackward();
  void AnimateForward();

  // Returns the data used for the animation
	const AnimData *GetAnimData() const;

  // Returns the current animation frame
	Uint GetFrame() const;

  // Returns the current animation
	Uint GetAnimation() const;

  // Loads animation data from xml file passed
  void LoadAnimData( const std::string &filepath );

  // Freezes animation on current frame
	void Pause();

  // Resumes animation from current frame
	void Play();

  // Stops animation then resumes from first frame
	void Restart();

  // Sets the current animation's frame
	void SetFrame( Uint frame );

  // Sets the current animation
	void SetAnimation( Uint animation );

  // Sets the data to be used for the animation
	void SetAnimData( const AnimData &animData );

  // Starts the animation from the first frame
	void Start();

  // Stops animation and sets it to first frame
	void Stop();

  // Updates the AnimSprite
  void Update();

 protected:
  // Returns whether sprite is still animating 
  bool IsAnimating();

 private:
  // Updates the animation
  void AnimUpdate();

  // Selects the next frame
	void NextFrame();


	const AnimData *m_animData; //Pointer to constant animation data
	bool m_play; //If true; animate AnimSprite
  bool m_reverse; //If true; animate in reverse order
	float m_frameTime; //Time left on current frame
	Uint m_animation; //Current animation selections
	Uint m_frame; //Current frame selection
};

#endif