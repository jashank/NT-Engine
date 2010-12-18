#include "Object.h"

#include <cmath>

#include <boost/bind/bind.hpp>
extern "C" {
  #include "lualib.h"
}

#include "Utilities.h"
#include "Window.h"
#include "tinyxml.h"

namespace nt {

int Object::m_numCreated = 0;
IntRect Object::m_mapRect;
int Object::m_tileSize = 0;
float Object::m_diagTileDist = 0.0;

/**********************************
 * Public
 *********************************/
const char Object::className[] = "Object";

Lunar<Object>::RegType Object::methods[] = {
  { "GetAnim", &Object::LuaGetAnim },
  { "SetAnim", &Object::LuaSetAnim },
  { "PlayAnim", &Object::LuaPlayAnim },
  { "StopAnim", &Object::LuaStopAnim },
  { "PauseAnim", &Object::LuaPauseAnim },
  { "RestartAnim", &Object::LuaRestartAnim },
  { "SetReverseAnim", &Object::LuaSetReverseAnim },
  { "GetFrame", &Object::LuaGetFrame },
  { "IsAnimating", &Object::LuaIsAnimating },
  { "SetAlpha", &Object::LuaSetAlpha },
  { "SetRenderPriority", &Object::LuaSetRenderPriority },
  { "Move", &Object::LuaMove },
  { "IsMoving", &Object::LuaIsMoving },
  { "GetType", &Object::LuaGetType },
  { "GetTile", &Object::LuaGetTile },
  { "GetTileRange", &Object::LuaGetTileRange },
  { "GetDir", &Object::LuaGetDir },
  { "SetDir", &Object::LuaSetDir },
  { "GetTable", &Object::LuaGetTable },
  { "ResetTimer", &Object::LuaResetTimer },
  { "GetElapsedTime", &Object::LuaGetElapsedTime },
  { "GetSpeed", &Object::LuaGetSpeed },
  { "SetSpeed", &Object::LuaSetSpeed },
  { "AdjustSpeed", &Object::LuaAdjustSpeed },
  { "PlaySound", &Object::LuaPlaySound },
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
 : m_creationNum( 0 ),
   m_references( 0 ),
   m_renderPriority( 0 ),
   m_moving( false ),
   m_ptrCallScriptFunc( boost::bind( &Object::CallScriptFunc, this, _1 )),
   m_distance( 0.0f ),
   m_id( LUA_NOREF ),
   m_L( NULL ) {
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
  for ( std::list<Object*>::iterator colObj = m_collidingWith.begin();
        colObj != m_collidingWith.end(); ++colObj ) {
    (*colObj)->m_collidingWith.remove( this );
  }

  if ( m_L ) {
    luaL_unref( m_L, LUA_REGISTRYINDEX, m_id );
  }
}


void Object::LuaRegister( lua_State *L ) {
  Lunar<Object>::Register( L );
}


void Object::SetMapRect( const IntRect &mapRect ) {
  m_mapRect = mapRect;
}


void Object::SetTileSize( int tileSize ) {
  m_tileSize = tileSize;
  m_diagTileDist = sqrt( 2 * ( m_tileSize * m_tileSize ));
}

/***********************************************
 * Private
 **********************************************/
Object::Object(
  const std::string &filepath,
  int tileX,
  int tileY,
  int strip,
  lua_State *L
)
 : 
   m_creationNum( ++m_numCreated ),
   m_references( 0 ),
   m_renderPriority( 0 ),
   m_moving( false ),
   m_ptrCallScriptFunc( boost::bind( &Object::CallScriptFunc, this, _1 )),
   m_distance( 0.0f ),
   m_id( LUA_NOREF ),
   m_L( L ) {
  if( !LoadObjectData( filepath ) ) {
    LogErr( "Object XML file " + filepath + " didn't load correctly." );
  }

  m_sprite.SetAnimation( strip );
  m_sprite.Play();

  float x = static_cast<float>( m_tileSize * tileX );
  float y = static_cast<float>( m_tileSize * tileY ); 

  int height = m_sprite.GetFrameHeight();
  if ( height > m_tileSize ) {
    //Take into account the sprites that are taller than a normal tile
    y -= height - m_tileSize;
  }
  m_sprite.SetStartingPos( x, y );

  if( !LoadCollisionData( filepath ) ) {
    LogErr( "Object XML file " + filepath + " didn't load correctly." );
  }

  AdjustTileRange();
  m_lastTileRange = m_tileRange;

  InitLua();
}


void Object::Init() {
  CallScriptFunc( "Init" );
}


void Object::HandleEvents() {
  m_input.Update();
  if ( !m_moving ) {
    m_input.ScanKeys( m_ptrCallScriptFunc );
    m_input.ScanMouse( m_ptrCallScriptFunc, m_collisionRect );
  }
}


void Object::HandleCollision( Object* const collisionObj ) {
  // collisionObj not NULL guaranteed by ObjectManager
  m_collidingWith.push_back( collisionObj );

  if ( m_id != LUA_NOREF ) {
    // Push HandleCollision from Object table onto stack
    lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_id );
    lua_getfield( m_L, -1, "HandleCollision" );
    if ( lua_isfunction( m_L, -1 ) ) {
      // Push Object's table, the object, and object it is colliding with
      // as arguments to HandleCollision.
      lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_id );
      Lunar<Object>::push( m_L, this );
      Lunar<Object>::push( m_L, collisionObj );
      lua_call( m_L, 3, 0 );
    }
    lua_settop( m_L, 0 );
  }
}


