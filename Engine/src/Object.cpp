#include "Object.h"

#include <cmath>

#include "boost/bind/bind.hpp"
extern "C" {
  #include "lualib.h"
}

#include "App.h"
#include "State.h"
#include "TileManager.h"
#include "tinyxml.h"

/**********************************
 * Public
 *********************************/
const char Object::className[] = "Object";

Lunar<Object>::RegType Object::methods[] = {
  { "Move", &Object::LuaMove },
  { "GetAnimation", &Object::LuaGetAnimation },
  { "SetAnimation", &Object::LuaSetAnimation },
  { "SetAnimationReverse", &Object::LuaSetAnimationReverse },
  { "PlayAnimation", &Object::LuaPlayAnimation },
  { "PlayAnimationReverse", &Object::LuaPlayAnimationReverse },
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
  { NULL, NULL }
};


Object::Object( lua_State *L )
 : m_ptrCallScriptFunc( boost::bind( &Object::CallScriptFunc, this, _1 )),
   m_moving( false ),
   m_blockingTile( false ),
   m_noClip( false ),
   m_direction( UP ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ),
   m_tileX( 0 ),
   m_tileY( 0 ) {
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
 : m_ptrCallScriptFunc( boost::bind( &Object::CallScriptFunc, this, _1 )),
   m_moving( false ),
   m_blockingTile( false ),
   m_noClip( false ),
   m_direction( UP ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ),
   m_tileX( tileX ),
   m_tileY( tileY ) {
  m_state = App::GetApp()->GetCurrentState();

  if( !LoadObjectData( filepath ) ) {
    LogErr( "Object XML file " + filepath + " didn't load correctly." );
  }

  //Calculate the float positions given tileX and tileY
  //Taking into account tile size, and max tiles across/down
  int tileDim = m_state->GetTileManager().GetTileDim();

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
  SetAnimation( strip );

  if( !LoadCollisionData( filepath ) ) {
    LogErr( "Object XML file " + filepath + " didn't load correctly." );
  }

  InitLua();
}


void Object::Init() {
  lua_State *L = App::GetApp()->GetLuaState();
  lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
  lua_getfield( L, -1, "Init" );
  if ( lua_isfunction( L, -1 )) {
    if ( lua_isfunction( L, -1 )) {
      Lunar<Object>::push( L, this );
      lua_call( L, 1, 0 );
    }
    lua_settop( L, 0 );
  } 
}


void Object::HandleEvents() {
  if ( !m_moving ) {
    m_input.ScanInput( m_ptrCallScriptFunc );
  }
}


void Object::UpdateCollision( Object* const collisionObj ) {
  // collisionObj not NULL guaranteed by ObjectManager
  m_collidingWith.push_back( collisionObj );

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


void Object::UpdateAI() {
  if( m_moving ) {
    MovementUpdate();
  } else {
    lua_State *L = App::GetApp()->GetLuaState();
    lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
    lua_getfield( L, -1, "AI" );
    if ( lua_isfunction( L, -1 )) {
      Lunar<Object>::push( L, this );
      lua_call( L, 1, 0 );
    }
    lua_settop( L, 0 );
  }
}


void Object::UpdateRendering() {
  AnimSprite::Update();
}


int Object::GetTileX() const {
  return m_tileX;
}


int Object::GetTileY() const {
  return m_tileY;
}


int Object::LuaMove( lua_State *L ) {
  if( !m_moving ) {
    int nextTileX = m_tileX;
    int nextTileY = m_tileY;

    switch ( m_direction ) {
      case UP: {
        nextTileY--;
        break;
      }
      case DOWN: {
        nextTileY++;
        break;
      }
      case LEFT: {
        nextTileX--;
        break;
      }
      case RIGHT: {
        nextTileX++;
        break;
      }
      default: {}
    }

    if ( m_state->GetTileManager().TileOnMap( nextTileX, nextTileY )) {
      if (( m_noClip ) ||
          ( m_state->GetTileManager().TileIsCrossable( 
              nextTileX, nextTileY  ) &&
            !m_state->GetObjectManager().ObjectBlockingTile( 
              nextTileX, nextTileY ))) {
        m_moving = true;
      }
    }
    lua_pushboolean( L, m_moving );
    return 1;
  }
  lua_pushboolean( L, true );
  return 1;
}


int Object::LuaGetAnimation( lua_State *L ) {
  lua_pushinteger( L, GetAnimation());
  return 1;
}


int Object::LuaSetAnimation( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Didn't pass number to SetAnimation in Object: " + m_type );
    return 0;
  }
  int animation = lua_tointeger( L, -1 );
  SetAnimation( animation );
  return 0;
}


int Object::LuaSetAnimationReverse( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( 
      "Didn't pass number to SetAnimationReverse in Object: " + m_type
    );
    return 0;
  }
  int animation = lua_tointeger( L, -1 );
  SetAnimation( animation, true );
  return 0;
}


int Object::LuaPlayAnimation( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Didn't pass number to PlayAnimation in Object: " + m_type );
    return 0;
  }
  int animation = lua_tointeger( L, -1 );
  SetAnimation( animation );
  Play();
  return 0;
}


