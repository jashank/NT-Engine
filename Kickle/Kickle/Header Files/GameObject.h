#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>

#include "BasicTypeDefs.h"


class AnimData;


/************************************************
GameObject
-Inherits from sf::Sprite
-Animates a spritesheet when given AnimData
************************************************/
class GameObject : public sf::Sprite {
 public:
	GameObject( const std::string &filename );
  ~GameObject();

	/************************************************
	GetAnimData
	-Returns the data used for the animation
	************************************************/
	const AnimData *GetAnimData() const;
	
	/************************************************
	GetFrame
	-Returns the current animation frame
	************************************************/
	Uint GetFrame() const;

	/************************************************
	GetAnimation
	-Returns the current animation
	************************************************/
	Uint GetAnimation() const;

	/************************************************
	Pause
	-Freezes animation on current frame
	************************************************/	
	void Pause();

	/************************************************
	Play
	-Resumes animation from current frame
	************************************************/
	void Play();

	/************************************************
	Restart
	-Stops animation then resumes from first frame
	************************************************/
	void Restart();

	/************************************************
	SetFrame
	-Sets the current animation frame
	************************************************/
	void SetFrame( Uint frame );

	/************************************************
	SetAnimation
	-Sets the current animation
	************************************************/
	void SetAnimation( Uint animation );

	/************************************************
	SetAnimData
	-Sets the data to be used for the animation
	************************************************/
	void SetAnimData( const AnimData &animData );

	/************************************************
	Start
	-Resumes animation from current frame
	************************************************/
	void Start();

	/************************************************
	Stop
	-Stops animation
  -Sets animation to first frame
	************************************************/
	void Stop();

	/************************************************
	Update
	-Updates the GameObject
	************************************************/
	void Update();

 private:
	/************************************************
	AnimUpdate
	-Updates the animation
	************************************************/
  void AnimUpdate();

	/************************************************
	NextFrame
	-Selects next frame
	************************************************/
	void NextFrame();

	const AnimData *m_animData; //Pointer to constant animation data

	bool m_play; //If true; animate GameObject
	float m_frameTime; //time left on current frame
	Uint m_animation; //current animation selections
	Uint m_frame; //current frame selection
};

#endif