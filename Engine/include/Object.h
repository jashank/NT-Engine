#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include "boost/function/function1.hpp"

#include "AnimSprite.h"
#include "InputHandler.h"
#include "Key.h"
#include "Lunar.h"

class State;

/*! \class Object
 * -Inherits from sf::Sprite\n
 * -Animates a spritesheet when given AnimData\n
*/
class Object : public AnimSprite {
 public:
  enum Dir { Up, Down, Left, Right };

  /// First constructor for registering to Lunar, second for loading in Objects
  Object( lua_State *L );
  Object( const std::string &filepath, int tileX, int tileY, int strip );
  ~Object();

  /// Handle events generated for Object
  void HandleEvents();

  /// Updates the Object's collision
  void UpdateCollision( Object *collisionObj );

  /// Updates the Object's movement
  void UpdateAI();

  /// Updates the Object's rendering
  void UpdateRendering();

  /// Returns collision box for  object
  const sf::FloatRect& GetCollisionBox() const;

  /// Returns whether Object blocks tile it is on,
  /// preventing other Objects from getting on it
  bool BlockingTile() const;

  /// Returns x-location of Object on  grid
  int GetTileX() const;

  /// Returns y-location of Object on  grid
  int GetTileY() const;

  /// Returns type of Object
  const std::string& GetType() const;

  /************************************************
  Lua Functions
  ************************************************/
	/// Allows user to tell Object to move from Lua.
	/// Returns true if Object can move in the direction it is facing,
	/// or is in motion. False if it can't move in the direction it is facing.
  int LuaMove( lua_State *L );

  /// Returns current animation ID
  int LuaGetAnimation( lua_State *L );

	/// Sets animation on sheet corresponding to index top to bottom
  int LuaSetAnimation( lua_State *L );

  /// Same as above, but animation is played in reverse
  int LuaSetAnimationReverse( lua_State *L );

	/// Returns whether Object is animating
  int LuaIsAnimating( lua_State *L );

  /// Returns whether Object is moving
  int LuaMoving( lua_State *L );

	/// Allows Lua to access type of Object
  int LuaGetType( lua_State *L );

  /// Wrap GetTileX and GetTileY to allow it to be exposed to Lua
  int LuaGetTile( lua_State *L );

  /// Takes boolean. True means Object should block tile it is on,
  /// false means it shouldn't.
  int LuaBlockTile( lua_State *L );

  /// Allows user to get and set the Object's direction from Lua
  int LuaGetDir( lua_State *L );
  int LuaSetDir( lua_State *L );

  /// Allows Lua to reverse direction of Object
  int LuaReverse( lua_State *L );

  /// Allows user to get the Object's table (to change variables in it, etc.)
  int LuaGetTable( lua_State *L );

  /// Sets the Object's `noClip` to true or false
  int LuaSetNoClip( lua_State *L );

  /// Sets the starting time to the current system time
  int LuaResetTimer( lua_State *L );

  /// Returns the elapsed time since timer was last reset in seconds
  /// Has floating point precision
  int LuaGetElapsedTime( lua_State *L );

  /// Returns speed of Object
  int LuaGetSpeed( lua_State *L );

  /// Sets speed to speed passed (float)
  int LuaSetSpeed( lua_State *L );

  /// Reduces speed by float given. Speed can't go below zero.
  int LuaSlowDown( lua_State *L );

  /// Increases speed by float given. Speed set to zero if greater than numeric
  /// limit.
  int LuaSpeedUp( lua_State *L );


  /// Necessities for Lunar
  static const char className[];
  static Lunar<Object>::RegType methods[];

 private:
  typedef std::pair< Key, std::string > KeyEntry;

  /// Restricts copy constructor and assignment.
  Object( const Object &object );
  Object& operator=( const Object &object );

	/// Loads Object given a path to an xml file,
  /// returning true if loading was successful
  bool LoadObjectData( const std::string &filepath );

	/// Loads Object's collision data given path to xml file,
  /// returning true if loading was successful
  bool LoadCollisionData( const std::string &filepath );

	/// Initializes the lua script
  void InitLua();

	///Updates the movement of Object
  void MovementUpdate();

  /// Corrects movement of object when it exceeds next grid location
  void CorrectMovement();

  /// Calls lua function named 'funcName' that is in this Object's script
  void CallScriptFunc( std::string &funcName );

  /// Function pointer registered to CallLuaFunc for use with InputHandler
  const boost::function1<void, std::string&> m_ptrCallScriptFunc;

  bool m_moving; // If true; keep moving in m_direction
  bool m_blockingTile; // True if object blocks other objects from accessing tile it is on
  bool m_noClip; // When true, allows object to pass through solid objects and tiles
  Dir m_direction; // Current direction  object is moving
  float m_distance; // Distance traveled from last grid location
  float m_speed; // m_speed at which object moves
  InputHandler m_input; // Handles input for this Object
  int m_id; // ID of object
  sf::Clock m_timer; // Timer that Object can use from its script
  sf::FloatRect m_collisionRect; // Object's collision box
  static State *m_state; // State that Object is in
  std::string m_luaScript; // Filepath to the lua script
  std::string m_type; // What type of  object (slime, kickle, etc.)
};

#endif
