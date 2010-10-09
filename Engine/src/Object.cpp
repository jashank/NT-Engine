#include "Object.h"

#include <cmath>
#include <cstring>

#include <boost/bind/bind.hpp>
extern "C" {
  #include "lualib.h"
}
#include <SFML/Graphics/Color.hpp>

#include "App.h"
#include "MapLib.h"
#include "ResourceLib.h"
#include "TileManager.h"
#include "State.h"
#include "tinyxml.h"

/**********************************
 * Public
 *********************************/
const char Object::className[] = "Object";

Lunar<Object>::RegType Object::methods[] = {
  { "Move", &Object::LuaMove },
  { "GetAnim", &Object::LuaGetAnim },
  { "SetAnim", &Object::LuaSetAnim },
  { "PlayAnim", &Object::LuaPlayAnim },
  { "StopAnim", &Object::LuaStopAnim },
  { "PauseAnim", &Object::LuaPauseAnim },
  { "RestartAnim", &Object::LuaRestartAnim },
  { "SetReverseAnim", &Object::LuaSetReverseAnim },
  { "GetFrame", &Object::LuaGetFrame },
  { "IsAnimating", &Object::LuaIsAnimating },
  { "IsMoving", &Object::LuaIsMoving },
  { "OnCollisionCourse", &Object::LuaOnCollisionCourse },
  { "SetNotColliding", &Object::LuaSetNotColliding },
  { "GetType", &Object::LuaGetType },
  { "GetTile", &Object::LuaGetTile },
  { "BlockTile", &Object::LuaBlockTile },
  { "GetDir", &Object::LuaGetDir },
  { "SetDir", &Object::LuaSetDir },
  { "GetTable", &Object::LuaGetTable },
  { "SetNoClip", &Object::LuaSetNoClip },
  { "ResetTimer", &Object::LuaResetTimer },
  { "GetElapsedTime", &Object::LuaGetElapsedTime },
  { "GetSpeed", &Object::LuaGetSpeed },
  { "SetSpeed", &Object::LuaSetSpeed },
  { "SlowDown", &Object::LuaSlowDown },
  { "SpeedUp", &Object::LuaSpeedUp },
  { "Print", &Object::LuaPrint },
  { "ClearText", &Object::LuaClearText },
  { "SetText", &Object::LuaSetText },
  { "SetTextFont", &Object::LuaSetTextFont },
  { "SetTextSize", &Object::LuaSetTextSize },
  { "SetTextColor", &Object::LuaSetTextColor },
  { "SetTextRotation", &Object::LuaSetTextRotation },
  { "ScaleText", &Object::LuaScaleText },
  { "SetTextPos", &Object::LuaSetTextPos },
  { "SetPrintTime", &Object::LuaSetPrintTime },
  { NULL, NULL }
};


