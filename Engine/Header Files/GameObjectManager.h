#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include <list>
#include <string>
#include <vector>

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
}

class TiXmlElement;
class GameObject;

class GameObjectManager {
 public:
  static const int NULL_GAME_OBJECT = -1;

  GameObjectManager() {}
  ~GameObjectManager();

  /// Parses data from <game_objects> section of state file
  bool LoadData( const TiXmlElement *dataRoot );

  /// Handle the events of the GameObjects
  void HandleEvents();
  /// Update and render the GameObjects in the manager.
  void Update();
  void Render() const;

  /// Returns whether an object is blocking the tile passed
  bool ObjectBlockingTile( int x, int y ) const;

  /****************************************
   * Lua Functions
   ***************************************/
  /// Registers lua functions to state passed
  static void RegisterLuaFuncs( lua_State *L );

  /// Creates an GameObject given its XML file. Reports error if file is invalid.
  static int LuaCreateGameObject( lua_State *L );

  /// Destroys GameObject passed. Does nothing if GameObject doesn't exist.
  static int LuaDestroyGameObject( lua_State *L );

  /// Returns a GameObject given the GameObject's "type". NULL if there is none.
  static int LuaGetGameObject( lua_State *L );

  /// Returns all GameObjects of type specified. Empty table if there are none.
  static int LuaGetGameObjects( lua_State *L );

  /// Returns closest GameObject of type passed given tile coordinates. If no
  /// type passed, just returns the closest GameObject
  static int LuaGetNearestGameObject( lua_State *L );

  /// Returns GameObject located on tile. NULL if none exists.
  static int LuaGetGameObjectOnTile( lua_State *L );

  /// Returns whether tile specified has an object on it
  /// that is blocking access to it.
  static int LuaObjectBlockingTile( lua_State *L );

 private:
  typedef std::list<GameObject*>::iterator GameObjItr;
  typedef std::list<GameObject*>::const_iterator GameObjItrConst;

  static const luaL_Reg LuaFuncs[];

  /// Restricts copy constructor and assignment.
  GameObjectManager( const GameObjectManager &manager );
  GameObjectManager& operator=( const GameObjectManager &manager );

  /// Add 'gameObject' passed.
  void AddGameObject( GameObject *gameObject );
  /// Remove 'gameObject' passed.
  void RemoveGameObject( GameObject *gameObject );

  /// Returns a GameObject in the GameObjectManager with the given 'objectType'
  /// as its type. Returns NULL if an object isn't found.
  GameObject* GetGameObject( const std::string &objectType ) const;

  /// If an object is on the specified tile location, that object is returned.
  /// Returns NULL otherwise.
  GameObject* ObjectOnTile( int x, int y ) const;

  /// Collision detection for non-solid GameObjects using rectangular
  /// collision detection. Returns GameObject that 'gameObject' collided with,
  /// or NULL otherwise.
  GameObject* DetectCollision( const GameObject *gameObject );

  /// Returns current instance of GameObjectManager in application
  static GameObjectManager& Inst();

  /// List of game objects that manager is holding
  std::list<GameObject*> m_gameObjects;
  /// Holds GameObjects that were sent to be destroyed on the last update
  std::vector<GameObject*> m_toBeDestroyed;
};

#endif
