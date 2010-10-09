#ifndef OBJECT_H
#define OBJECT_H

#include <algorithm>
#include <list>
#include <string>

#include <boost/function/function1.hpp>
#include <SFML/System/Clock.hpp>

#include "AnimSprite.h"
#include "InputHandler.h"
#include "Key.h"
#include "Lunar.h"
#include "Rect.h"
#include "TimedString.h"
#include "Vector.h"

/**
 * Objects are heavily customizable via their associated lua script and xml 
 * file. They make up everything that has behavior in the app. They are 
 * restricted to tile-based placement and movement. The class provides an API 
 * to assist in writing behavior in an object's script. Almost everything is
 * private because the only class that uses Objects is the ObjectManager, and
 * it accesses this through the ObjectAttorney interface.
 *
 * See the Object Guide for more information.
 */
class Object : public AnimSprite {
 public:
  /**
   * Lunar requires Object constructor that just takes pointer to lua_State. 
   */
  Object( lua_State *L );

  ~Object();

  static const char className[]; /** Stores name of class. Needed by Lunar. */

  /**
   * Stores method names for the Object API to associate with methods in the
   * Object class. For example, "Move" is matched with &Object::LuaMove.
   */
  static Lunar<Object>::RegType methods[]; 

 private:
  /**
   * Objects can only travel in 2 dimensions.
   */
  enum Dir { UP, DOWN, LEFT, RIGHT };
  typedef std::pair< Key, std::string > KeyEntry;
  friend class ObjectAttorney;

  /**
   * The standard constructor used by ObjectAttorney.
   * @param filepath path to Object's XML file.
   * @param tileX x tile coordinate Object will start on.
   * @param tileY y tile coordinate Object will start on.
   * @param strip animation strip Object will begin with.
   */
  Object( const std::string &filepath, int tileX, int tileY, int strip );

  //@{
  /**
   * Restricts copy constructor and assignment.
   */
  Object( const Object &object );
  Object& operator=( const Object &object );
  //@}

  /**
   * Calls Object's 'Init' function in its lua script (if defined), passing
   * this Object as argument.
   */
  void Init();

  /**
   * If Object isn't moving, checks keys registered for this Object, calling 
   * their corresponding lua function with self as argument if the key 
   * specifications are met (held down long enough, etc.).
   */
  void HandleEvents();

  /**
   * Calls Object's 'HandleCollision' function in its lua script (if defined),
   * passing self as first argument and Object that this Object collided
   * with as second argument. Once an Object collides with another, those
   * Objects won't 'HandleCollision' with each other again until after they
   * separated (unless user undoes this with SetNotColliding). 
   * @param collisionObj Object this Object collided with.
   */
  void UpdateCollision( Object* collisionObj );

  /**
   * If the Object isn't moving, calls Object's 'AI' function in its 
   * lua script (if defined) passing self as argument. Otherwise, updates
   * movement for Object.
   */
  void UpdateAI();

  /**
   * Updates Object's sprite animation (if applicable).
   */
  void UpdateRendering();

  //@{
  /**
   * Lua functions. See the Object API for how these functions interact with
   * the Object's lua script.
   * @param L lua state Object is in. Note that there is only one state in
   * application at any time.
   * @return Number of arguments pushed on the lua stack.
   */
  int LuaMove( lua_State *L );

  int LuaGetAnim( lua_State *L );

  int LuaSetAnim( lua_State *L );

  int LuaPlayAnim( lua_State *L );

  int LuaStopAnim( lua_State *L );

  int LuaPauseAnim( lua_State *L );

  int LuaRestartAnim( lua_State *L );

  int LuaSetReverseAnim( lua_State *L );

  int LuaGetFrame( lua_State *L );

  int LuaIsAnimating( lua_State *L );

  int LuaIsMoving( lua_State *L );

  int LuaOnCollisionCourse( lua_State *L );
 
  int LuaSetNotColliding( lua_State *L );

  int LuaGetType( lua_State *L );

  int LuaGetTile( lua_State *L );

  int LuaBlockTile( lua_State *L );

  int LuaGetDir( lua_State *L );

  int LuaSetDir( lua_State *L );

  int LuaGetTable( lua_State *L );

  int LuaSetNoClip( lua_State *L );

