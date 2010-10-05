#include "ObjectManager.h"

#include <queue>
#include <utility>

#include <cstdlib>

#include "App.h"
#include "Config.h"
#include "Object.h"
#include "State.h"
#include "TileManager.h"
#include "tinyxml.h"
#include "Utilities.h"
#include "Vector.h"

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
  for ( unsigned int x = 0; x < m_objGrid.size(); ++x ) {
    for ( unsigned int y = 0; y < m_objGrid[x].size(); ++y ) {
      for ( ListItr obj = m_objGrid[x][y].begin();
            obj != m_objGrid[x][y].end(); ++obj ) {
        SAFEDELETE( *obj );
      }
    }
  }
}

/*******************************
Public Methods
*******************************/
bool ObjectManager::LoadData( const TiXmlElement *dataRoot ) {
  State *state = App::GetApp()->GetCurrentState();
  // State guaranteed to be loaded and TileManager guaranteed to be loaded
  // before ObjectManager
  m_objGrid.resize( state->GetTileManager().GetMapWidth());
  for ( unsigned int i = 0; i < m_objGrid.size(); ++i ) {
    m_objGrid[i].resize( state->GetTileManager().GetMapHeight());
  }

  const TiXmlElement *objType = dataRoot->FirstChildElement( "object" );
  if ( objType ) {
    do {
      const char *path = objType->Attribute( "path" );
      if ( path ) {
        const TiXmlElement *instance = objType->FirstChildElement( "inst" );
        if ( instance ) {
          do {
            int x = 0;
            int y = 0;
            int strip = 0;
            instance->QueryIntAttribute( "x", &x );
            instance->QueryIntAttribute( "y", &y );
            instance->QueryIntAttribute( "strip", &strip );
            if ( TileOnMap( x, y ) && strip >= 0 ) {
              AddObject( ObjectAttorney::Create( path, x, y, strip ));
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
    } while ( (objType = objType->NextSiblingElement( "object" )) );
  } else {
    LogErr( "No object specified in <objects>. Thus, not necessary." );
    return false;
  }
  
  for ( unsigned int x = 0; x < m_objGrid.size(); ++x ) {
    for ( unsigned int y = 0; y < m_objGrid[x].size(); ++y ) {
      for ( ListItr obj = m_objGrid[x][y].begin();
            obj != m_objGrid[x][y].end(); ) {
        ObjectAttorney::Init( *obj );
        obj = AdjustGridCoord( x, y, obj );
      }
    }
  }
  return true;
}


void ObjectManager::HandleEvents() {
  for ( unsigned int x = 0; x < m_objGrid.size(); ++x ) {
    for ( unsigned int y = 0; y < m_objGrid[x].size(); ++y ) {
      for ( ListItr obj = m_objGrid[x][y].begin();
            obj != m_objGrid[x][y].end(); ) {
        ObjectAttorney::HandleEvents( *obj );
        obj = AdjustGridCoord( x, y, obj );
      }
    }
  }
}


void ObjectManager::Update() {
  // Size used in inner loop because DetectCollision could potentially move
  // elements to the end of the list. Don't need to worry about
  // iterating past end because it is guaranteed that no objects
  // will be removed in UpdateCollision
  for ( unsigned int x = 0; x < m_objGrid.size(); ++x ) {
    for ( unsigned int y = 0; y < m_objGrid[x].size(); ++y ) {
      ListItr obj = m_objGrid[x][y].begin();
      for ( unsigned int i = 0; i < m_objGrid[x][y].size(); ++i ) {
        Object* const otherObj = DetectCollision( *obj );
        if ( otherObj ) {
          ObjectAttorney::UpdateCollision( *obj, otherObj );
          obj = AdjustGridCoord( x, y, obj );
        } else {
          ++obj;
        }
      }
    }
  }

  for ( unsigned int x = 0; x < m_objGrid.size(); ++x ) {
    for ( unsigned int y = 0; y < m_objGrid[x].size(); ++y ) {
      for ( ListItr obj = m_objGrid[x][y].begin();
            obj != m_objGrid[x][y].end(); ) {
        ObjectAttorney::UpdateAI( *obj );
        obj = AdjustGridCoord( x, y, obj );
      }
    }
  }

  for ( unsigned int x = 0; x < m_objGrid.size(); ++x ) {
    for ( unsigned int y = 0; y < m_objGrid[x].size(); ++y ) {
      for ( ListItr obj = m_objGrid[x][y].begin();
            obj != m_objGrid[x][y].end(); ++obj ) {
        ObjectAttorney::UpdateRendering( *obj );
      }
    }
  }

  for ( unsigned int i = 0; i < m_toBeDestroyed.size(); i++ ) {
    Object *delObj = m_toBeDestroyed[i];
    
    nt::core::IntVec coords = ObjectAttorney::GetTile( delObj );
    m_objGrid[coords.x][coords.y].remove( delObj );
        
    std::string type = ObjectAttorney::GetType( delObj ); 
    std::pair<MapItr, MapItr> objects = m_objTypes.equal_range( type );
    for ( MapItr obj = objects.first; obj != objects.second; ) {
      if ((*obj).second == delObj ) {
        m_objTypes.erase( obj++ );
      } else {
        ++obj;
      }
    }
    
    SAFEDELETE( m_toBeDestroyed[i] );
  }
  m_toBeDestroyed.clear();
}


void ObjectManager::Render() const {
  std::priority_queue< std::pair<float, Object*> > renderOrder;

  for ( unsigned int x = 0; x < m_objGrid.size(); ++x ) {
    for ( unsigned int y = 0; y < m_objGrid[x].size(); ++y ) {
      for ( ListItrConst obj = m_objGrid[x][y].begin();
            obj != m_objGrid[x][y].end(); ++obj ) {
        renderOrder.push(
          std::make_pair( -( (*obj)->GetPosition().y ), *obj ) );
      }
    }
  }

  while ( !renderOrder.empty() ) {
    const Object *obj = renderOrder.top().second;
    App::GetApp()->Draw( *obj );
    App::GetApp()->Draw( ObjectAttorney::GetText( obj ));
    renderOrder.pop();
  }
}


bool ObjectManager::ObjectBlockingTile( int x, int y ) const {
  for( ListItrConst obj = m_objGrid[x][y].begin();  
        obj != m_objGrid[x][y].end(); ++obj ) {
    if ( ObjectAttorney::BlockingTile( *obj )) {
      return true;
    }
  }
  return false;
}


void ObjectManager::RegisterLuaFuncs( lua_State *L ) {
  luaL_register( L, "State", LuaFuncs );
  Lunar<Object>::Register( L );
}

/***************************************
 * Lua Functions
 * ************************************/
int ObjectManager::LuaCreateObject( lua_State *L ) {
  if( !lua_isstring( L, -3 )) {
    LogLuaErr( "String not passed for file path in CreateObject." );
    return 0;
  }
  std::string path = lua_tostring( L, -3 );

  if( !lua_isnumber( L, -2 ) || !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to tile location in CreateObject." );
    return 0;
  }
  int tileX = lua_tointeger( L, -2 );
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    Object *newObject = ObjectAttorney::Create( path, tileX, tileY, 0 );
    Inst().AddObject( newObject );
    ObjectAttorney::Init( newObject );
    Lunar<Object>::push( L, newObject );
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to CreateObject" );
    return 0;
  }
}


int ObjectManager::LuaDestroyObject( lua_State *L ) {
  Object *objToDestroy = Lunar<Object>::check(L, 1);
  if ( objToDestroy ) {
    lua_remove( L, 1 );
    Inst().RemoveObject( objToDestroy );
    return 0;
  } else {
    LogLuaErr( "No Object passed to DestroyObject." );
    return 0;
  }
}


int ObjectManager::LuaGetObject( lua_State *L ) {
  if ( !lua_isstring( L, -1 ) ) {
    LogLuaErr( "String not passed for Object type in GetObject." );
    return 0;
  }
  std::string type = lua_tostring( L, -1 );

  Lunar<Object>::push( L, Inst().FindObject( type ));
  return 1;
}


int ObjectManager::LuaGetObjects( lua_State *L ) {
  if ( !lua_isstring( L, -1 ) ) {
    LogLuaErr( "String not passed for Object type in GetObject." );
    return 0;
  }
  std::string type = lua_tostring( L, -1 );

  lua_newtable( L );
  int newTable = lua_gettop( L );
  int index = 1; // Lua table indices start at 1

  std::pair<MapItrConst, MapItrConst> objects =
    Inst().m_objTypes.equal_range( type );
  for ( MapItrConst obj = objects.first; obj != objects.second; ++obj ) {
    if ( (*obj).second ) {
      Lunar<Object>::push( L, (*obj).second );
      lua_rawseti( L, newTable, index );
      ++index;
    }
  }
  return 1;
}


int ObjectManager::LuaGetNearestObject( lua_State *L ) {
  if ( !lua_isstring( L, -3 )) {
    LogLuaErr( "String not passed for object type to GetNearestObject." );
    return 0;
  }
  std::string type = lua_tostring( L, -3 );

  if ( !lua_isnumber( L, -2 ) || !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed for tile location to GetNearestObject" );
    return 0;
  }
  int tileX = lua_tointeger( L, -2 );
  int tileY = lua_tointeger( L, -1 );

  int distanceX = 
    App::GetApp()->GetCurrentState()->GetTileManager().GetMapWidth();
  int distanceY = 
    App::GetApp()->GetCurrentState()->GetTileManager().GetMapHeight();

  Object *nearestObj = NULL;

  std::pair<MapItrConst, MapItrConst> keyRange =  
    Inst().m_objTypes.equal_range( type );
  
  for ( MapItrConst itr = keyRange.first; itr != keyRange.second; ++itr ) {
    Object *obj = (*itr).second;

    nt::core::IntVec coords = ObjectAttorney::GetTile( obj );
    int distanceX2 = abs( coords.x - tileX );
    int distanceY2 = abs( coords.y - tileY );

    if (( distanceX2 + distanceY2 ) < ( distanceX + distanceY )) {
      distanceX = distanceX2;
      distanceY = distanceY2;
      nearestObj = obj;
    }
  }

  Lunar<Object>::push( L, nearestObj );
  return 1;
}


int ObjectManager::LuaGetObjectOnTile( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in GetObjectOnTile." );
    return 0;
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in GetObjectOnTile.");
    return 0;
  }
  int tileY = lua_tointeger( L, -1 );

  if ( Inst().TileOnMap( tileX, tileY )) {
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
    return 0;
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in ObjectBlockingTile." );
    return 0;
  }
  int tileY = lua_tointeger( L, -1 );

  if ( Inst().TileOnMap( tileX, tileY )) {
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
void ObjectManager::AddObject( Object *obj ) {
  m_objTypes.insert( std::make_pair(
    ObjectAttorney::GetType( obj ), obj )); 
  
  nt::core::IntVec coords = ObjectAttorney::GetTile( obj );
  m_objGrid[coords.x][coords.y].push_back( obj );
}


void ObjectManager::RemoveObject( Object *obj ) {
  if ( std::find( m_toBeDestroyed.begin(),
    m_toBeDestroyed.end(), obj ) == m_toBeDestroyed.end() ) {
    m_toBeDestroyed.push_back( obj );
  }
}


Object* ObjectManager::FindObject( const std::string &type ) const {
  std::pair<MapItrConst, MapItrConst> objects =
    m_objTypes.equal_range( type ); 
  if ( objects.first != objects.second ) {
    return ( (*(objects.first)).second );
  }
  return NULL;
}


Object* ObjectManager::ObjectOnTile( int x, int y ) const {
  if ( !m_objGrid[x][y].empty()) {
    return m_objGrid[x][y].front();
  }
  return NULL;
}


Object* ObjectManager::DetectCollision( Object *obj ) {
  for ( unsigned int x = 0; x < m_objGrid.size(); ++x ) {
    for ( unsigned int y = 0; y < m_objGrid[x].size(); ++y ) {
      for ( ListItr otherObj = m_objGrid[x][y].begin();
            otherObj != m_objGrid[x][y].end(); ++otherObj ) {
        if ( *otherObj != obj && std::find( 
               m_toBeDestroyed.begin(), m_toBeDestroyed.end(), *otherObj ) ==
               m_toBeDestroyed.end()) {
          bool collidingWithObj = 
            ObjectAttorney::IsCollidingWith( obj, *otherObj );
          bool intersects = 
            ObjectAttorney::GetRect( obj ).Intersects( 
              ObjectAttorney::GetRect( *otherObj ));
          if ( !collidingWithObj && intersects ) {
            // So next collision check will return a different object colliding
            // with 'object' if there is one
            m_objGrid[x][y].splice( 
              m_objGrid[x][y].end(), 
              m_objGrid[x][y], 
              otherObj 
            );
            return m_objGrid[x][y].back();
          } else if ( collidingWithObj && !intersects ) {
            ObjectAttorney::RemoveFromCollidingWith( obj, *otherObj );
          }
        }
      }
    }
  }
  return NULL;
}


ObjectManager::ListItr ObjectManager::AdjustGridCoord( 
  int x, 
  int y, 
  ListItr objItr 
) {
  // GetTile guaranteed to return Vector with valid indices
  Object* const obj = *objItr;
  nt::core::IntVec coords = ObjectAttorney::GetTile( obj );
  if ( x != coords.x || y != coords.y  ) {
    objItr = m_objGrid[x][y].erase( objItr );
    m_objGrid[coords.x][coords.y].push_back( obj );
    return objItr;
  }
  return ++objItr;
} 


bool ObjectManager::TileOnMap( int x, int y ) const {
  return App::GetApp()->GetCurrentState()->GetTileManager().TileOnMap( x, y );
}


ObjectManager& ObjectManager::Inst() {
  return App::GetApp()->GetCurrentState()->GetObjectManager();
}


