#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include "Lunar.h"

#include "AnimSprite.h"
#include "BasicTypeDefs.h"


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
    Right,
  };

  GameObject( lua_State *L );
	GameObject( const std::string &filepath );
  GameObject( const std::string &filepath, Uint tileX, Uint tileY );
  ~GameObject();

	/************************************************
	MoveDir
	-Moves 1 tile length in the supplied direction
	************************************************/	
  void MoveDir( Dir direction );

	/************************************************
	StopMoving
	-Stops moving
	************************************************/
  void StopMoving();

	/************************************************
	Update
	-Updates the GameObject
	************************************************/
	virtual void Update();

  /**********************************************
  SetId()
  - Sets the member unique identifier.
  ***********************************************/
  void SetId( int id );
  
  /*************************************************
  GetId()
  - Returns the member unique identifier (m_id)
  **************************************************/
  int GetId() const;

  /*************************************************
  GetCollisionBox()
  - Returns collision box for game object
  **************************************************/
  const sf::FloatRect &GetCollisionBox() const;

  /*************************************************
  IsSolid()
  - Returns whether game object is solid
  **************************************************/
  bool IsSolid() const;

  /*************************************************
  GetTileX()
  - Returns x-location of GameObject on game grid
  **************************************************/
  Uint GetTileX() const;

  /*************************************************
  GetTileY()
  - Returns y-location of GameObject on game grid
  **************************************************/
  Uint GetTileY() const;

  /*************************************************
  GetType()
  - Returns type of GameObject
  **************************************************/
  const std::string& GetType() const;
  
	/************************************************
	LuaMoveDir
	-Wraps MoveDir to allow it to be exposed to Lua
	************************************************/
  int LuaMoveDir( lua_State *L );

	/************************************************
	LuaSetAnimation
	-Wraps SetAnimation to allow it to be exposed to Lua
	************************************************/
  int LuaSetAnimation( lua_State *L );

  /************************************************
	LuaGetType
	-Allows Lua to access type of GameObject
	************************************************/
  int LuaGetType( lua_State *L );

  /************************************************
	LuaGetTileX
	-Wraps GetTileX to allow it to be exposed to Lua
	************************************************/
  int LuaGetTileX( lua_State *L );

  /************************************************
	LuaGetTileY
	-Wraps GetTileY to allow it to be exposed to Lua
	************************************************/
  int LuaGetTileY( lua_State *L );

  //Necessities for Lunar
  static const char className[];
  static Lunar<GameObject>::RegType methods[];

 private:
  /************************************************
	LoadObjectData
	-Loads a GameObject given a path to an xml file
  -Returns true if loading was successful
	************************************************/
  bool LoadObjectData( const std::string &filepath );

  /************************************************
	LoadCollisionData
	-Loads GameObject's collision data given path to
   xml file
  -Returns true if loading was successful
	************************************************/
  bool LoadCollisionData( const std::string &filepath );

	/************************************************
	InitLua
	-Initializes the lua script
  - //TODO - Call OnCreation here
	************************************************/  
  void InitLua();

	/************************************************
	MovementUpdate
	-Updates the movement of GameObject
	************************************************/
  void MovementUpdate();


  static LevelState *m_level; // Level that GameObject is on

  bool m_moving; // If true; keep moving in m_direction
  bool m_solid; // Solid objects completely restrict access to tile they are on
  Dir m_direction; // Current direction game object is moving
  float m_distance; // Distance traveled from last grid location
  int m_id; // ID of object
  lua_State* m_luaState; // Mediator between C/C++ and Lua VM
  sf::FloatRect m_collisionRect; // Object's collision box
  std::string m_luaScript; // Filepath to the lua script
  std::string m_type; // What type of game object (slime, kickle, etc.)
};

#endif