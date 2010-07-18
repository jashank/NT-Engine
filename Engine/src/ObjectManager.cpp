#include "ObjectManager.h"

#include <queue>

#include <cstdlib>

#include <SFML/Graphics.hpp>

#include "App.h"
#include "Config.h"
#include "Object.h"
#include "State.h"
#include "tinyxml.h"
#include "Utilities.h"

/*******************************************
 Data Members
*******************************************/
const luaL_Reg ObjectManager::LuaFuncs[] = {
  { "CreateObject", LuaCreateObject },
  { "DestroyObject", LuaDestroyObject },
  { "GetObject", LuaGetObject },
  { "GetObjects", LuaGetObjects },
  { "GetNearestObject", LuaGetNearestObject },
  { "GetObjectOnTile", LuaGetObjectOnTile },
  { "ObjectBlockingTile", LuaObjectBlockingTile },
  { NULL, NULL }
};

/********************************
Constructor and Destructor
********************************/
ObjectManager::~ObjectManager() {
  for ( ObjItr obj = m_objects.begin();
        obj != m_objects.end(); ++obj ) {
    if ( *obj != NULL ) {
      SAFEDELETE( *obj );
    }
  }
}

/*******************************
Public Methods
*******************************/
bool ObjectManager::LoadData( const TiXmlElement *dataRoot ) {
  const TiXmlElement *objectType = dataRoot->FirstChildElement( "object" );
  if ( objectType ) {
    do {
      const char *path = objectType->Attribute( "path" );
      if ( path ) {
        const TiXmlElement *instance = objectType->FirstChildElement( "inst" );
        if ( instance ) {
          do {
            int x, y, strip = 0;
            instance->QueryIntAttribute( "x", &x );
            instance->QueryIntAttribute( "y", &y );
            instance->QueryIntAttribute( "strip", &strip );
            if ( x >= 0 && y >= 0 && strip >= 0 ) {
              AddObject( new Object( path, x, y, strip ));
            } else {
              LogErr( "Tile location or strip negative for Object in state file." );
              return false;
            }
          } while ( (instance = instance->NextSiblingElement( "inst" )) );
        } else {
          LogErr( "No instances specified for Object in state file." );
          return false;
        }
      } else {
        LogErr( "No path specified for Object in state file." );
        return false;
      }
    } while ( (objectType = objectType->NextSiblingElement( "object" )) );
  } else {
    LogErr( "No object specified in <objects>. Thus, not necessary." );
    return false;
  }
  return true;
}


void ObjectManager::HandleEvents() {
  ObjItr obj = m_objects.begin();
  for ( ; obj != m_objects.end(); obj++ ) {
    if ( *obj != NULL ) {
      ( *obj )->HandleEvents();
    }
  }
}


void ObjectManager::Update() {
  ObjItr obj = m_objects.begin();
  // Size used because DetectCollision could potentially move
  // elements to the end of the list. Don't need to worry about
  // iterating past end because it is guaranteed that no objects
  // will be removed in UpdateCollision().
  for ( unsigned int i = 0; i < m_objects.size(); ++i ) {
    if ( *obj != NULL ) {
      ( *obj )->UpdateCollision( DetectCollision( *obj ));
    }
    ++obj;
  }

  obj = m_objects.begin();
  for ( ; obj != m_objects.end(); obj++ ) {
    if ( *obj != NULL ) {
      ( *obj )->UpdateAI();
    }
  }

  obj = m_objects.begin();
  for ( ; obj != m_objects.end(); obj++ ) {
    if ( *obj != NULL ) {
      ( *obj )->UpdateRendering();
    }
  }

  for ( unsigned int i = 0; i < m_toBeDestroyed.size(); i++ ) {
    m_objects.remove( m_toBeDestroyed[i] );
    SAFEDELETE( m_toBeDestroyed[i] );
  }
  m_toBeDestroyed.clear();
}


void ObjectManager::Render() const {
  std::priority_queue< std::pair<float, Object*> > renderOrder;

  for ( ObjItrConst obj = m_objects.begin();
        obj != m_objects.end(); obj++ ) {
    if ( *obj != NULL ) {
      renderOrder.push(
        std::make_pair( -( (*obj)->GetPosition().y ), *obj ) );
    }
  }

  while ( !renderOrder.empty() ) {
    App::GetApp()->Draw( *renderOrder.top().second );
    renderOrder.pop();
  }
}


bool ObjectManager::ObjectBlockingTile( int x, int y ) const {
  for ( ObjItrConst obj = m_objects.begin();
        obj != m_objects.end(); obj++ ) {
    if ( *obj ) {
      if (( *obj )->GetTileX() == x &&
          ( *obj )->GetTileY() == y &&
          ( *obj )->BlockingTile()) {
        return true;
      }
    }
  }

  return false;
}

/***************************************
 * Lua Functions
 * ************************************/
void ObjectManager::RegisterLuaFuncs( lua_State *L ) {
  luaL_register( L, "State", LuaFuncs );
  Lunar<Object>::Register( L );
}


int ObjectManager::LuaCreateObject( lua_State *L ) {
  if( !lua_isstring( L, -3 )) {
    LogLuaErr( "String not passed for file path in CreateObject." );
    return luaL_error( L, "String not passed for file path in CreateObject." );
  }
  std::string path = lua_tostring( L, -3 );

  if( !lua_isnumber( L, -2 ) || !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to tile location in CreateObject." );
    return luaL_error( L,
      "Number not passed to tile location  in CreateObject." );
  }
  int tileX = lua_tointeger( L, -2 );
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    Object *newObject = new Object( path, tileX, tileY, 0 );
    Inst().AddObject( newObject );
    Lunar<Object>::push( L, newObject );
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to CreateObject" );
    return 0;
  }
}


