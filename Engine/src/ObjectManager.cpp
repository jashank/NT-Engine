#include "ObjectManager.h"

#include <queue>
#include <utility>

#include <cstdlib>

#include "AnimSprite.h"
#include "Camera.h"
#include "Object.h"
#include "Rect.h"
#include "StateComm.h"
#include "tinyxml.h"
#include "Utilities.h"
#include "Vector.h"
#include "Window.h"

/********************************
Constructor and Destructor
********************************/
ObjectManager::ObjectManager()
  :m_objGrid( NULL ) {}


ObjectManager::~ObjectManager() {
  SAFEDELETE( m_objGrid );
}

/*******************************
Public Methods
*******************************/
bool ObjectManager::LoadData( const TiXmlElement *dataRoot, lua_State *L ) {
  // State guaranteed to be loaded and TileManager guaranteed to be loaded
  // before ObjectManager
  int width = nt::state::GetMapWidth();
  int height = nt::state::GetMapHeight();
  m_objGrid = new nt::core::Matrix2D<ObjectList>( width, height );

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
            if ( nt::state::InRange( x, y ) && strip >= 0 ) {
              AddObject( ObjectAttorney::Create( path, x, y, strip, L ));
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
  
  int mapWidth = nt::state::GetMapWidth();
  int mapHeight = nt::state::GetMapHeight();

  for ( int x = 0; x < mapWidth; ++x ) {
    for ( int y = 0; y < mapHeight; ++y ) {
      nt::core::Matrix2D<ObjectList>::iterator objList =
        m_objGrid->Get( x, y );
      for ( ListItr obj = objList->begin(); obj != objList->end(); ++obj ) {
        ObjectAttorney::Init( *obj );
      }
    }
  }

  return true;
}


void ObjectManager::HandleEvents( const Camera & cam ) {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  for ( int x = tLx; x <= bRx; ++x ) {
    for ( int y = tLy; y <= bRy; ++y ) {
      nt::core::Matrix2D<ObjectList>::iterator objList =
        m_objGrid->Get( x, y );
      for ( ListItr obj = objList->begin(); obj != objList->end(); ++obj ) {
        ObjectAttorney::HandleEvents( *obj );
      }
    }
  }
}


void ObjectManager::Update( float dt, const Camera &cam ) {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  // Size used in inner loop because DetectCollision could potentially move
  // elements to the end of the list. Don't need to worry about
  // iterating past end because it is guaranteed that no objects
  // will be removed in UpdateCollision
  for ( int x = tLx; x <= bRx; ++x ) {
    for ( int y = tLy; y <= bRy; ++y ) {
      nt::core::Matrix2D<ObjectList>::iterator objList =
        m_objGrid->Get( x, y );

      unsigned int initSize = objList->size();
      ListItr obj = objList->begin();

      for ( unsigned int i = 0; i < initSize; ++i ) {
        Object *const otherObj = DetectCollision( *obj, cam );
        if ( otherObj ) {
          ObjectAttorney::UpdateCollision( *obj, otherObj );
        }
        ++obj;
      }
    }
  }

  for ( int x = tLx; x <= bRx; ++x ) {
    for ( int y = tLy; y <= bRy; ++y ) {
      nt::core::Matrix2D<ObjectList>::iterator objList =
        m_objGrid->Get( x, y );

      for ( ListItr obj = objList->begin(); obj != objList->end(); ++obj) {
        ObjectAttorney::UpdateAI( *obj, dt );
      }
    }
  }

  AdjustGridCoords( tLx, tLy, bRx, bRy );

  for ( unsigned int i = 0; i < m_toBeDestroyed.size(); i++ ) {
    Object *delObj = m_toBeDestroyed[i];
    
    nt::core::IntVec coords = ObjectAttorney::GetTile( delObj );
    m_objGrid->Get(coords.x, coords.y)->remove( delObj );
        
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


void ObjectManager::Render( const Camera &cam ) const {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  std::priority_queue< std::pair<float, Object*> > renderOrder;

  for ( int x = tLx; x <= bRx; ++x ) {
    for ( int y = tLy; y <= bRy; ++y ) {
      nt::core::Matrix2D<ObjectList>::iterator objList =
        m_objGrid->Get( x, y );

      for ( ListItr obj = objList->begin(); obj != objList->end(); ++obj ) {
        renderOrder.push( std::make_pair( 
          -( ObjectAttorney::GetSprite( *obj ).GetPosition().y ), *obj ));
      }
    }
  }

  while ( !renderOrder.empty() ) {
    const Object *obj = renderOrder.top().second;
    nt::window::Draw( ObjectAttorney::GetSprite( obj ) );
    nt::window::Draw( ObjectAttorney::GetText( obj ));
    renderOrder.pop();
  }
}


bool ObjectManager::ObjectBlockingTile( int x, int y ) const {
  ObjectList *list = m_objGrid->Get( x, y );
  for( ListItr obj = list->begin(); obj != list->end(); ++obj ) { 
    if ( ObjectAttorney::BlockingTile( *obj )) {
      return true;
    }
  }
  return false;
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
    Object *newObject = ObjectAttorney::Create( path, tileX, tileY, 0, L );
    AddObject( newObject );
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
    RemoveObject( objToDestroy );
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

  Lunar<Object>::push( L, FindObject( type ));
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
    m_objTypes.equal_range( type );
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

  int distanceX = nt::state::GetMapWidth();
  int distanceY = nt::state::GetMapHeight();

  Object *nearestObj = NULL;

  std::pair<MapItrConst, MapItrConst> keyRange =  
    m_objTypes.equal_range( type );
  
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

  if ( nt::state::InRange( tileX, tileY )) {
    Lunar<Object>::push( L, ObjectOnTile( tileX, tileY ));
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

  if ( nt::state::InRange( tileX, tileY )) {
    lua_pushboolean( L, ObjectBlockingTile( tileX, tileY ));
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
  m_objGrid->Get( coords.x, coords.y )->push_back( obj );
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
  ObjectList *list = m_objGrid->Get( x, y );
  if ( !list->empty()) {
    return list->front();
  }
  return NULL;
}


Object* ObjectManager::DetectCollision( 
  Object *obj, 
  const Camera &cam ) 
{
  nt::core::FloatRect objRect = ObjectAttorney::GetRect( obj );
  nt::core::IntRect tileRange = cam.GetTileOverlap( objRect );

  for ( int x = tileRange.topLeft.x; x <= tileRange.bottomRight.x; ++x ) {
    for ( int y = tileRange.topLeft.y; y <= tileRange.bottomRight.y; ++y ) {
      nt::core::Matrix2D<ObjectList>::iterator objList =
        m_objGrid->Get( x, y );

      for ( ListItr colObj = objList->begin(); colObj != objList->end(); 
            ++colObj ) {
        if ( *colObj != obj && std::find( 
             m_toBeDestroyed.begin(), m_toBeDestroyed.end(), *colObj ) ==
             m_toBeDestroyed.end()) {

          bool collidingWithObj = 
            ObjectAttorney::IsCollidingWith( obj, *colObj );

          bool intersects = ObjectAttorney::GetRect( obj ).Intersects( 
              ObjectAttorney::GetRect( *colObj ));

          if ( !collidingWithObj && intersects ) {
            // So next collision check will return a different object colliding
            // with 'object' if there is one
            objList->splice( objList->end(), *objList, colObj );
            return objList->back();

          } else if ( collidingWithObj && !intersects ) {
            ObjectAttorney::RemoveFromCollidingWith( obj, *colObj );
          }
        }
      }
      ++objList;
    }
  }
  return NULL;
}


void ObjectManager::AdjustGridCoords( int tLx, int tLy, int bRx, int bRy ) {
  for ( int x = tLx; x <= bRx; ++x ) {
    for ( int y = tLy; y <= bRy; ++y ) {
      nt::core::Matrix2D<ObjectList>::iterator objList =
        m_objGrid->Get( x, y );

      for ( ListItr obj = objList->begin(); obj != objList->end(); ) {
        nt::core::IntVec coords = ObjectAttorney::GetTile( *obj );
        if ( x != coords.x || y != coords.y  ) {
          Object *moveObject = *obj;
          obj = objList->erase( obj );
          ObjectList *newTile = m_objGrid->Get( coords.x, coords.y );
          newTile->push_back( moveObject );
        } else {
          ++obj;
        }
      }
    }
  }
}


void ObjectManager::GetCamCoords( 
  const Camera &cam, 
  int xadj, 
  int yadj, 
  int &tLx, 
  int &tLy, 
  int &bRx, 
  int &bRy
) const {
  nt::core::IntRect view = cam.GetAdjustedFocus( xadj, yadj );
  tLx = view.topLeft.x;
  tLy = view.topLeft.y;
  bRx = view.bottomRight.x;
  bRy = view.bottomRight.y;
}

