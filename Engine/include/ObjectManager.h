#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <list>
#include <map>
#include <string>
#include <vector>

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
}

#include "Matrix2D.h"

class TiXmlElement;
class Object;

/**
 * Holds and manages all Objects in the current state, updating them,
 * handling their interactions, and providing object management 
 * functions to the State API.
 */
class ObjectManager {
 public:
  ObjectManager(); 
  ~ObjectManager();

  /**
   * Loads Objects in from <objects> element of State XML file.
   * @param dataRoot parent element of objects set for creation upon
   * initialization of state.
   * @return True if load was successful (no syntax errors).
   */
  bool LoadData( const TiXmlElement *dataRoot );

  /**
   * Calls HandleEvents on every Object on screen.
   */
  void HandleEvents();

  /**
   * Calls Update functions on every Object on screen. 
   */
  void Update();
  
  /**
   * Renders every Object on screen.
   */
  void Render() const;

  /**
   * @return Whether there is an Object on tile coordinate passed that is
   * blocking the tile.
   */
  bool ObjectBlockingTile( int x, int y ) const;

  /**
   * Registers Lua functions to the State API for access in scripts.
   * @param L lua state that functions will be registered in.
   */
  static void RegisterLuaFuncs( lua_State *L );

  //@{
  /**
   * Lua functions. See the State API for how these functions manipulate the
   * current state.
   * @param L lua state Object is in. Note that there is only one state in
   * application at any time.
   * @return Number of arguments pushed on the lua stack.
   */
  static int LuaCreateObject( lua_State *L );

  static int LuaDestroyObject( lua_State *L );

  static int LuaGetObject( lua_State *L );

  static int LuaGetObjects( lua_State *L );

  static int LuaGetNearestObject( lua_State *L );

  static int LuaGetObjectOnTile( lua_State *L );

  static int LuaObjectBlockingTile( lua_State *L );
  //@}

 private:
  typedef std::list<Object*> ObjectList;
  typedef std::list<Object*>::iterator ListItr;
  typedef std::list<Object*>::const_iterator ListItrConst;
  typedef std::multimap<std::string, Object*>::iterator MapItr;
  typedef std::multimap<std::string, Object*>::const_iterator MapItrConst;

  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  ObjectManager( const ObjectManager &manager );
  ObjectManager& operator=( const ObjectManager &manager );
  //@}

  /**
   * @param obj Object to add to manager.
   */
  void AddObject( Object *obj );

  /**
   * @param obj Object to remove from manager.
   */
  void RemoveObject( Object *obj );

  /**
   * @param objType type of Object to search for.
   * @return Object with 'objType' as its type. If no Object is found, returns
   * NULL. If more than one is found, returns first found.
   */ 
  Object* FindObject( const std::string &objType ) const;

  /**
   * @param x x tile coordinate.
   * @param y y tile coordinate.
   * @return First Object on specified tile location. If no objects,
   * returns NULL. 
   */
  Object* ObjectOnTile( int x, int y ) const;

  /**
   * Checks to see if Object passed is colliding with any Objects. 
   * @param obj Object to check.
   * @return If a collision is detected, returns Object that 'obj' collided
   * with. Returns NULL otherwise.
   */
  Object* DetectCollision( Object *obj );

  /**
   * Checks to see if Object at objItr is on coordinate passed. If not, 
   * adjusts Object location accordingly in the object grid. Since function
   * can modify list being iterated through and invalidate 'objItr', returns
   * valid iterator to continue iterating with.
   * @param x x tile coordinate to check.
   * @param y y tile coordinate to check.
   * @param objItr iterator to object to check.
   * @return Validated iterator to continue using.
   */
  ListItr AdjustGridCoord( int x, int y, ListItr objItr );

  /**
   * Increments point passed, given a width and a height.
   *
   * First increments x. If x is still less then then the width, then returns.
   * If x has greater than or equal to the width, then x is set to 0 and y is
   * incremented. If both breach width and height, respectively, then both are
   * set to -1.
   */
  void IncPoint( int &x, int &y, int width, int height );

  /**
   * Needed by Lua functions because they are required to be static.
   * @return Current instance of ObjectManager in current State.
   */
  static ObjectManager& Inst();

  /**
   * Method names for State API to associate with methods in ObjectManager
   * class. For example, "GetObject" is matched with LuaGetObject.
   */ 
  static const luaL_Reg LuaFuncs[]; 

  /**
   * Key is Object's type. Holds all Objects in the current State of that type.
   */
  std::multimap<std::string, Object*> m_objTypes; 

  /**
   * 2D Matrix that holds list of Objects at each position.
   */
  nt::core::Matrix2D<ObjectList> *m_objGrid; 

  /**
   * Holds Objects that were sent to be destroyed on the last update.
   */
  std::vector<Object*> m_toBeDestroyed;
};

#endif // OBJECT_MANAGER_H