  int LuaResetTimer( lua_State *L );

  int LuaGetElapsedTime( lua_State *L );

  int LuaGetSpeed( lua_State *L );

  int LuaSetSpeed( lua_State *L );

  int LuaSlowDown( lua_State *L );

  int LuaSpeedUp( lua_State *L );

  int LuaPrint( lua_State *L );

  int LuaSetText( lua_State *L );

  int LuaClearText( lua_State *L );
  
  int LuaSetTextFont( lua_State *L );

  int LuaSetTextSize( lua_State *L );

  int LuaSetTextColor( lua_State *L );

  int LuaSetTextRotation( lua_State *L );

  int LuaScaleText( lua_State *L );

  int LuaSetTextPos( lua_State *L );

  int LuaSetPrintTime( lua_State *L );

  //@}

  //@{
  /**
   * Helper functions.
   */

  /**
   * Loads all data except collision data from Object's XML file.
   * @param filepath path to Object's XML file.
   * @return True if load was successful (no syntax errors in file).
   */
  bool LoadObjectData( const std::string &filepath );

  /**
   * Loads collision data in Object's XML file.
   * @param filepath path to Object's XML file.
   * @return True if load was successful (no syntax errors in file).
   */
  bool LoadCollisionData( const std::string &filepath );

  /**
   * Stores reference to Object's Lua table in its script to enable calling
   * Lua functions from application. If Object doesn't have a script or doesn't
   * return a table from its script, reference is nil and script functions 
   * won't be called on it.
   */
  void InitLua();

  /**
   * Updates movement of Object given its direction, speed, etc. Assumes that
   * Object is currently moving.
   */
  void MovementUpdate();

  /**
   * Realigns object if it has traveled past the center of the next tile it
   * is traveling to. 
   */
  void Realign();

  /**
   * Calls function in Object's script, passing self as argument. If reference
   * to Object's table is nil, nothing happens.
   * @param funcName name of function in Object's script to call.
   */
  void CallScriptFunc( std::string funcName );

  /**
   * @return Object's x and y velocity in a 2D vector. Upward and leftward vel
   * are negative, downward and rightward vel are positive.
   */
  nt::core::FloatVec GetVelocityVector();

  /**
   * @return Opposite direction Object is traveling.
   */
  Dir GetOppositeDir( Dir dir );
  //@}

  /***********************
   Private Data Members
  ***********************/
  /**
   * If true, keep moving in m_direction.
   */
  bool m_moving; 

  /**
   * If true, block other Objects from accessing this Object's tile.
   */
  bool m_blockingTile;

  /**
   * If true, allow this Object to pass through objects blocking tiles and
   * non-crossable tiles.
   */
  bool m_noClip;

  /**
   * Pointer to CallScriptFunc method.
   */
  const boost::function1<void, std::string&> m_ptrCallScriptFunc;

  /**
   * Direction Object is currently facing.
   */
  Dir m_direction; 

  /**
   * Distance traveled (in pixels) from last point of alignment on grid.
   */
  float m_distance; 

  /**
   * Speed in pixels per second.
   */
  float m_speed; 

  /**
   * Handles input for Object.
   */
  InputHandler m_input;

  /**
   * Unique key into Lua registry index that holds reference to Object's table 
   * (if it has one). Initialized to LUA_NOREF. Assigned to in InitLua so if 
   * Object's script exists and returns table (no longer LUA_NOREF), m_id will 
   * be a valid key associated with reference in the Lua registry index. 
   */
  int m_id; 

  /**
   * (x,y) coordinates of Object on tile map.
   */
  nt::core::IntVec m_coords; 
  
  /**
   * Object's bounding box for collision.
   */
  nt::core::FloatRect m_collisionRect; // Object's collision box

  /**
   * Timer that Object can use from its script.
   */
  sf::Clock m_timer; // Timer that Object can use from its script

  /**
   * String that Object can manipulate from its script. This is rendered.
   */
  nt::core::TimedString m_text; 

  /**
   * Objects this Object is colliding with.
   */
  std::list<Object*> m_collidingWith; 

  /**
   * File path to this Object's lua script (if it has one).
   */
  std::string m_luaScript; 

  /**
   * Type of object, equivalent to the Object's file name without the '.xml'.
   */
  std::string m_type; 
};


