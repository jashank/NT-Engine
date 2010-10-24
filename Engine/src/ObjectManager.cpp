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
  m_objGrid = new nt::core::RangeMatrix3D<Object*>( width, height );

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

  m_objGrid->SetRange( 0, 0, mapWidth, mapHeight );
  while ( Object *obj = m_objGrid->GetElem() ) {
    ObjectAttorney::Init( obj );
  }

  return true;
}


void ObjectManager::HandleEvents( const Camera & cam ) {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  while ( Object *obj = m_objGrid->GetElem() ) {
    ObjectAttorney::HandleEvents( obj );
  }
}


void ObjectManager::Update( float dt, const Camera &cam ) {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  while ( Object *obj = m_objGrid->GetElem() ) {
    m_objGrid->SavePlace();
    UpdateCollisions( obj, cam );
    m_objGrid->ToPlace();
  }

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  while ( Object *obj = m_objGrid->GetElem() ) {
    ObjectAttorney::UpdateAI( obj, dt );
  }

  // Make sure object's grid positions in matrix are correct
  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  while ( Object *obj = m_objGrid->GetElem() ) {
    nt::core::IntVec tile = ObjectAttorney::GetTile( obj );
    m_objGrid->MoveReturnedElem( tile.x, tile.y );
  }

  /*
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
  */
}


void ObjectManager::Render( const Camera &cam ) const {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  std::priority_queue< std::pair<float, Object*> > renderOrder;

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  while ( Object *obj = m_objGrid->GetElem() ) {
    renderOrder.push( std::make_pair( 
      -( ObjectAttorney::GetSprite( obj ).GetPosition().y ), obj ));
  }

  while ( !renderOrder.empty() ) {
    const Object *obj = renderOrder.top().second;
    nt::window::Draw( ObjectAttorney::GetSprite( obj ) );
    nt::window::Draw( ObjectAttorney::GetText( obj ));
    renderOrder.pop();
  }
}


bool ObjectManager::ObjectBlockingTile( int x, int y ) const {
  m_objGrid->SetRange( x, y, x, y );
  while ( Object *obj = m_objGrid->GetElem() ) {
    if ( ObjectAttorney::BlockingTile( obj )) {
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
  m_objGrid->AddElem( obj, coords.x, coords.y );
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
  m_objGrid->SetRange( x, y, x, y );
  while ( Object *obj = m_objGrid->GetElem() ) {
    return obj;
  }
  return NULL;
}


void ObjectManager::UpdateCollisions( Object *obj, const Camera &cam ) {
  nt::core::FloatRect objRect = ObjectAttorney::GetRect( obj );
  nt::core::IntRect tileRange = cam.GetTileOverlap( objRect );

  m_objGrid->SetRange( tileRange.topLeft.x, tileRange.topLeft.y,
                       tileRange.bottomRight.x, tileRange.bottomRight.y );

  while ( Object *colObj = m_objGrid->GetElem() ) {
    if ( colObj != obj && std::find( 
         m_toBeDestroyed.begin(), m_toBeDestroyed.end(), *colObj ) ==
         m_toBeDestroyed.end()) {

      bool collidingWithObj = ObjectAttorney::IsCollidingWith( obj, colObj );

      bool intersects = ObjectAttorney::GetRect( obj ).Intersects( 
          ObjectAttorney::GetRect( colObj ));

      if ( !collidingWithObj && intersects ) {
        ObjectAttorney::HandleCollision( obj, colObj );
      } else if ( collidingWithObj && !intersects ) {
        ObjectAttorney::RemoveFromCollidingWith( obj, colObj );
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

