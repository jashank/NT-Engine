#include "ObjectManager.h"

#include <utility>

#include "AnimSprite.h"
#include "Camera.h"
#include "Object.h"
#include "Rect.h"
#include "StateComm.h"
#include "tinyxml.h"
#include "Utilities.h"
#include "Vector.h"
#include "Window.h"

/*******************************
 * Comparison Functors
 ******************************/
struct ObjectManager::CreationCmp {
  bool operator()( Object *a, Object *b ) {
    return ObjectAttorney::GetCreationNum( a ) <
           ObjectAttorney::GetCreationNum( b );
  }
};


struct ObjectManager::YPosCmp {
  bool operator()( Object *a, Object *b ) {
    int aPos = ObjectAttorney::GetSprite( a ).GetPosition().y;
    int bPos = ObjectAttorney::GetSprite( b ).GetPosition().y;
    if ( aPos != bPos ) {
      return aPos < bPos;
    }

    int aNum = ObjectAttorney::GetCreationNum( a );
    int bNum = ObjectAttorney::GetCreationNum( b );
    return aNum < bNum;
  }
};

/*******************************
 * Typedefs
 ******************************/
namespace {
  typedef std::set<Object*, ObjectManager::CreationCmp>::iterator SetItr;
  typedef std::set<Object*, ObjectManager::YPosCmp>::iterator RenderSetItr;
}

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

  m_objGrid->SetRange( 0, 0, mapWidth - 1, mapHeight - 1 );
  std::set<Object*, CreationCmp> set;
  FillSet<CreationCmp>( set );
  for ( SetItr obj = set.begin(); obj != set.end(); ++obj ) {
    ObjectAttorney::Init( *obj );
  }

  return true;
}


void ObjectManager::HandleEvents( const Camera & cam ) {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  std::set<Object*, CreationCmp> set;
  FillSet<CreationCmp>( set );
  for ( SetItr obj = set.begin(); obj != set.end(); ++obj ) {
    ObjectAttorney::HandleEvents( *obj );
  }
}


