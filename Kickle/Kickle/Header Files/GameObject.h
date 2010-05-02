#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <utility>
#include "Lunar.h"

#include "AnimSprite.h"
#include "Key.h"


class LevelState;


/************************************************
GameObject
-Inherits from sf::Sprite
-Animates a spritesheet when given AnimData
************************************************/
class GameObject : public AnimSprite {
 public:
  enum Dir {
    Up,
    Down,
    Left,
    Right
  };

  GameObject( lua_State *L );
	GameObject( const std::string &filepath );
  GameObject( const std::string &filepath, unsigned int tileX, unsigned int tileY );
  ~GameObject();

	// Updates the GameObject's collision
	void UpdateCollision();

	// Updates the GameObject's movement
	void UpdateMovement();

  // Updates the GameObject's rendering
  void UpdateRendering();

  // Returns collision box for game object
  const sf::FloatRect& GetCollisionBox() const;

  // Returns whether game object's collision is grid-based
  bool HasGridCollision() const;

  // Returns x-location of GameObject on game grid
  unsigned int GetTileX() const;

  // Returns y-location of GameObject on game grid
  unsigned int GetTileY() const;

  // Returns type of GameObject
  const std::string& GetType() const;

  /************************************************
  Lua Functions
  ************************************************/
	// Allows Lua to move the GameObject
  int LuaMove( lua_State *L );

	// Wraps SetAnimation to allow it to be exposed to Lua
  int LuaSetAnimation( lua_State *L );

	// Returns whether GameObject is animating
  int LuaIsAnimating( lua_State *L );

	// Allows Lua to access type of GameObject
  int LuaGetType( lua_State *L );

  // Wrap GetTileX and GetTileY to allow it to be exposed to Lua
  int LuaGetTile( lua_State *L );

  // Allows user to get and set the GameObject's direction from Lua
  int LuaGetDir( lua_State *L );
  int LuaSetDir( lua_State *L );

  // Allows Lua to reverse direction of GameObject
  int LuaReverse( lua_State *L );

  // Allows user to get the GameObject's table (to change variables in it, etc.)
  int LuaGetTable( lua_State *L );

  // Sets the GameObject's `noClip` to true or false
  int LuaSetNoClip( lua_State *L );

  //Necessities for Lunar
  static const char className[];
  static Lunar<GameObject>::RegType methods[];

 private:
  typedef std::pair< Key, std::string > KeyEntry;
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

  static LevelState *m_level; // Level that GameObject is on

  bool m_moving; // If true; keep moving in m_direction
  bool m_gridCollision; // Grid-based collision completely restricts access to tile
  bool m_noClip; // When true, allows object to pass through solid objects and tiles
  Dir m_direction; // Current direction game object is moving
  float m_distance; // Distance traveled from last grid location
  float m_speed; // m_speed at which object moves ( 1.0 is "normal" i.e. Kickle )
  int m_id; // ID of object
  sf::FloatRect m_collisionRect; // Object's collision box

  //Array of Key and string(lua script's function names) pairs
  KeyEntry* m_keyRegistry; 
  unsigned int m_numKeyEntries; //Number of KeyEntries in m_keyRegistry

  std::string m_luaScript; // Filepath to the lua script
  std::string m_type; // What type of game object (slime, kickle, etc.)
};

#endif