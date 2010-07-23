#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <list>
#include <string>
#include <vector>

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
}

class TiXmlElement;
class Object;

class ObjectManager {
 public:
  static const int NULL_OBJECT = -1;

  ObjectManager() {}
  ~ObjectManager();

  /// Parses data from <objects> section of state file
  bool LoadData( const TiXmlElement *dataRoot );

  /// Handle the events of the Objects
  void HandleEvents();
  /// Update and render the Objects in the manager.
  void Update();
  void Render() const;

  /// Returns whether an object is blocking the tile passed
  bool ObjectBlockingTile( int x, int y ) const;

  /****************************************
   * Lua Functions
   ***************************************/
  /// Registers lua functions to state passed
  static void RegisterLuaFuncs( lua_State *L );

  /// Creates Object given its XML file. Reports error if file is invalid.
  static int LuaCreateObject( lua_State *L );

  /// Destroys Object passed. Does nothing if Object doesn't exist.
  static int LuaDestroyObject( lua_State *L );

  /// Returns a Object given the Object's "type". NULL if there is none.
  static int LuaGetObject( lua_State *L );

  /// Returns all Objects of type specified. Empty table if there are none.
  static int LuaGetObjects( lua_State *L );

  /// Returns closest Object of type passed given tile coordinates. If no
  /// type passed, just returns the closest Object
  static int LuaGetNearestObject( lua_State *L );

  /// Returns Object located on tile. NULL if none exists.
  static int LuaGetObjectOnTile( lua_State *L );

  /// Returns whether tile specified has an object on it
  /// that is blocking access to it.
  static int LuaObjectBlockingTile( lua_State *L );

 private:
  typedef std::list<Object*>::iterator ObjItr;
  typedef std::list<Object*>::const_iterator ObjItrConst;

  static const luaL_Reg LuaFuncs[];

  /// Restricts copy constructor and assignment.
  ObjectManager( const ObjectManager &manager );
  ObjectManager& operator=( const ObjectManager &manager );

  /// Calls Init 
  void InitObjs();

  /// Add 'Object' passed.
  void AddObject( Object *object );
  /// Remove 'Object' passed.
  void RemoveObject( Object *object );

  /// Returns Object in the ObjectManager with the given 'objectType'
  /// as its type. Returns NULL if an object isn't found.
  Object* GetObject( const std::string &objectType ) const;

  /// If an object is on the specified tile location, that object is returned.
  /// Returns NULL otherwise.
  Object* ObjectOnTile( int x, int y ) const;

  /// Collision detection for non-solid Objects using rectangular
  /// collision detection. Returns Object that 'object' collided with,
  /// or NULL otherwise.
  Object* DetectCollision( const Object *object );

  /// Returns current instance of ObjectManager in application
  static ObjectManager& Inst();

  /// List of Objects that manager is holding
  std::list<Object*> m_objects;
  /// Holds Objects that were sent to be destroyed on the last update
  std::vector<Object*> m_toBeDestroyed;
};

#endif // OBJECT_MANAGER_H
