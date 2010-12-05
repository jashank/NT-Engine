#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "Object.h"
#include "RangeMatrix3D.h"

class Camera;
class lua_State;
class TiXmlElement;

/**
 * Holds and manages all Objects in the current state, updating them,
 * handling their interactions, and providing object management 
 * functions to the State API.
 */
class ObjectManager {
 public:
  /**
   * Loads Objects in from <object> element of State file and sets
   * lua_State to use for script interaction.
   */
  ObjectManager( const TiXmlElement *root, lua_State *L ); 

  /**
   * Initializes Objects added to ObjectManager during construction.
   * Should only be called once immediately after construction.
   */
  void Init();

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
   * @param alpha blending factor between previous frame and current frame.
   * Should be from [0:1].
   * @param cam Camera currently viewing the State.
   */
  void Render( float alpha, const Camera &cam );

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

  int LuaGetObject( lua_State *L ) const;

  int LuaGetObjects( lua_State *L ) const;

  int LuaGetNearestObject( lua_State *L ) const ;

  int LuaGetNearestToObject( lua_State *L ) const ;

  int LuaGetObjectOnTile( lua_State *L ) const ;

  int LuaGetObjectsOnTile( lua_State *L ) const ;

  int LuaObjectBlockingTile( lua_State *L ) const ;
  //@}

 private:
  /**
   * Compares Objects by order of creation.
   * Earlier creation takes precedence.
   */
  struct CreationCmp;

  /**
   * Compares Objects by their render priority.
   * If equal then compares by creation order. An Object with a higher
   * render priority will be rendered last so it will be blitted on top.
   */
  struct RenderPriorityCmp;

  typedef boost::intrusive_ptr<Object> intrObj_type;
  typedef std::multimap<std::string, intrObj_type> nameMap_type;

  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  ObjectManager( const ObjectManager &manager );
  ObjectManager& operator=( const ObjectManager &manager );
  //@}

  /**
   * Loads Objects in from <objects> element of State file.
   */
  void LoadData( const TiXmlElement *root, lua_State *L );

  /**
   * @param obj Object to add to manager.
   */
  void AddObject( const intrObj_type &obj );

  /**
   * @param obj Object to remove from manager.
   */
  void RemoveObject( const intrObj_type &obj );

  /**
   * @param objType type of Object to search for.
   * @return Object with 'objType' as its type. If no Object is found, returns
   * NULL. If more than one is found, returns first found.
   */ 
  Object *FindObject( const std::string &objType ) const;

  /**
   * @param x x tile coordinate.
   * @param y y tile coordinate.
   * @return First Object on specified tile location. If no objects,
   * returns NULL. 
   */
  Object *ObjectOnTile( int x, int y ) const;

  /**
   * Returns object on (x,y) tile coordinate with type passed.
   */
  Object *ObjectOnTile( int x, int y, std::string &type ) const;

  /**
   * Returns object of type nearest to the tile (x,y) (inclusive). 
   * Optional fourth argument to specify NOT to return that
   * Object if it is found, and instead to continue searching.
   */
  Object *NearestObject(
    std::string &type,
    int x,
    int y,
    Object *exclude = NULL
  ) const;

  /**
   * Checks to see if Object passed is colliding with any Objects
   * and updates it if so. Note that this function does call
   * SetRange on the object grid so you may need to save your place
   * beforehand.
   * @param obj Object to check.
   * @param cam Camera viewing the State.
   */
  void UpdateCollisions( const intrObj_type &obj, const Camera &cam );

  /**
   * Checks objects in range passed to ensure that their tile coordinate
   * matches their spot in the matrix. If not, they are moved.
   * @param tLx top left x tile coordinate
   * @param tLy top left y tile coordinate
   * @param bRx bottom right x tile coordinate
   * @param bRy bottom right y tile coordinate
   */
  void AdjustGridCoords( int tLx, int tLy, int bRx, int bRy );

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
  ) const;

  /**
   * Fills set passed with all objects in range set on object grid.
   * This means that you should call SetRange before calling this method.
   */
  template< typename Compare >
  void FillSet( std::set<intrObj_type, Compare> &set ) const;

  /**
   * Key is Object's type. Holds all Objects in the current State of that type.
   */
  nameMap_type m_objTypes;

  /**
   * 3D Matrix that holds Objects at each position.
   */
  boost::scoped_ptr<nt::core::RangeMatrix3D<intrObj_type> > m_objGrid;

  /**
   * Holds Objects that were sent to be destroyed on the last update.
   */
  std::vector<intrObj_type> m_toBeDestroyed;
};


template< typename Compare >
void ObjectManager::FillSet( std::set<intrObj_type, Compare> &set ) const {
  while ( const intrObj_type *obj = m_objGrid->GetElem() ) {
    set.insert( *obj );
  }
}

#endif // OBJECT_MANAGER_H

