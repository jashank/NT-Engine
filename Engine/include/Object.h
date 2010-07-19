#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include "boost/function/function1.hpp"

#include "AnimSprite.h"
#include "InputHandler.h"
#include "Key.h"
#include "Lunar.h"

class State;

/** 
 * Object class. An Object's movement is restricted defined tile-based
 * movements. It is also heavily customizable via its associated lua script
 * and xml file.
*/
class Object : public AnimSprite {
 public:
  static const char className[];
  static Lunar<Object>::RegType methods[];
  Object( lua_State *L );
  ~Object();

 private:
  /// An Object's direction can be Up, Down, Left, or Right
  enum Dir { Up, Down, Left, Right };
  typedef std::pair< Key, std::string > KeyEntry;
  friend class ObjectAttorney;

  /// Constructor
  Object( const std::string &filepath, int tileX, int tileY, int strip );

  /// Restricts copy constructor
  Object( const Object &object );

  /// Restricts copy assignment
  Object& operator=( const Object &object );

  /// Handles events generated for Object
  void HandleEvents();

  /// Updates the Object's collision
  void UpdateCollision( Object* const collisionObj );

  /// Updates the Object's movement
  void UpdateAI();

  /// Updates the Object's rendering
  void UpdateRendering();

  /// Returns x-location of Object on  grid
  int GetTileX() const;

  /// Returns y-location of Object on  grid
  int GetTileY() const;

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

  /************************************************
   * Helper Functions
   ***********************************************/
  /// Loads Object given a path to an xml file, returning true if load was
  /// successful. Called by constructor.
  bool LoadObjectData( const std::string &filepath );

	/// Loads Object's collision data given path to xml file, returning true if
  /// load was successful. Called by constructor.
  bool LoadCollisionData( const std::string &filepath );

	/// Initializes the lua script. Called by constructor.
  void InitLua();

	/// Updates the movement of Object
  void MovementUpdate();

  /// Corrects movement of object when it exceeds next grid location
  void CorrectMovement();

  /// Calls lua function named 'funcName' that is in this Object's script
  void CallScriptFunc( std::string &funcName );

  /// Function pointer registered to CallLuaFunc for use with InputHandler
  const boost::function1<void, std::string&> m_ptrCallScriptFunc;

  /******************************************
   * Data Members
   *****************************************/
  static State *m_state; // Pointer to application's current state

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
  std::string m_luaScript; // Filepath to the lua script
  std::string m_type; // What type of  object (slime, kickle, etc.)
};


/**
 * Establishes Attorney-Client idiom between Object and ObjectManager.
 * <p>
 * Since Object only needs to be accessed by ObjectManager, and 'friend'
 * doesn't offer any granularity (it's all or nothing), it is appropriate to
 * establish a contract between Object and ObjectManager to give ObjectManager
 * access to only some of Object, while still completely encapsulating Object
 * from the rest of the application.
 * <p>
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
  static Object* const Create( 
    const std::string &filepath, 
    int x, 
    int y, 
    int strip ) 
  { return new Object( filepath, x, y, strip ); }
  
  /**
   * Calls Object's HandleEvents function.
   * @param obj object to call HandleEvents on
   */
  static void HandleEvents( Object* const obj ) 
  { obj->HandleEvents(); }

  /**
   * Calls Object's HandleCollision function.
   * @param obj object to call HandleCollision on
   * @param collisionObj object that obj is colliding with
   */ 
  static void UpdateCollision( Object* const obj, Object* const collisionObj ) 
  { obj->UpdateCollision( collisionObj ); }

  /**
   * Calls Object's UpdateAI function.
   * @param obj object to call UpdateAI on
   */
  static void UpdateAI( Object* const obj ) 
  { obj->UpdateAI(); }

  /**
   * Calls Object's UpdateRendering function.
   * @param obj object to call UpdateRendering on
   */
  static void UpdateRendering( Object* const obj ) 
  { obj->UpdateRendering(); }
  
  /**
   * Calls Object's GetTileX function.
   * @param obj object to call GetTileX on
   * @return obj's x coordinate on the tile map
   */
  static int GetTileX( const Object* const obj )
  { return obj->GetTileX(); }

  /**
   * Calls Object's GetTileY function.
   * @param obj object to call GetTileY on
   * @return obj's y coordinate on the tile map
   */
  static int GetTileY( const Object* const obj )
  { return obj->GetTileY(); }
  
  /**
   * Returns whether Object is blocking the tile it is on.
   * @param obj object to check
   * @return true if obj is blocking tile, false otherwise
   */
  static int BlockingTile( const Object* const obj )
  { return obj->m_blockingTile; }
 
  /**
   * Returns the Object's type (name of it's XML file with .xml cut off)
   * @param obj object to retrieve type from
   * @return object's type
   */
  static const std::string& GetType( const Object* const obj )
  { return obj->m_type; }

  /**
   * Returns the Object's collision rectangle (bounding box if you prefer).
   * @param obj object to retrieve collision rectangle from
   * @return obj's collision rectangle
   */
  static const sf::FloatRect& GetRect( const Object* const obj )
  { return obj->m_collisionRect; }
};  

#endif // OBJECT_H