Object::Object( lua_State *L )
 : m_moving( false ),
   m_blockingTile( false ),
   m_noClip( false ),
   m_ptrCallScriptFunc( boost::bind( &Object::CallScriptFunc, this, _1 )),
   m_direction( UP ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( LUA_NOREF ) {
  m_state = App::GetApp()->GetCurrentState();

  if( !lua_isstring( L, -1 ) ) {
    luaL_error( L, "Invalid argument for Object." );
    LogErr( "State not passed to Object." );
  }

  std::string filepath( lua_tostring( L, -1 ) );
  if( !( LoadObjectData( filepath ) &&
         LoadCollisionData( filepath ) ) ) {
    LogErr( "Object XML file " + filepath + " didn't load correctly." );
  }

  InitLua();
}


Object::~Object() {
  lua_State *L = App::GetApp()->GetLuaState();
  if ( L ) {
    luaL_unref( L, LUA_REGISTRYINDEX, m_id );
  }
}

/***********************************************
 * Private
 **********************************************/
State* Object::m_state = NULL;

Object::Object(
  const std::string &filepath,
  int tileX,
  int tileY,
  int strip
)
 : m_moving( false ),
   m_blockingTile( false ),
   m_noClip( false ),
   m_ptrCallScriptFunc( boost::bind( &Object::CallScriptFunc, this, _1 )),
   m_direction( UP ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( LUA_NOREF ),
   m_coords( tileX, tileY ) {
  m_state = App::GetApp()->GetCurrentState();

  if( !LoadObjectData( filepath ) ) {
    LogErr( "Object XML file " + filepath + " didn't load correctly." );
  }

  SetAnimation( strip );

  //Calculate the float positions given tileX and tileY
  //Taking into account tile size, and max tiles across/down
  int tileDim = nt::map::GetTileSize();

  float x = static_cast<float>( tileDim * tileX );
  float y = static_cast<float>( tileDim * tileY ); 

  if( GetAnimData() ) {
    int height = GetAnimData()->GetFrameHeight( GetAnimation() );
    if ( height > tileDim ) {
      //Take into account the sprites that are taller than a normal tile
      y -= GetAnimData()->GetFrameHeight( GetAnimation() ) % tileDim;
    }
  }
  SetPosition( x, y );

  if( !LoadCollisionData( filepath ) ) {
    LogErr( "Object XML file " + filepath + " didn't load correctly." );
  }

  InitLua();
}


void Object::Init() {
  CallScriptFunc( "Init" );
}


void Object::HandleEvents() {
  if ( !m_moving ) {
    m_input.ScanKeys( m_ptrCallScriptFunc );
    m_input.ScanMouse( m_ptrCallScriptFunc, m_collisionRect );
  }
}


void Object::UpdateCollision( Object* const collisionObj ) {
  // collisionObj not NULL guaranteed by ObjectManager
  m_collidingWith.push_back( collisionObj );

  if ( m_id != LUA_NOREF ) {
    lua_State *L = App::GetApp()->GetLuaState();
    lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
    lua_getfield( L, -1, "HandleCollision" );
    if ( lua_isfunction( L, -1 ) ) {
      Lunar<Object>::push( L, this );
      Lunar<Object>::push( L, collisionObj );
      lua_call( L, 2, 0 );
    }
    lua_settop( L, 0 );
  }
}


void Object::UpdateAI() {
  if( m_moving ) {
    MovementUpdate();
  } else {
    CallScriptFunc( "AI" );
  }
}


void Object::UpdateRendering() {
  AnimSprite::Update();
  m_text.UpdatePrint();
}

/********************
 * Lua API
 ********************/
int Object::LuaMove( lua_State *L ) {
  if( !m_moving ) {
    nt::core::IntVec nextCoords = m_coords;

    switch ( m_direction ) {
      case UP: {
        --nextCoords.y;
        break;
      }
      case DOWN: {
        ++nextCoords.y;
        break;
      }
      case LEFT: {
        --nextCoords.x;
        break;
      }
      case RIGHT: {
        ++nextCoords.x;
        break;
      }
      default: {}
    }

    // Need to check if tile is on map because of no clip.
    if ( nt::map::InRange( nextCoords.x, nextCoords.y )) {
      if (( m_noClip ) ||
          ( m_state->GetTileManager().TileIsCrossable( 
              nextCoords.x, nextCoords.y  ) &&
            !m_state->GetObjectManager().ObjectBlockingTile( 
              nextCoords.x, nextCoords.y ))) {
        m_moving = true;
      }
    }
    lua_pushboolean( L, m_moving );
    return 1;
  }
  lua_pushboolean( L, true );
  return 1;
}


int Object::LuaGetAnim( lua_State *L ) {
  lua_pushinteger( L, GetAnimation());
  return 1;
}


int Object::LuaSetAnim( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Didn't pass number to SetAnimation in Object: " + m_type );
    return 0;
  }
  SetAnimation( lua_tointeger( L, -1 ));
  return 0;
}


int Object::LuaPlayAnim( lua_State *L ) {
  Play();
  return 0;
}


int Object::LuaStopAnim( lua_State *L ) {
  Stop();
  return 0;
}


int Object::LuaPauseAnim( lua_State *L ) {
  Pause();
  return 0;
}


int Object::LuaRestartAnim( lua_State *L ) {
  Restart();
  return 0;
}


int Object::LuaSetReverseAnim( lua_State *L ) {
  if ( lua_gettop( L ) == 0 ) {
    LogLuaErr( "Didn't pass anything to SetReverseAnim in Object: " + m_type );
    return 0;
  }
  SetReverse( lua_toboolean( L, -1 ));
  return 0;
}


int Object::LuaGetFrame( lua_State *L ) {
  lua_pushinteger( L, GetFrame());
  return 1;
}


int Object::LuaIsAnimating( lua_State *L ) {
  lua_pushboolean( L, IsAnimating() );
  return 1;
}


int Object::LuaIsMoving( lua_State *L ) {
  lua_pushboolean( L, m_moving );
  return 1;
}


int Object::LuaOnCollisionCourse( lua_State *L ) {
  Object* const other = Lunar<Object>::check( L, 1 );
  if ( other ) {
    nt::core::FloatRect thisRect = m_collisionRect;
    nt::core::FloatRect otherRect = other->m_collisionRect;

    float distLR = otherRect.topLeft.x - thisRect.bottomRight.x;
    float distRL = thisRect.topLeft.x - otherRect.bottomRight.x;
    float distTB = otherRect.topLeft.y - thisRect.bottomRight.y;
    float distBT = thisRect.topLeft.y - otherRect.bottomRight.y;

    bool intersectingX = 
      ( fabs( distRL ) < ( thisRect.GetWidth() + otherRect.GetWidth() ));
    bool intersectingY = 
      ( fabs( distBT ) < ( thisRect.GetHeight() + otherRect.GetHeight() ));
    if ( intersectingX && intersectingY ) {
      lua_pushboolean ( L, true );
      return 1;
    }
    
    nt::core::FloatVec velVec = GetVelocityVector() - other->GetVelocityVector();
    float timeLR = 0.f;
    float timeRL = 0.f;
    float timeTB = 0.f;
    float timeBT = 0.f;

    // Can use direct comparison b/c unmoving velVec assigned to 0 exactly
    if ( velVec.x != 0.f ) {
      timeLR = distLR / velVec.x;
      timeRL = -distRL / velVec.x;
    } else if ( !intersectingX ) {
      lua_pushboolean( L, false );
      return 1;
    }

    if ( velVec.y != 0.f ) {
      timeTB = distTB / velVec.y;
      timeBT = -distBT / velVec.y;
    } else if ( !intersectingY ) {
      lua_pushboolean( L, false );
      return 1;
    }

    float timeXMin = std::min( timeLR, timeRL );
    float timeXMax = std::max( timeLR, timeRL );
    float timeYMin = std::min( timeTB, timeBT );
    float timeYMax = std::max( timeTB, timeBT );
    if ( timeXMin == 0.f && timeXMax == 0.f && intersectingX ) {
      timeXMin = timeYMin;
      timeXMax = timeYMax;
    }
    if ( timeYMin == 0.f && timeYMax == 0.f && intersectingY ) {
      timeYMin = timeXMin;
      timeYMax = timeXMax;
    }

    bool willCollide = ( !( timeXMin > timeYMax ) && !( timeXMax < timeYMin )); 
    lua_pushboolean( L, willCollide );
    return 1;
        
  } else {
    LogLuaErr( "No Object passed to OnCollisionCourse in Object: " + m_type );
    return 0;
  }
}


int Object::LuaSetNotColliding( lua_State *L ) {
  Object* const other = Lunar<Object>::check( L, 1 );
  if ( other ) {
    m_collidingWith.remove( other );
    return 0;
  } else {
    LogLuaErr( "No Object passed to SetNotColliding in Object: " + m_type );
    return 0;
  }
}


int Object::LuaGetType( lua_State *L ) {
  lua_pushstring( L, m_type.c_str() );
  return 1;
}


int Object::LuaGetTile( lua_State *L ) {
  lua_pushinteger( L, m_coords.x );
  lua_pushinteger( L, m_coords.y );
  return 2;
}


int Object::LuaBlockTile( lua_State *L ) {
  m_blockingTile = lua_toboolean( L, -1 );
  return 0;
}


int Object::LuaGetDir( lua_State *L ) {
  lua_pushinteger( L, m_direction );
  return 1;
}


int Object::LuaSetDir( lua_State *L ) {
  if( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Didn't pass number to SetDir in Object: " + m_type );
    return 0;
  }

  Dir dir = static_cast<Dir>( lua_tointeger( L, -1 ) );
  if ( !m_moving ) {
    m_direction = dir;
  } else {
    if ( dir == GetOppositeDir( m_direction )) {
      m_distance = nt::map::GetTileSize() - m_distance;
      m_direction = dir;
    } else {
      LogLuaErr( "Direction passed to SetDir will unalign Object: " + m_type );
      return 0;
    }
  }  
  lua_pushinteger( L, m_direction );
  return 1;
}


int Object::LuaGetTable( lua_State *L ) {
  lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
  return 1;
}


int Object::LuaSetNoClip( lua_State *L ) {
  if ( lua_gettop( L ) == 0 ) {
    LogLuaErr( "Nothing passed to SetNoClip in Object: " + m_type );
    return 0;
  }
  m_noClip = lua_toboolean( L, -1 );
  return 0;
}


int Object::LuaResetTimer( lua_State *L ) {
  m_timer.Reset();
  return 0;
}


int Object::LuaGetElapsedTime( lua_State *L ) {
  lua_pushnumber( L, m_timer.GetElapsedTime() );
  return 1;
}


int Object::LuaGetSpeed( lua_State *L ) {
  lua_pushnumber( L, m_speed );
  return 1;
}


int Object::LuaSetSpeed( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SetSpeed for Object: " + m_type );
    return 0;
  }
  m_speed = lua_tonumber( L, -1 );
  return 0;
}


