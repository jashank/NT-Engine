#include "ObjectManager.h"

#include <utility>

extern "C" {
  #include "lua.h"
}

#include "Camera.h"
#include "Rect.h"
#include "tinyxml.h"
#include "Utilities.h"
#include "Vector.h"

namespace nt {

/*******************************
 * Comparison Functors
 ******************************/
struct ObjectManager::CreationCmp {
  bool operator()( const intrObj_type &a, const intrObj_type &b ) {
    return ObjectAttorney::GetCreationNum( a ) <
           ObjectAttorney::GetCreationNum( b );
  }
};


struct ObjectManager::RenderPriorityCmp {
  bool operator()( const intrObj_type &a, const intrObj_type &b ) {
    int aPriority = ObjectAttorney::GetRenderPriority( a );
    int bPriority = ObjectAttorney::GetRenderPriority( b );
    if ( aPriority != bPriority ) {
      return aPriority < bPriority;
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
  typedef std::set<ObjectManager::intrObj_type, 
            ObjectManager::CreationCmp>::iterator SetItr;
  typedef std::set<ObjectManager::intrObj_type, 
            ObjectManager::RenderPriorityCmp>::iterator RenderSetItr;
}

/***********************
 * Constructor
 **********************/
ObjectManager::ObjectManager( 
  const TiXmlElement *root, 
  const IntRect &mapRect,
  int tileSize,
  lua_State *L 
) : m_mapRect( mapRect ), m_tileSize( tileSize ) {
  LoadData( root, L );
}

/*******************************
 * Public Methods
 ******************************/
void ObjectManager::Init() {
  int mapWidth = nt::state::GetMapWidth();
  int mapHeight = nt::state::GetMapHeight();

  m_objGrid->SetRange( 0, 0, mapWidth - 1, mapHeight - 1 );
  std::set<intrObj_type, CreationCmp> set;
  FillSet( set );
  for ( SetItr obj = set.begin(); obj != set.end(); ++obj ) {
    ObjectAttorney::Init( *obj );
  }
}


void ObjectManager::HandleEvents( const Camera & cam ) {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  std::set<intrObj_type, CreationCmp> set;
  FillSet( set );
  for ( SetItr obj = set.begin(); obj != set.end(); ++obj ) {
    ObjectAttorney::HandleEvents( *obj );
  }
}


void ObjectManager::Update( float dt, const Camera &cam ) {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  std::set<intrObj_type, CreationCmp> set;
  FillSet( set );

  // Need to separate collision from logic
  for ( SetItr obj = set.begin(); obj != set.end(); ++obj ) {
    UpdateCollisions( *obj, cam );
  }
  for ( SetItr obj = set.begin(); obj != set.end(); ++obj ) {
    const intrObj_type &object = *obj;
    ObjectAttorney::UpdateAI( object, dt );
    const IntRect &lastTiles =
      ObjectAttorney::GetLastTileRange( object );
    const IntRect &currTiles =
      ObjectAttorney::GetTileRange( object );
    m_objGrid->MoveElem( object, lastTiles, currTiles );
  }

  // Delete all objects ready to be destroyed
  for ( unsigned int i = 0; i < m_toBeDestroyed.size(); ++i ) {
    const intrObj_type &delObj = m_toBeDestroyed[i];
    
    const IntRect &tiles =
      ObjectAttorney::GetTileRange( delObj );
    m_objGrid->RemoveElem( delObj, tiles );
        
    std::string type = ObjectAttorney::GetType( delObj ); 
    std::pair<nameMap_type::iterator, nameMap_type::iterator> objects =
      m_objTypes.equal_range( type );

    for ( nameMap_type::iterator obj = objects.first; 
          obj != objects.second; ) {
      if ((*obj).second == delObj ) {
        m_objTypes.erase( obj++ );
      } else {
        ++obj;
      }
    }
  }

  m_toBeDestroyed.clear();
}


void ObjectManager::Render( float alpha, const Camera &cam )  {
  int tLx, tLy, bRx, bRy;
  GetCamCoords( cam, 1, 1, tLx, tLy, bRx, bRy );

  m_objGrid->SetRange( tLx, tLy, bRx, bRy );
  std::set<intrObj_type, RenderPriorityCmp> set;
  FillSet<RenderPriorityCmp>( set );

  for ( RenderSetItr obj = set.begin(); obj != set.end(); ++obj ) {
    ObjectAttorney::Draw( *obj, alpha );
  }
}


bool ObjectManager::ObjectBlockingTile( int x, int y ) const {
  m_objGrid->SetRange( x, y, x, y );
  while ( const intrObj_type *obj = m_objGrid->GetElem() ) {
    if ( ObjectAttorney::BlockingTileRange( *obj )) {
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
    const intrObj_type newObject(
      ObjectAttorney::Create( path, tileX, tileY, 0, L ));
    AddObject( newObject );
    ObjectAttorney::Init( newObject );
    Lunar<Object>::push( L, newObject.get() );
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
    const intrObj_type obj( objToDestroy );
    RemoveObject( obj );
    return 0;
  } else {
    LogLuaErr( "No Object passed to DestroyObject." );
    return 0;
  }
}


int ObjectManager::LuaGetObject( lua_State *L ) const {
  if ( !lua_isstring( L, -1 ) ) {
    LogLuaErr( "String not passed for Object type in GetObject." );
    return 0;
  }
  std::string type = lua_tostring( L, -1 );

  Lunar<Object>::push( L, FindObject( type ));
  return 1;
}


int ObjectManager::LuaGetObjects( lua_State *L ) const {
  if ( !lua_isstring( L, -1 ) ) {
    LogLuaErr( "String not passed for Object type in GetObject." );
    return 0;
  }
  std::string type = lua_tostring( L, -1 );

  lua_newtable( L );
  int newTable = lua_gettop( L );
  int index = 1; // Lua table indices start at 1

  std::pair<nameMap_type::const_iterator, 
            nameMap_type::const_iterator> objects =
              m_objTypes.equal_range( type );

  for ( nameMap_type::const_iterator obj = objects.first; 
        obj != objects.second; ++obj ) {
      Lunar<Object>::push( L, ((*obj).second).get() );
      lua_rawseti( L, newTable, index );
      ++index;
  }
  return 1;
}


int ObjectManager::LuaGetNearestObject( lua_State *L ) const {
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


int ObjectManager::LuaGetNearestToObject( lua_State *L ) const {
  if ( !lua_isstring( L, -2 )) {
    LogLuaErr( "String not passed for object type to GetNearestToObject." );
    return 0;
  }
  std::string type = lua_tostring( L, -2 );

  Object *pObj = NULL;
  if ( !(pObj = Lunar<Object>::check( L, -1 ))) {
    LogLuaErr( "Object not passed for second argument to GetNearestToObject" );
    return 0;
  }

  const intrObj_type obj( pObj );
  const IntRect &tiles = ObjectAttorney::GetTileRange( obj ); 
  int tileX = tiles.topLeft.x;
  int tileY = tiles.topLeft.y;

  Lunar<Object>::push( L, NearestObject( type, tileX, tileY, pObj ));
  return 1;
}


int ObjectManager::LuaGetObjectOnTile( lua_State *L ) const {
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


int ObjectManager::LuaGetObjectsOnTile( lua_State *L ) const {
  if ( !lua_isnumber( L, 1 ) ) {
    LogLuaErr( "Number not passed to x position in GetObjectsOnTile." );
    return 0;
  }
  int tileX = lua_tointeger( L, 1 );

  if ( !lua_isnumber( L, 2 ) ) {
    LogLuaErr( "Number not passed to y position in GetObjectsOnTile." );
    return 0;
  }
  int tileY = lua_tointeger( L, 2 );

  std::string type = "";
  if ( lua_gettop( L ) == 3 ) {
    if ( !lua_isstring( L, 3 )) {
      LogLuaErr( "String not passed to type argument in GetObjectsOnTile." );
      return 0;
    }
    type = lua_tostring( L, 3 );
  }

  if ( nt::state::InRange( tileX, tileY )) {
    lua_newtable( L );
    int newTable = lua_gettop( L );
    int index = 1; // Lua table indices start at 1

    m_objGrid->SetRange( tileX, tileY, tileX, tileY );
    if ( type == "" ) {
      while ( const intrObj_type *obj = m_objGrid->GetElem()) {
        Lunar<Object>::push( L, obj->get() );
        lua_rawseti( L, newTable, index );
        ++index;
      }
    } else {
      while ( const intrObj_type *obj = m_objGrid->GetElem()) {
        if ( ObjectAttorney::GetType( *obj ) == type ) {
          Lunar<Object>::push( L, obj->get() );
          lua_rawseti( L, newTable, index );
          ++index;
        }
      }
    }
    return 1;
  }
  LogLuaErr( "Invalid tile passed to GetObjectsOnTile." );
  return 0;
}


int ObjectManager::LuaObjectBlockingTile( lua_State *L ) const {
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
void ObjectManager::LoadData( 
  const TiXmlElement *root, 
  lua_State *L ) {
  int width = m_mapRect.GetWidth();
  int hight = m_mapRect.GetHeight();
  m_objGrid.reset( new RangeMatrix3D<intrObj_type>( width, height ));

  const TiXmlElement *objType = root->FirstChildElement( "object" );
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
            if ( m_mapRect.Contains( x, y ) && strip >= 0 ) {
              const intrObj_type obj(
                ObjectAttorney::Create( path, x, y , strip, L ));
              AddObject( obj );
            } else {
              LogErr( 
                "Tile out of range or strip negative for Object in state file."
              );
              break;
            }
          } while ( (instance = instance->NextSiblingElement( "inst" )) );
        } else {
          LogErr( "No instances specified for Object in state file." );
          break;
        }
      } else {
        LogErr( "No path specified for Object in state file." );
        break;
      }
    } while ( (objType = objType->NextSiblingElement( "object" )) );
  } else {
    LogErr( "No object specified in <objects>. Thus, not necessary." );
  }
}


void ObjectManager::AddObject( const intrObj_type &obj ) {
  m_objTypes.insert( std::make_pair(
    ObjectAttorney::GetType( obj ), obj )); 
  
  const IntRect &tiles = ObjectAttorney::GetTileRange( obj );
  m_objGrid->AddElem( obj, tiles );
}


void ObjectManager::RemoveObject( const intrObj_type &obj ) {
  if ( std::find( m_toBeDestroyed.begin(),
    m_toBeDestroyed.end(), obj ) == m_toBeDestroyed.end() ) {
    m_toBeDestroyed.push_back( obj );
  }
}


Object *ObjectManager::FindObject( const std::string &type ) const {
  std::pair<nameMap_type::const_iterator, 
            nameMap_type::const_iterator> objects =
              m_objTypes.equal_range( type ); 

  if ( objects.first != objects.second ) {
    return ( ((*(objects.first)).second).get() );
  }
  return NULL;
}


Object *ObjectManager::ObjectOnTile( int x, int y ) const {
  m_objGrid->SetRange( x, y, x, y );
  while ( const intrObj_type *obj = m_objGrid->GetElem() ) {
    return obj->get();
  }
  return NULL;
}


Object *ObjectManager::ObjectOnTile( int x, int y, std::string &type ) const {
  m_objGrid->SetRange( x, y, x, y );
  while ( const intrObj_type *obj = m_objGrid->GetElem() ) {
    if ( ObjectAttorney::GetType( obj->get() ) == type ) {
      return obj->get();
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

  std::pair<nameMap_type::const_iterator, 
            nameMap_type::const_iterator> keyRange =  
              m_objTypes.equal_range( type );
  
  for ( nameMap_type::const_iterator itr = keyRange.first; 
        itr != keyRange.second; ++itr ) {

    const intrObj_type obj( (*itr).second );
    if ( obj.get() == exclude ) {
      continue;
    }

    const IntRect &tiles = ObjectAttorney::GetTileRange( obj );
    if ( tiles.Contains( x, y )) {
      return obj.get();
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
      nearestObj = obj.get();
    }
  }

  return nearestObj;
}


void ObjectManager::UpdateCollisions( const intrObj_type &obj, const Camera &cam ) {
  int tileSize = nt::state::GetTileSize();

  const FloatRect &objRect = ObjectAttorney::GetRect( obj );

  IntRect tileRange;
  tileRange.topLeft.x = ( objRect.topLeft.x / tileSize );
  tileRange.bottomRight.x = ( objRect.bottomRight.x / tileSize );
  tileRange.topLeft.y = ( objRect.topLeft.y / tileSize );
  tileRange.bottomRight.y = ( objRect.bottomRight.y / tileSize );
  nt::state::CullTileRect( tileRange );

  m_objGrid->SetRange( tileRange.topLeft.x, tileRange.topLeft.y,
                       tileRange.bottomRight.x, tileRange.bottomRight.y );
  std::set<intrObj_type, CreationCmp> set;
  FillSet( set );

  for ( SetItr itr = set.begin(); itr != set.end(); ++itr ) {
    const intrObj_type colObj( *itr );

    if ( colObj != obj ) {
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
  const IntRect &view = cam.GetAdjustedFocus( xadj, yadj );
  tLx = view.topLeft.x;
  tLy = view.topLeft.y;
  bRx = view.bottomRight.x;
  bRy = view.bottomRight.y;
}

} // namespace nt