void ObjectManager::Update( float dt, const Camera &cam ) {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  std::set<Object*, CreationCmp> set;
  FillSet<CreationCmp>( set );

  // Need to separate collision from logic
  for ( SetItr obj = set.begin(); obj != set.end(); ++obj ) {
    UpdateCollisions( *obj, cam );
  }
  for ( SetItr obj = set.begin(); obj != set.end(); ++obj ) {
    Object *object = *obj;
    ObjectAttorney::UpdateAI( object, dt );
    const nt::core::IntRect &lastTiles =
      ObjectAttorney::GetLastTileRange( object );
    const nt::core::IntRect &currTiles = 
      ObjectAttorney::GetTileRange( object );
    m_objGrid->MoveElem( object, lastTiles, currTiles );
  }

  // Delete all objects ready to be destroyed
  for ( unsigned int i = 0; i < m_toBeDestroyed.size(); ++i ) {
    Object *delObj = m_toBeDestroyed[i];
    
    const nt::core::IntRect &tiles =
      ObjectAttorney::GetTileRange( &(*delObj) );
    m_objGrid->RemoveElem( delObj, tiles );
        
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


void ObjectManager::Render( float alpha, const Camera &cam )  {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  std::set<Object*, YPosCmp> set;
  FillSet<YPosCmp>( set );

  for ( RenderSetItr obj = set.begin(); obj != set.end(); ++obj ) {
    Object *object = *obj;
    // Interpolate the sprite's position for blending, then turn it back.
    // Copy current position because it will change when interpolated.
    sf::Vector2f pos = ObjectAttorney::GetSpritePosition( object );
    ObjectAttorney::InterpolateSprite( object, alpha );
    nt::window::Draw( ObjectAttorney::GetSprite( object ) );
    nt::window::Draw( ObjectAttorney::GetText( object ));
    ObjectAttorney::SetSpritePosition( object, pos );
  }
}


bool ObjectManager::ObjectBlockingTile( int x, int y ) const {
  m_objGrid->SetRange( x, y, x, y );
  while ( Object *obj = m_objGrid->GetElem() ) {
    if ( ObjectAttorney::BlockingTileRange( obj )) {
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

  Lunar<Object>::push( L, NearestObject( type, tileX, tileY ));
  return 1;
}


int ObjectManager::LuaGetNearestToObject( lua_State *L ) {
  if ( !lua_isstring( L, -2 )) {
    LogLuaErr( "String not passed for object type to GetNearestToObject." );
    return 0;
  }
  std::string type = lua_tostring( L, -2 );

  Object *obj = NULL;
  if ( !(obj = Lunar<Object>::check( L, -1 ))) {
    LogLuaErr( "Object not passed for second argument to GetNearestToObject" );
    return 0;
  }

  const nt::core::IntRect &tiles = ObjectAttorney::GetTileRange( obj ); 
  int tileX = tiles.topLeft.x;
  int tileY = tiles.topLeft.y;

  Lunar<Object>::push( L, NearestObject( type, tileX, tileY, obj ));
  return 1;
}


int ObjectManager::LuaGetObjectOnTile( lua_State *L ) {
  if ( !lua_isnumber( L, 1 ) ) {
    LogLuaErr( "Number not passed to x position in GetObjectOnTile." );
    return 0;
  }
  int tileX = lua_tointeger( L, 1 );

  if ( !lua_isnumber( L, 2 ) ) {
    LogLuaErr( "Number not passed to y position in GetObjectOnTile." );
    return 0;
  }
  int tileY = lua_tointeger( L, 2 );

  std::string type = "";
  if ( lua_gettop( L ) == 3 ) {
    if ( !lua_isstring( L, 3 )) {
      LogLuaErr( "String not passed to type argument in GetObjectOnTile." );
      return 0;
    }
    type = lua_tostring( L, 3 );
  }

  if ( nt::state::InRange( tileX, tileY )) {
    if ( type == "" ) {
      Lunar<Object>::push( L, ObjectOnTile( tileX, tileY ));
    } else {
      Lunar<Object>::push( L, ObjectOnTile( tileX, tileY, type ));
    }
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
  
  const nt::core::IntRect &tiles = ObjectAttorney::GetTileRange( obj );
  m_objGrid->AddElem( obj, tiles );
}


void ObjectManager::RemoveObject( Object *obj ) {
  if ( std::find( m_toBeDestroyed.begin(),
    m_toBeDestroyed.end(), obj ) == m_toBeDestroyed.end() ) {
    m_toBeDestroyed.push_back( obj );
  }
}


Object *ObjectManager::FindObject( const std::string &type ) const {
  std::pair<MapItrConst, MapItrConst> objects =
    m_objTypes.equal_range( type ); 
  if ( objects.first != objects.second ) {
    return ( (*(objects.first)).second );
  }
  return NULL;
}


Object *ObjectManager::ObjectOnTile( int x, int y ) const {
  m_objGrid->SetRange( x, y, x, y );
  while ( Object *obj = m_objGrid->GetElem() ) {
    return obj;
  }
  return NULL;
}


Object *ObjectManager::ObjectOnTile( int x, int y, std::string &type ) const {
  m_objGrid->SetRange( x, y, x, y );
  while ( Object *obj = m_objGrid->GetElem() ) {
    if ( ObjectAttorney::GetType( obj ) == type ) {
      return obj;
    }
  }
  return NULL;
}


Object *ObjectManager::NearestObject( 
  std::string &type, 
  int x, 
  int y, 
  Object *exclude 
) const {
  int distanceX = nt::state::GetMapWidth();
  int distanceY = nt::state::GetMapHeight();

  Object *nearestObj = NULL;

  std::pair<MapItrConst, MapItrConst> keyRange =  
    m_objTypes.equal_range( type );
  
  for ( MapItrConst itr = keyRange.first; itr != keyRange.second; ++itr ) {
    Object *obj = (*itr).second;
    if ( obj == exclude ) {
      continue;
    }

    const nt::core::IntRect &tiles = ObjectAttorney::GetTileRange( obj );
    if ( tiles.Contains( x, y )) {
      return obj;
    }

    int distanceX2 = 0;
    if ( x < tiles.topLeft.x ) {
      distanceX2 = tiles.topLeft.x - x;
    } else if ( x > tiles.bottomRight.x ) { 
      distanceX2 = x - tiles.bottomRight.x;
    }

    int distanceY2 = 0;
    if ( y < tiles.topLeft.y ) {
      distanceY2 = tiles.topLeft.y - y;
    } else if ( y > tiles.bottomRight.y ) {
      distanceY2 = y - tiles.topLeft.y;
    }

    if (( distanceX2 + distanceY2 ) < ( distanceX + distanceY )) {
      distanceX = distanceX2;
      distanceY = distanceY2;
      nearestObj = obj;
    }
  }

  return nearestObj;
}


void ObjectManager::UpdateCollisions( Object *obj, const Camera &cam ) {
  int tileSize = nt::state::GetTileSize();

  const nt::core::FloatRect &objRect = ObjectAttorney::GetRect( obj );

  nt::core::IntRect tileRange;
  tileRange.topLeft.x = ( objRect.topLeft.x / tileSize );
  tileRange.bottomRight.x = ( objRect.bottomRight.x / tileSize );
  tileRange.topLeft.y = ( objRect.topLeft.y / tileSize );
  tileRange.bottomRight.y = ( objRect.bottomRight.y / tileSize );
  nt::state::CullTileRect( tileRange );

  m_objGrid->SetRange( tileRange.topLeft.x, tileRange.topLeft.y,
                       tileRange.bottomRight.x, tileRange.bottomRight.y );
  std::set<Object*, CreationCmp> set;
  FillSet<CreationCmp>( set );

  for ( SetItr itr = set.begin(); itr != set.end(); ++itr ) {
    Object *colObj = *itr;

    if ( colObj != obj && std::find( 
         m_toBeDestroyed.begin(), m_toBeDestroyed.end(), colObj ) ==
         m_toBeDestroyed.end()) {

      bool collidingWithObj = ObjectAttorney::IsCollidingWith( obj, colObj );

      bool intersects =
        objRect.Intersects( ObjectAttorney::GetRect( colObj ));

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
  const nt::core::IntRect &view = cam.GetAdjustedFocus( xadj, yadj );
  tLx = view.topLeft.x;
  tLy = view.topLeft.y;
  bRx = view.bottomRight.x;
  bRy = view.bottomRight.y;
}