int Object::LuaSlowDown( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SlowDown for Object: " + m_type );
    return 0;
  }
  m_speed -= lua_tonumber( L, -1 );

  if ( m_speed < 0.f ) {
    m_speed = 0.f;
  }
  return 0;
}


int Object::LuaSpeedUp( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SpeedUp for Object: " + m_type );
    return 0;
  }
  m_speed += lua_tonumber( L, -1 );
  return 0;
}


int Object::LuaPrint( lua_State *L ) {
  m_text.StartPrint();  
  return 0;
}


int Object::LuaSetText( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to SetText for Object: " + m_type );
    return 0;
  } 
  m_text.BufferText( lua_tostring( L, -1 ));
  m_text.SetPosition( GetPosition().x, GetPosition().y );
  return 0;
}


int Object::LuaClearText( lua_State *L ) {
  m_text.SetText( "" );
  return 0;
}  


int Object::LuaSetTextFont( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to SetTextFont for Object: " + m_type );
    return 0;
  }
  sf::Font *font =  nt::rsrc::LoadFont( lua_tostring( L, -1 ));

  if ( font ) {
    m_text.SetFont( *font );
  } 
  return 0;
}


int Object::LuaSetTextSize( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SetTextSize for Object: " + m_type );
    return 0;
  }
  m_text.SetSize( lua_tonumber( L, -1 ));
  return 0;
}