int Object::LuaPlayAnimationReverse( lua_State *L ) { 
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( 
      "Didn't pass number to PlayAnimationReverse in Object: " + m_type 
    );
    return 0;
  }
  int animation = lua_tointeger( L, -1 );
  SetAnimation( animation, true );
  Play();
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
    sf::FloatRect thisRect = m_collisionRect;
    sf::FloatRect otherRect = other->m_collisionRect;

    float distLR = otherRect.Left - thisRect.Right;
    float distRL = thisRect.Left - otherRect.Right;
    float distTB = otherRect.Top - thisRect.Bottom;
    float distBT = thisRect.Top - otherRect.Bottom;

    bool intersectingX = 
      ( fabs( distRL ) < ( thisRect.GetWidth() + otherRect.GetWidth() ));
    bool intersectingY = 
      ( fabs( distBT ) < ( thisRect.GetHeight() + otherRect.GetHeight() ));
    if ( intersectingX && intersectingY ) {
      lua_pushboolean ( L, true );
      return 1;
    }
    
    sf::Vector2f velVec = GetVelocityVector() - other->GetVelocityVector();
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
  lua_pushinteger( L, m_tileX );
  lua_pushinteger( L, m_tileY );
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
      m_distance = m_state->GetTileManager().GetTileDim() - m_distance;
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


bool Object::LoadCollisionData( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );

  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlHandle handleDoc( &doc );
  TiXmlElement *root = handleDoc.FirstChildElement( "object" ).Element();

  TiXmlElement *rect = root->FirstChildElement( "rect" );
  if ( rect ) {
    rect->QueryFloatAttribute( "x", &m_collisionRect.Left );
    m_collisionRect.Left += GetPosition().x;

    rect->QueryFloatAttribute( "y", &m_collisionRect.Top );
    m_collisionRect.Top += GetPosition().y;

    rect->QueryFloatAttribute( "width", &m_collisionRect.Right );
    m_collisionRect.Right += m_collisionRect.Left;

    rect->QueryFloatAttribute( "height", &m_collisionRect.Bottom );
    m_collisionRect.Bottom += m_collisionRect.Top;
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
  TiXmlDocument doc ( filepath.c_str() );

  if ( !doc.LoadFile() ) {
    LogErr( "Unable to load Object file: " + filepath );
    return false;
  }

  m_type = GetXmlFileName( filepath );

  TiXmlHandle handleDoc( &doc );
  TiXmlElement *root = handleDoc.FirstChildElement( "object" ).Element();

  TiXmlElement *animation = root->FirstChildElement( "animation" );
  if( animation ) {
    const char *animPath = animation->Attribute( "path" );
    if ( animPath ) {
      LoadAnimData( animPath );
    } else {
      LogErr( "No animation path specified in Object: " + filepath );
      return false;
    }
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
  int halfTile = m_state->GetTileManager().GetTileDim() / 2;
  float prevDist = m_distance;
  float distThisFrame = m_speed * App::GetApp()->GetDeltaTime();
  m_distance += distThisFrame;
  
  bool nextTile = ( prevDist < halfTile && m_distance >= halfTile );

  switch( m_direction ) {
    case UP: {
      Move( 0.0f, -distThisFrame );
      m_collisionRect.Offset( 0.0f, -distThisFrame );
      if ( nextTile ) {
        --m_tileY;
      }
      break;
    }
    case DOWN: {
      Move( 0.0f, distThisFrame );
      m_collisionRect.Offset( 0.0f, distThisFrame );
      if ( nextTile ) {
        ++m_tileY;
      }
      break;
    }
    case LEFT: {
      Move( -distThisFrame, 0.0f );
      m_collisionRect.Offset( -distThisFrame, 0.0f );
      if ( nextTile ) {
        --m_tileX; 
      }
      break;
    }
    case RIGHT: {
      Move( distThisFrame, 0.0f );
      m_collisionRect.Offset( distThisFrame, 0.0f );
      if ( nextTile ) {
        ++m_tileX;
      }
      break;
    }
    default: {}
  }

  if( m_distance >= m_state->GetTileManager().GetTileDim()) {
    m_moving = false;
    CorrectMovement();
    m_distance = 0.0f;
  }
}


void Object::CorrectMovement() {
  static float diff = 0.0f;
  //Calculate the amount of distance to move back
  diff = m_distance - m_state->GetTileManager().GetTileDim();

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

  SetPosition( round( GetPosition().x ), round( GetPosition().y ) );
  m_collisionRect.Top = round( m_collisionRect.Top );
  m_collisionRect.Bottom = round( m_collisionRect.Bottom );
  m_collisionRect.Left = round( m_collisionRect.Left );
  m_collisionRect.Right = round( m_collisionRect.Right );
}


void Object::CallScriptFunc( std::string &funcName ) {
  lua_State *L = App::GetApp()->GetLuaState();
  lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
  lua_getfield( L, -1, funcName.c_str() );
  if( lua_isfunction( L, -1 ) ) {
    Lunar<Object>::push( L, this );
    lua_call( L, 1, 0 );
  }
  lua_settop( L, 0 );
}


sf::Vector2f Object::GetVelocityVector() {
  sf::Vector2f velVec( 0.f, 0.f );
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