void Object::UpdateAI( float dt ) {
  // Order matters
  m_sprite.Update( dt );
  m_text.UpdatePrint();

  if ( m_moving ) {
    float distThisFrame = m_velVec.magnitude * dt;
    m_distance -= distThisFrame;
    if ( m_distance <= 0.0 ) {
      m_moving = false;
      distThisFrame += m_distance;
    }

    if ( dir::IsCardinal( m_velVec.direction )) {
      MoveCardinally( distThisFrame );
    } else {
      MoveNonCardinally( distThisFrame );
    }

    m_lastTileRange = m_tileRange;
    AdjustTileRange();
  } else {
    CallScriptFunc( "AI" );
  }
}


void Object::Draw( float alpha ) {
  window::Draw( m_sprite, alpha );
  window::Draw( m_text, alpha );
}

/********************
 * Lua API
 ********************/
int Object::LuaGetAnim( lua_State *L ) {
  lua_pushinteger( L, m_sprite.GetAnimation());
  return 1;
}


int Object::LuaSetAnim( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Didn't pass number to SetAnimation in Object: " + m_type );
    return 0;
  }
  m_sprite.SetAnimation( lua_tointeger( L, -1 ));
  return 0;
}


int Object::LuaPlayAnim( lua_State *L ) {
  m_sprite.Play();
  return 0;
}


int Object::LuaStopAnim( lua_State *L ) {
  m_sprite.Stop();
  return 0;
}


int Object::LuaPauseAnim( lua_State *L ) {
  m_sprite.Pause();
  return 0;
}


int Object::LuaRestartAnim( lua_State *L ) {
  m_sprite.Restart();
  return 0;
}


int Object::LuaSetReverseAnim( lua_State *L ) {
  if ( lua_gettop( L ) == 0 ) {
    LogLuaErr( "Didn't pass anything to SetReverseAnim in Object: " + m_type );
    return 0;
  }
  m_sprite.SetReverse( lua_toboolean( L, -1 ));
  return 0;
}


int Object::LuaGetFrame( lua_State *L ) {
  lua_pushinteger( L, m_sprite.GetFrame());
  return 1;
}


int Object::LuaIsAnimating( lua_State *L ) {
  lua_pushboolean( L, m_sprite.IsAnimating() );
  return 1;
}


int Object::LuaSetAlpha( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SetAlpha in Object: " + m_type );
    return 0;
  }
  int alpha = lua_tointeger( L, -1 );
  if ( alpha >= 0 ) {
    m_sprite.SetAlpha( static_cast<unsigned int>( alpha ));
  } else {
    LogLuaErr( "Passed negative integer to SetAlpha in Object: " + m_type );
  }
  return 0;
}