int Object::LuaSetTextColor( lua_State *L ) {
  if ( !lua_isnumber( L, -3 ) || !lua_isnumber( L, -2 ) ||
       !lua_isnumber( L, -1 )) {
    LogLuaErr( 
      "Number not passed to one of 'rgb' values in SetTextColor for Object: " + 
      m_type
    );
    return 0;
  }
  int r = lua_tonumber( L, -3 );
  int g = lua_tonumber( L, -2 );
  int b = lua_tonumber( L, -1 );
  sf::Color color( r, g, b );

  m_text.SetColor( color );
  return 0;
}


int Object::LuaSetTextRotation( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SetTextRotation for Object: " + m_type );
    return 0;
  }
  m_text.SetRotation( lua_tonumber( L, -1 ));
  return 0;
}


int Object::LuaScaleText( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) || !lua_isnumber( L, -1 )) {
    LogLuaErr( "Numbers not passed to ScaleText for Object: " + m_type );
    return 0;
  }
  m_text.SetScale( lua_tonumber( L, -2 ), lua_tonumber( L, -1 ));
  return 0;
}


int Object::LuaSetTextPos( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) || !lua_isnumber( L, -1 )) {
    LogLuaErr( "Numbers not passed SetTextPos for Object: " + m_type );
    return 0;
  }
  float offsetX = lua_tonumber( L, -2 );
  float offsetY = lua_tonumber( L, -1 );

  m_text.SetPosition( GetPosition().x + offsetX, GetPosition().y + offsetY );
  return 0;
}


