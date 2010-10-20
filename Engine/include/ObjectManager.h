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

class Camera;
class Object;
class TiXmlElement;

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
   * @param L lua state for Objects to use
   * @return True if load was successful (no syntax errors).
   */
  bool LoadData( const TiXmlElement *dataRoot, lua_State *L );

  /**
   * Calls HandleEvents on every Object on screen.
   * @param cam Camera currently viewing the State.
   */
  void HandleEvents( const Camera &cam );

  /**
   * Calls Update functions on every Object on screen. 
   * @param dt delta time - amount of time to step forward
   * @param cam Camera currently viewing the State.
   */
  void Update( float dt, const Camera &cam );
  
  /**
   * Renders every Object on screen.
   * @param cam Camera currently viewing the State.
   */
  void Render( const Camera &cam ) const;

  /**
   * @return Whether there is an Object on tile coordinate passed that is
   * blocking the tile.
   */
  bool ObjectBlockingTile( int x, int y ) const;

  //@{
  /**
   * Lua-related functions. See State API documentation for details. Note that
   * these all return the number of arguments that the caller should
   * return to Lua.
   */
  int LuaCreateObject( lua_State *L );

  int LuaDestroyObject( lua_State *L );

  int LuaGetObject( lua_State *L );

  int LuaGetObjects( lua_State *L );

  int LuaGetNearestObject( lua_State *L );

  int LuaGetObjectOnTile( lua_State *L );

  int LuaObjectBlockingTile( lua_State *L );
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
   * Calls Camera's GetAdjustedFocus given adjustment values, and inserts
   * them into other arguments passed.
   * @param cam Camera to call function on.
   * @param xadj x-wise adjustment for Camera.
   * @param yadj y-wise adjustment for Camera.
   * @param tLx top left x tile coordinate.
   * @param tLy top left y tile coordinate.
   * @param bRx bottom right x tile coordinate.
   * @param bRy bottom right y tile coordinate.
   */
  void GetCamCoords( 
    const Camera &cam, 
    int xadj, 
    int yadj, 
    int &tLx, 
    int &tLy, 
    int &bRx, 
    int &bRy
  );

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