int Object::LuaSetRenderPriority( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SetRenderPriority in Object: " + m_type );
    return 0;
  }
  m_renderPriority = lua_tointeger( L, -1 );
  return 0;
}


int Object::LuaMove( lua_State *L ) {
  if( !m_moving ) {
    IntRect nextRange = m_tileRange;

    int offsetX = 0;
    int offsetY = 0;

    if ( dir::IsNorthward( m_velVec.direction )) {
      offsetY = -1;
    } else if ( dir::IsSouthward( m_velVec.direction )) {
      offsetY = 1;
    }

    if ( dir::IsWestward( m_velVec.direction )) {
      offsetX = -1;
    } else if ( dir::IsEastward( m_velVec.direction )) {
      offsetX = 1;
    }

    nextRange.Offset( offsetX, offsetY );

    int tX = nextRange.topLeft.x;
    int tY = nextRange.topLeft.y;
    int bX = nextRange.bottomRight.x;
    int bY = nextRange.bottomRight.y;

    if ( m_mapRect.Contains( tX, tY ) && m_mapRect.Contains( bX, bY )) {
      m_moving = true;

      if ( dir::IsCardinal( m_velVec.direction )) {
        m_distance = m_tileSize;
      } else {
        m_distance = m_diagTileDist;
      }
    }
  }

  lua_pushboolean( L, m_moving );
  return 1;
}


int Object::LuaIsMoving( lua_State *L ) {
  lua_pushboolean( L, m_moving );
  return 1;
}


int Object::LuaGetType( lua_State *L ) {
  lua_pushstring( L, m_type.c_str() );
  return 1;
}


int Object::LuaGetTile( lua_State *L ) {
  lua_pushinteger( L, m_tileRange.topLeft.x );
  lua_pushinteger( L, m_tileRange.topLeft.y );
  return 2;
}


int Object::LuaGetTileRange( lua_State *L ) {
  lua_pushinteger( L, m_tileRange.topLeft.x );
  lua_pushinteger( L, m_tileRange.topLeft.y );
  lua_pushinteger( L, m_tileRange.bottomRight.x );
  lua_pushinteger( L, m_tileRange.bottomRight.y );
  return 4;
}


int Object::LuaGetDir( lua_State *L ) {
  lua_pushinteger( L, m_velVec.direction );
  return 1;
}


int Object::LuaSetDir( lua_State *L ) {
  if( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Didn't pass number to SetDir in Object: " + m_type );
    return 0;
  }

  dir::Direction dir = static_cast<dir::Direction>( lua_tointeger( L, -1 ) );
  if ( !m_moving ) {
    m_velVec.direction = dir;
  } else {
    if ( dir == dir::GetOpposite( m_velVec.direction )) {

      if ( dir::IsCardinal( dir )) {
        m_distance = m_tileSize - m_distance;
      } else {
        m_distance = m_diagTileDist - m_distance;
      }

      m_velVec.direction = dir;
    } else {
      LogLuaErr( "Direction passed to SetDir will unalign Object: " + m_type );
      return 0;
    }
  }  
  lua_pushinteger( L, m_velVec.direction );
  return 1;
}


int Object::LuaGetTable( lua_State *L ) {
  lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
  return 1;
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
  lua_pushnumber( L, m_velVec.magnitude );
  return 1;
}


int Object::LuaSetSpeed( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SetSpeed for Object: " + m_type );
    return 0;
  }
  m_velVec.magnitude = lua_tonumber( L, -1 );
  return 0;
}


int Object::LuaAdjustSpeed( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to AdjustSpeed for Object: " + m_type );
    return 0;
  }
  m_velVec.magnitude += lua_tonumber( L, -1 );
  if ( m_velVec.magnitude < 0.f ) {
    m_velVec.magnitude = 0.f;
  }
  return 0;
}