int ObjectManager::LuaDestroyObject( lua_State *L ) {
  Object *ObjectToDestroy = Lunar<Object>::check(L, 1);
  if ( ObjectToDestroy ) {
    lua_remove( L, 1 );
    Inst().RemoveObject( ObjectToDestroy );
  }
  return 0;
}


int ObjectManager::LuaGetObject( lua_State *L ) {
  if ( !lua_isstring( L, -1 ) ) {
    LogLuaErr( "String not passed for Object type in GetObject." );
    return luaL_error( L, "String not passed for Object type in GetObject." );
  }
  std::string ObjectType = lua_tostring( L, -1 );

  Lunar<Object>::push( L, Inst().GetObject( ObjectType ));
  return 1;
}


int ObjectManager::LuaGetObjects( lua_State *L ) {
  if ( !lua_isstring( L, -1 ) ) {
    LogLuaErr( "String not passed for Object type in GetObject." );
    return luaL_error( L, "String not passed for Object type in GetObject." );
  }
  std::string type = lua_tostring( L, -1 );

  lua_newtable( L );
  int newTable = lua_gettop( L );
  int index = 1; // Lua table indices start at 1

  std::list<Object*>::const_iterator obj = Inst().m_objects.begin();
  while ( obj != Inst().m_objects.end()) {
    if ( *obj && ( *obj )->GetType() == type ) {
      Lunar<Object>::push( L, *obj );
      lua_rawseti( L, newTable, index );
      ++index;
    }
    ++obj;
  }
  return 1;
}


int ObjectManager::LuaGetNearestObject( lua_State *L ) {
  if ( !lua_isstring( L, -3 )) {
    LogLuaErr( "String not passed for object type to GetNearestObject." );
    return luaL_error( L,
      "String not passed for object type to GetNearestObject." );
  }
  std::string type = lua_tostring( L, -3 );

  if ( !lua_isnumber( L, -2 ) || !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed for tile location to GetNearestObject" );
    return luaL_error( L,
      "Number not passed for tile location to GetNearestObject" );
  }
  int x = lua_tointeger( L, -2 );
  int y = lua_tointeger( L, -1 );

  int distanceX = 100; // Turn into width of 3d vector when implemented
  int distanceY = 100; // Turn into height of 3d vector when implemented
  Object *nearestObj = NULL;

  std::list<Object*>::const_iterator obj = Inst().m_objects.begin();
  while ( obj != Inst().m_objects.end()) {
    if ( *obj ) {
      if (( type != "" && ( *obj )->GetType() == type ) || ( type == "" )) {
        int distanceX2 = abs(( *obj )->GetTileX() - x );
        int distanceY2 = abs(( *obj )->GetTileY() - y );
        if (( distanceX2 + distanceY2 ) < ( distanceX + distanceY )) {
          distanceX = distanceX2;
          distanceY = distanceY2;
          nearestObj = *obj;
        }
      }
    }
    ++obj;
  }
  Lunar<Object>::push( L, nearestObj );
  return 1;
}


int ObjectManager::LuaGetObjectOnTile( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in GetObjectOnTile." );
    return luaL_error( L, "Number not passed to x position in GetObjectOnTile." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in GetObjectOnTile." );
    return luaL_error( L, "Number not passed to y position in GetObjectOnTile." );
  }
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    Lunar<Object>::push( L, Inst().ObjectOnTile( tileX, tileY ));
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to GetObjectOnTile" );
    return 0;
  }
}


int ObjectManager::LuaObjectBlockingTile( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in ObjectBlockingTile" );
    return luaL_error( L, "Number not passed to x position in ObjectBlockingTile." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in ObjectBlockingTile." );
    return luaL_error( L, "Number not passed to y position in ObjectBlockingTile." );
  }
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    lua_pushboolean( L, Inst().ObjectBlockingTile( tileX, tileY ));
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to ObjectBlockingTile" );
    return 0;
  }
}

/********************************************
  Private Methods
*********************************************/
void ObjectManager::AddObject( Object *object ) {
  m_objects.push_back( object );
  object->Play();
}


void ObjectManager::RemoveObject( Object *object ) {
  if ( std::find( m_toBeDestroyed.begin(),
    m_toBeDestroyed.end(), object ) == m_toBeDestroyed.end() ) {
    m_toBeDestroyed.push_back( object );
  }
}


Object* ObjectManager::GetObject( const std::string &type ) const {
  for ( ObjItrConst obj = m_objects.begin();
        obj != m_objects.end(); obj++ ) {
    if ( *obj != NULL ) {
      if (( *obj )->GetType() == type ) {
        return *obj;
      }
    }
  }

  return NULL;
}


Object* ObjectManager::ObjectOnTile( int x, int y ) const {
  for ( ObjItrConst obj = m_objects.begin();
        obj != m_objects.end(); obj++ ) {
    if ( *obj != NULL ) {
      if (( *obj )->GetTileX() == x &&
          ( *obj )->GetTileY() == y ) {
        return *obj;
      }
    }
  }

  return NULL;
}


Object* ObjectManager::DetectCollision( const Object *
  object ) {
  const sf::FloatRect &mainObj = object->GetCollisionBox();

  for ( ObjItr obj = m_objects.begin();
        obj != m_objects.end(); obj++ ) {
    if ( *obj && *obj != object ) {
      const sf::FloatRect &otherObj = ( *obj )->GetCollisionBox();
      if( mainObj.Intersects( otherObj ) ) {
        // So next collision check will return a different object colliding
        // with 'object' if there is one
        m_objects.splice( m_objects.end(), m_objects, obj );
        return m_objects.back();
      }
    }
  }

  return NULL;
}


ObjectManager& ObjectManager::Inst() {
  return App::GetApp()->GetCurrentState()->GetObjectManager();
}