int Object::LuaSetPrintTime( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SetPrintTime for Object: " + m_type );
    return 0;
  }
  m_text.SetPrintTime( lua_tonumber( L, -1 ));
  return 0;
}


bool Object::LoadCollisionData( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );

  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlHandle handleDoc( &doc );
  TiXmlElement *root = handleDoc.FirstChildElement( "object" ).Element();

  TiXmlElement *rect = root->FirstChildElement( "rect" );
  if ( rect ) {
    rect->QueryFloatAttribute( "x", &m_collisionRect.topLeft.x );
    m_collisionRect.topLeft.x += GetPosition().x;

    rect->QueryFloatAttribute( "y", &m_collisionRect.topLeft.y );
    m_collisionRect.topLeft.y += GetPosition().y;
    
    float width = 0;
    float height = 0;
    rect->QueryFloatAttribute( "width", &width );
    rect->QueryFloatAttribute( "height", &height );

    m_collisionRect.Scale( width, height );
  }

  TiXmlElement *tile = root->FirstChildElement( "tile" );
  if ( tile ) {
    const char *blockingTile = tile->Attribute( "block" );
    if ( blockingTile ) {
      m_blockingTile = ( ToLowerCase( blockingTile ) == "true" );
    } else {
      LogErr( "No 'block' attribute specified for tile element in Object: " +
              filepath );
      return false;
    }
  }

  return true;
}


bool Object::LoadObjectData( const std::string &filepath ) {
  TiXmlDocument doc( filepath.c_str() );

  if ( !doc.LoadFile() ) {
    LogErr( "Unable to load Object file: " + filepath );
    return false;
  }

  m_type = GetFileName( filepath );

  TiXmlHandle handleDoc( &doc );
  TiXmlElement *root = handleDoc.FirstChildElement( "object" ).Element();

  TiXmlElement *animation = root->FirstChildElement( "animation" );
  const char *animPath = animation->Attribute( "path" );
  if ( strcmp( animPath, "" ) != 0 ) {
    LoadAnimData( animPath );
  }

  TiXmlElement *script = root->FirstChildElement( "script" );
  if ( script ) {
    const char *scriptPath = script->Attribute( "path" );
    if ( scriptPath ) {
      m_luaScript = scriptPath;
    } else {
      LogErr( "No script path specified in Object: " + filepath );
      return false;
    }
  }

  TiXmlElement *speed = root->FirstChildElement( "speed" );
  if ( speed ) {
    speed->QueryFloatAttribute( "pps", &m_speed );
  }

  //Load input data if there is any
  const TiXmlElement *inputListRoot = root->FirstChildElement( "input_list" );
  if ( inputListRoot ) {
    if ( !m_input.LoadInputList( inputListRoot )) {
      LogErr( "Problem loading input list in Object: " + m_type );
      return false;
    }
  }

  return true;
}


void Object::InitLua() {
  lua_State *L = App::GetApp()->GetLuaState();
  luaL_dofile( L, m_luaScript.c_str() );
  if ( lua_istable( L, -1 )) {
    m_id = luaL_ref( L, LUA_REGISTRYINDEX );
  } 
}