int Object::LuaPlaySound( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to PlaySound for Object: " + m_type );
    return 0;
  }

  m_sounds.Play( lua_tostring( L, -1 ));
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
  m_text.SetPosition( m_sprite.GetPosition().x, m_sprite.GetPosition().y );
  return 0;
}


int Object::LuaClearText( lua_State *L ) {
  m_text.BufferText( "" );
  m_text.StartPrint();
  return 0;
}  


int Object::LuaSetTextFont( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to SetTextFont for Object: " + m_type );
    return 0;
  }
  std::string font = lua_tostring( L, -1 );
  m_text.SetFont( font );

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

  float posX = m_sprite.GetPosition().x;
  float posY = m_sprite.GetPosition().y;
  m_text.SetPosition( posX + offsetX, posY + offsetY );
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
    m_collisionRect.topLeft.x += m_sprite.GetPosition().x;

    rect->QueryFloatAttribute( "y", &m_collisionRect.topLeft.y );
    m_collisionRect.topLeft.y += m_sprite.GetPosition().y;
    
    float width = 0;
    float height = 0;
    rect->QueryFloatAttribute( "width", &width );
    rect->QueryFloatAttribute( "height", &height );

    m_collisionRect.Scale( width, height );
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
    m_sprite.LoadAnimData( animPath );
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
    speed->QueryFloatAttribute( "pps", &m_velVec.magnitude );
  }

  TiXmlElement *render = root->FirstChildElement( "render" );
  if ( render ) {
    render->QueryIntAttribute( "priority", &m_renderPriority );
  }

  TiXmlElement *sound = root->FirstChildElement( "sound_effects" );
  if ( sound ) {
    m_sounds.LoadData( sound );
  }

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
  luaL_dofile( m_L, m_luaScript.c_str() );
  if ( lua_istable( m_L, -1 )) {
    m_id = luaL_ref( m_L, LUA_REGISTRYINDEX );
  } 
}


void Object::MoveCardinally( float distance ) {
  float offsetX = 0.0;
  float offsetY = 0.0;

  switch ( m_velVec.direction ) {
    case dir::North: {
      offsetY = -distance;
      break;
    }
    case dir::South: {
      offsetY = distance;
      break;
    }
    case dir::East: {
      offsetX = distance;
      break;
    }
    case dir::West: {
      offsetX = -distance;
      break;
    }
    default: {}
  }

  m_sprite.Move( offsetX, offsetY );
  m_collisionRect.Offset( offsetX, offsetY );
}


void Object::MoveNonCardinally( float distance ) {
  float offsetX = 0.0;
  float offsetY = 0.0;

  float compDist = sqrt(( distance * distance ) / 2.0);

  switch ( m_velVec.direction ) {
    case dir::Northeast: {
      offsetX = compDist;
      offsetY = -compDist;
      break;
    }
    case dir::Southeast: {
      offsetX = offsetY = compDist;
      break;
    }
    case dir::Southwest: {
      offsetX = -compDist;
      offsetY = compDist;
      break;
    }
    case dir::Northwest: {
      offsetX = offsetY = -compDist;
      break;
    }
    default: {}
  }

  m_sprite.Move( offsetX, offsetY );
  m_collisionRect.Offset( offsetX, offsetY );
}


void Object::AdjustTileRange() {
  m_tileRange.topLeft.x = m_collisionRect.topLeft.x / m_tileSize;
  m_tileRange.topLeft.y = m_collisionRect.topLeft.y / m_tileSize;
  m_tileRange.bottomRight.x = m_collisionRect.bottomRight.x / m_tileSize;
  m_tileRange.bottomRight.y = m_collisionRect.bottomRight.y / m_tileSize;
}


void Object::CallScriptFunc( std::string funcName ) {
  if ( m_id != LUA_NOREF ) {
    // Grab function name from Object's table
    lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_id );
    lua_getfield( m_L, -1, funcName.c_str() );
    if( lua_isfunction( m_L, -1 ) ) {
      // Push Object's table and Object to function.
      lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_id );
      Lunar<Object>::push( m_L, this );
      lua_call( m_L, 2, 0 );
    }
    lua_settop( m_L, 0 );
  }
}

} // namespace nt