/**
 * Establishes Attorney-Client idiom between Object and ObjectManager. 
 * Since Object only needs to be accessed by ObjectManager, and 'friend'
 * doesn't offer any granularity (it's all or nothing), it is appropriate to
 * establish a contract between Object and ObjectManager to give ObjectManager
 * access to only some of Object, while still completely encapsulating Object
 * from the rest of the application.
 * Note that no NULL pointer checking is done. ObjectManager guarantees that
 * all pointers passed are valid.
 *
 * @see<a href="http://www.drdobbs.com/184402053">Dr.Dobb's</a>
 */
class ObjectAttorney {
  friend class ObjectManager;

  /**
   * Constructs Object via dynamic allocation.
   * @param filepath path to Object's XML file
   * @param x x coordinate of tile that Object starts on
   * @param y y coordinate of tile that Object starts on
   * @param strip animation corresponding to clip on sprite sheet that
   *              Object will use as its first animation 
   * @return pointer to the dynamically allocated Object
   */
  static Object* Create( 
    const std::string &filepath, 
    int x, 
    int y, 
    int strip ) 
  { return new Object( filepath, x, y, strip ); }

  /**
   * Calls Object's Init function.
   * @param obj to call Init on
   */
  static void Init( Object *obj )
  { obj->Init(); }
  
  /**
   * Calls Object's HandleEvents function.
   * @param obj object to call HandleEvents on
   */
  static void HandleEvents( Object *obj ) 
  { obj->HandleEvents(); }

  /**
   * Calls Object's HandleCollision function.
   * @param obj object to call HandleCollision on
   * @param collisionObj object that obj is colliding with
   */ 
  static void UpdateCollision( Object *obj, Object* collisionObj ) 
  { obj->UpdateCollision( collisionObj ); }

  /**
   * Calls Object's UpdateAI function.
   * @param obj object to call UpdateAI on
   */
  static void UpdateAI( Object *obj ) 
  { obj->UpdateAI(); }

  /**
   * Calls Object's UpdateRendering function.
   * @param obj object to call UpdateRendering on
   */
  static void UpdateRendering( Object *obj ) 
  { obj->UpdateRendering(); }

  /**
   * Returns tile coordinates of Object.
   * @param obj obj to call GetTile on.
   * @return obj's tile coordinates.
   */
  static const nt::core::IntVec& GetTile( const Object *obj )
  { return obj->m_coords; }
  
  /**
   * Returns whether Object is blocking the tile it is on.
   * @param obj object to check
   * @return true if obj is blocking tile, false otherwise
   */
  static int BlockingTile( const Object *obj )
  { return obj->m_blockingTile; }
 
  /**
   * Returns the Object's type (name of it's XML file with .xml cut off)
   * @param obj object to retrieve type from
   * @return object's type
   */
  static const std::string& GetType( const Object *obj )
  { return obj->m_type; }

  /**
   * Returns the Object's collision rectangle (bounding box if you prefer).
   * @param obj object to retrieve collision rectangle from
   * @return obj's collision rectangle
   */
  static const nt::core::FloatRect& GetRect( const Object *obj )
  { return obj->m_collisionRect; }
 
  /**
   * Returns whether other is in obj's list of Objects it believes it
   * is colliding with.
   * @param obj object thats list is to be checked
   * @param other object to check for in obj's list
   * @return Whether other is in obj's list
   */
  static bool IsCollidingWith(
    const Object *obj, 
    const Object* other )
  { return ( std::find( 
               obj->m_collidingWith.begin(), 
               obj->m_collidingWith.end(), 
               other 
             ) != obj->m_collidingWith.end()); }

  /**
   * Removes object passed from Object's list of Objects it believes
   * it is currently colliding with.
   * @param obj object thats collision list needs to be updated
   * @param other object that needs to be removed from obj's collision list
   */
  static void RemoveFromCollidingWith( Object *obj, Object *other )
  { obj->m_collidingWith.remove( other ); }

  /**
   * Returns sf::String that Object holds.
   * @param obj Object to get sf::String from.
   * @return sf::String that Object holds.
   */
  static const sf::String& GetText( const Object *obj )
  { return obj->m_text; }
};  

#endif // OBJECT_H