void Object::MovementUpdate() {
  int tileSize = nt::map::GetTileSize();

  int halfTile = tileSize / 2;
  float prevDist = m_distance;
  float distThisFrame = m_speed * App::GetApp()->GetDeltaTime();
  m_distance += distThisFrame;
  
  bool nextTile = ( prevDist < halfTile && m_distance >= halfTile );

  switch( m_direction ) {
    case UP: {
      Move( 0.0f, -distThisFrame );
      m_collisionRect.Offset( 0.0f, -distThisFrame );
      if ( nextTile ) {
        --m_coords.y;
      }
      break;
    }
    case DOWN: {
      Move( 0.0f, distThisFrame );
      m_collisionRect.Offset( 0.0f, distThisFrame );
      if ( nextTile ) {
        ++m_coords.y;
      }
      break;
    }
    case LEFT: {
      Move( -distThisFrame, 0.0f );
      m_collisionRect.Offset( -distThisFrame, 0.0f );
      if ( nextTile ) {
        --m_coords.x;
      }
      break;
    }
    case RIGHT: {
      Move( distThisFrame, 0.0f );
      m_collisionRect.Offset( distThisFrame, 0.0f );
      if ( nextTile ) {
        ++m_coords.x;
      }
      break;
    }
    default: {}
  }

  if( m_distance >= tileSize ) {
    m_moving = false;
    Realign();
    m_distance = 0.0f;
  }
}


void Object::Realign() {
  static float diff = 0.0f;
  //Calculate the amount of distance to move back
  diff = m_distance - nt::map::GetTileSize();

  if ( diff > 0.f ) {
    //Find the correct direction to move back
    switch( m_direction ) {
      case UP: {
        Move( 0.0f, diff );
        m_collisionRect.Offset( 0.0f, diff );
        break;
      }
      case DOWN: {
        Move( 0.0f, -diff );
        m_collisionRect.Offset( 0.0f, -diff );
        break;
      }
      case LEFT: {
        Move( diff, 0.0f );
        m_collisionRect.Offset( diff, 0.0f );
        break;
      }
      case RIGHT: {
        Move( -diff, 0.0f );
        m_collisionRect.Offset( -diff, 0.0f );
        break;
      }
      default: {}
    }
  }

  SetPosition( round( GetPosition().x ), round( GetPosition().y ) );
  m_collisionRect.topLeft.x = round( m_collisionRect.topLeft.x );
  m_collisionRect.topLeft.y = round( m_collisionRect.topLeft.y );
  m_collisionRect.bottomRight.x = round( m_collisionRect.bottomRight.x );
  m_collisionRect.bottomRight.y = round( m_collisionRect.bottomRight.y );
}


void Object::CallScriptFunc( std::string funcName ) {
  if ( m_id != LUA_NOREF ) {
    lua_State *L = App::GetApp()->GetLuaState();
    lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
    lua_getfield( L, -1, funcName.c_str() );
    if( lua_isfunction( L, -1 ) ) {
      Lunar<Object>::push( L, this );
      lua_call( L, 1, 0 );
    }
    lua_settop( L, 0 );
  }
}


nt::core::FloatVec Object::GetVelocityVector() {
  nt::core::FloatVec velVec( 0.f, 0.f );
  if ( m_moving ) {
    switch ( m_direction ) {
      case UP: {
        velVec.y = -m_speed;
        break;
      }
      case DOWN: {
        velVec.y = m_speed;
        break;
      }
      case LEFT: {
        velVec.x = -m_speed;
        break;
      }
      case RIGHT: {
        velVec.x = m_speed;
        break;
      }
      default:; // Object should have direction, just return 0 vel
    }
  }
  return velVec;
}


Object::Dir Object::GetOppositeDir( Dir dir ) {
  switch ( dir ) {
    case UP: {
      return DOWN;
      break;
    }
    case DOWN: {
      return UP;
      break;
    }
    case LEFT: {
      return RIGHT;
      break;
    }
    case RIGHT: {
      return LEFT;
    }
    default: {
      return UP;
    }
  }
}

