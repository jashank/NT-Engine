#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>

#include "boost/function/function1.hpp"

#include "AnimSprite.h"
#include "InputHandler.h"
#include "Key.h"
#include "Lunar.h"

class GameState;

/************************************************
GameObject
-Inherits from sf::Sprite
-Animates a spritesheet when given AnimData
************************************************/
class GameObject : public AnimSprite {
 public:
  enum Dir { Up, Down, Left, Right };

  // First constructor for registering to Lunar, second for loading in GameObjects
  GameObject( lua_State *L );
  GameObject( const std::string &filepath, int tileX, int tileY );
  ~GameObject();

  // Handle events generated for GameObject
  void HandleEvents();

  // Updates the GameObject's collision
  void UpdateCollision( GameObject *collisionObj );

  // Updates the GameObject's movement
  void UpdateAI();

  // Updates the GameObject's rendering
  void UpdateRendering();

  // Returns collision box for game object
  const sf::FloatRect& GetCollisionBox() const;

  // Returns whether game object blocks tile it is on,
  // preventing other GameObjects from getting on it
  bool BlockingTile() const;

  // Returns x-location of GameObject on game grid
  int GetTileX() const;

  // Returns y-location of GameObject on game grid
  int GetTileY() const;

  // Returns type of GameObject
  const std::string& GetType() const;

  /************************************************
  Lua Functions
  ************************************************/
	// Allows user to tell GameObject to move from Lua.
	// Returns true if GameObject can move in the direction it is facing,
	// or is in motion. False if it can't move in the direction it is facing.
  int LuaMove( lua_State *L );

	// Sets animation on sheet corresponding to index top to bottom
  int LuaSetAnimation( lua_State *L );

  // Same as above, but animation is played in reverse
  int LuaSetAnimationReverse( lua_State *L );

	// Returns whether GameObject is animating
  int LuaIsAnimating( lua_State *L );

  // Returns whether GameObject is moving
  int LuaMoving( lua_State *L );

	// Allows Lua to access type of GameObject
  int LuaGetType( lua_State *L );

  // Wrap GetTileX and GetTileY to allow it to be exposed to Lua
  int LuaGetTile( lua_State *L );

  // Takes boolean. True means GameObject should block tile it is on,
  // false means it shouldn't.
  int LuaBlockTile( lua_State *L );

  // Allows user to get and set the GameObject's direction from Lua
  int LuaGetDir( lua_State *L );
  int LuaSetDir( lua_State *L );

  // Allows Lua to reverse direction of GameObject
  int LuaReverse( lua_State *L );

  // Allows user to get the GameObject's table (to change variables in it, etc.)
  int LuaGetTable( lua_State *L );

  // Sets the GameObject's `noClip` to true or false
  int LuaSetNoClip( lua_State *L );

  // Sets the starting time to the current system time
  int LuaResetTimer( lua_State *L );

  // Returns the elapsed time since timer was last reset in seconds
  // Has floating point precision
  int LuaGetElapsedTime( lua_State *L );

  //Necessities for Lunar
  static const char className[];
  static Lunar<GameObject>::RegType methods[];

 private:
  typedef std::pair< Key, std::string > KeyEntry;

  // Restricts copy constructor and assignment.
  GameObject( const GameObject &object );
  GameObject& operator=( const GameObject &object );

	// Loads a GameObject given a path to an xml file,
  // returning true if loading was successful
  bool LoadObjectData( const std::string &filepath );

	// Loads GameObject's collision data given path to xml file,
  // returning true if loading was successful
  bool LoadCollisionData( const std::string &filepath );

	// Initializes the lua script
  void InitLua();

	//Updates the movement of GameObject
  void MovementUpdate();

  // Corrects movement of object when it exceeds next grid location
  void CorrectMovement();

  // Calls lua function named 'funcName' that is in this GameObject's script
  void CallScriptFunc( std::string &funcName );

  // Function pointer registered to CallLuaFunc for use with InputHandler
  const boost::function1<void, std::string&> m_ptrCallScriptFunc;

  bool m_moving; // If true; keep moving in m_direction
  bool m_blockingTile; // True if object blocks other objects from accessing tile it is on
  bool m_noClip; // When true, allows object to pass through solid objects and tiles
  Dir m_direction; // Current direction game object is moving
  float m_distance; // Distance traveled from last grid location
  float m_speed; // m_speed at which object moves
  InputHandler m_input; // Handles input for this GameObject
  int m_id; // ID of object
  sf::Clock m_timer; // Timer that GameObject can use from its script
  sf::FloatRect m_collisionRect; // Object's collision box
  static GameState *m_gameState; // State that GameObject is in
  std::string m_luaScript; // Filepath to the lua script
  std::string m_type; // What type of game object (slime, kickle, etc.)
};

#endif
