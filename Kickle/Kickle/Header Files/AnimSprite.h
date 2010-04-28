#ifndef ANIMSPITE_H
#define ANIMSPRITE_H
#include <SFML/Graphics.hpp>
#include <string>

#include "tinyxml.h"

#include "AnimData.h"


class AnimData;

class AnimSprite : public sf::Sprite {
 public:
  AnimSprite();

  // Returns the data used for the animation
	const AnimData *GetAnimData() const;

  // Returns the current animation frame
	unsigned int GetFrame() const;

  // Returns the current animation
	unsigned int GetAnimation() const;

  // Loads animation data from xml file passed
  void LoadAnimData( const std::string &filepath );

  // Freezes animation on current frame
	void Pause();

  // Resumes animation from current frame
	void Play();

  // Stops animation then resumes from first frame
	void Restart();

  // Sets the current animation's frame
	void SetFrame( unsigned int frame );

  // Sets the current animation
	void SetAnimation( unsigned int animation );

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
	float m_frameTime; //Time left on current frame
	unsigned int m_animation; //Current animation selections
	unsigned int m_frame; //Current frame selection
};

#endif