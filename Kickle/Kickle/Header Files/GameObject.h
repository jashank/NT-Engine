#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>

#include "Lunar.h"

#include "BasicTypeDefs.h"


class AnimData;


/************************************************
GameObject
-Inherits from sf::Sprite
-Animates a spritesheet when given AnimData
************************************************/
class GameObject : public sf::Sprite {
 public:
  enum Dir {
    Up,
    Down,
    Left,
    Right
  };

  GameObject( lua_State *L );
	GameObject( const std::string &xmlGameObjectPath );
  GameObject( const std::string &xmlGameObjectPath, Uint tileX, Uint tileY );
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
	IsMoving
	-Returns true if the object is currently moving
	************************************************/
  bool IsMoving() const;


	/************************************************
	LoadFromFile
	-Loads a GameObject given a path to an xml file
  -Returns true if loading was successful
	************************************************/
  bool LoadFromFile( const std::string& filepath );


	/************************************************
	MoveDir
	-Moves 1 tile length in the supplied direction
	************************************************/	
  void MoveDir( Dir direction );

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
	StopMoving
	-Stops moving
	************************************************/
  void StopMoving();


	/************************************************
	Update
	-Updates the GameObject
	************************************************/
	void Update();


	/************************************************
	LuaMoveDir
	-Wraps MoveDir to allow it to be exposed to Lua
	************************************************/
  int LuaMoveDir( lua_State *L );

	/************************************************
	LuaMoveDir
	-Wraps MoveDir to allow it to be exposed to Lua
	************************************************/
  int LuaSetAnimation( lua_State *L );

  //Necessities for Lunar
  static const char className[];
  static Lunar<GameObject>::RegType methods[];
 private:
	/************************************************
	AnimUpdate
	-Updates the animation
	************************************************/
  void AnimUpdate();

	/************************************************
	InitLua
	-Initializes the lua script
  - //TODO - Call OnCreation here
	************************************************/  
  void InitLua();


	/************************************************
	NextFrame
	-Selects next frame
	************************************************/
	void NextFrame();


	const AnimData *m_animData; //Pointer to constant animation data

  bool m_moving; //If true; keep moving in m_direction
	bool m_play; //If true; animate GameObject
  Dir m_direction; //current direction game object is moving
	float m_frameTime; //time left on current frame
  lua_State* m_luaState; //Mediator between C/C++ and Lua VM
  std::string m_luaScript; //filepath to the lua script
	Uint m_animation; //current animation selections
	Uint m_frame; //current frame selection
};

#endif