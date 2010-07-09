#include "GameObject.h"

#include "boost/bind/bind.hpp"
extern "C" {
#include "lualib.h"
}

#include "App.h"
#include "GameObjectManager.h"
#include "GameState.h"
#include "TileManager.h"
#include "tinyxml.h"

/************************************************
Constant Members
************************************************/
GameState* GameObject::m_gameState = NULL;

/************************************************
Public Members
************************************************/
const char GameObject::className[] = "GameObject";
Lunar<GameObject>::RegType GameObject::methods[] = {
  { "Move", &GameObject::LuaMove },
  { "SetAnimation", &GameObject::LuaSetAnimation },
  { "SetAnimationReverse", &GameObject::LuaSetAnimationReverse },
  { "IsAnimating", &GameObject::LuaIsAnimating },
  { "GetType", &GameObject::LuaGetType },
  { "GetTile", &GameObject::LuaGetTile },
  { "GetDir", &GameObject::LuaGetDir },
  { "SetDir", &GameObject::LuaSetDir },
  { "Reverse", &GameObject::LuaReverse },
  { "GetTable", &GameObject::LuaGetTable },
  { "SetNoClip", &GameObject::LuaSetNoClip },
  { NULL, NULL }
};


/************************************************
 * Constructor and Destructor
************************************************/
GameObject::GameObject( lua_State *L )
 : m_ptrCallScriptFunc( boost::bind( &GameObject::CallScriptFunc, this, _1 )),
   m_moving( false ),
   m_blockingTile( false ),
   m_noClip( false ),
   m_direction( Up ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ) {
  m_gameState = App::GetApp()->GetCurrentState();

  if( !lua_isstring( L, -1 ) ) {
    luaL_error( L, "Invalid argument for GameObject." );
    LogErr( "State not passed to GameObject." );
  }

  std::string filepath( lua_tostring( L, -1 ) );
  if( !( LoadObjectData( filepath ) &&
         LoadCollisionData( filepath ) ) ) {
    LogErr( "GameObject XML file " + filepath + " didn't load correctly." );
  }

  InitLua();
}


GameObject::GameObject(
  const std::string &filepath,
  int tileX,
  int tileY
)
 : m_ptrCallScriptFunc( boost::bind( &GameObject::CallScriptFunc, this, _1 )),
   m_moving( false ),
   m_blockingTile( false ),
   m_noClip( false ),
   m_direction( Up ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ) {
  m_gameState = App::GetApp()->GetCurrentState();

  if( !LoadObjectData( filepath ) ) {
    LogErr( "GameObject XML file " + filepath + " didn't load correctly." );
  }

  //Calculate the float positions given tileX and tileY
  //Taking into account tile size, and max tiles across/down
  int tileDim = m_gameState->GetTileManager().GetTileDim();

  float x = static_cast<float>( tileDim *
     ( tileX % m_gameState->GetTileManager().GetMapWidth()));

  float y = static_cast<float>( tileDim *
     ( tileY % m_gameState->GetTileManager().GetMapHeight()));

  if( GetAnimData() ) {
    //Take into account the sprites that are taller than a normal tile
    y -= GetAnimData()->GetFrameHeight( GetAnimation() ) % tileDim;
  }
  SetPosition( x, y );

  if ( !m_blockingTile ) {
    if( !LoadCollisionData( filepath ) ) {
      LogErr( "GameObject XML file " + filepath + " didn't load correctly." );
    }
  }

  InitLua();
}


GameObject::~GameObject() {
  lua_State *L = m_gameState->GetLuaState();
  if ( L ) {
    luaL_unref( m_gameState->GetLuaState(), LUA_REGISTRYINDEX, m_id );
  }
}


void GameObject::HandleEvents() {
  if ( !m_moving ) {
    m_input.ScanEvents( m_ptrCallScriptFunc );
  }
}


void GameObject::UpdateCollision() {
  GameObject *collisionObj =
    m_gameState->GetGameObjectManager().DetectCollision( this );

  if( collisionObj != NULL ) {
    lua_State *L = m_gameState->GetLuaState();
    lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
    lua_getfield( L, -1, "HandleCollision" );
    if ( lua_isfunction( L, -1 ) ) {
      Lunar<GameObject>::push( L, this );
      Lunar<GameObject>::push( L, collisionObj );
      lua_call( L, 2, 0 );
    }
    lua_settop( L, 0 );
  }
}


void GameObject::UpdateMovement() {
  if( m_moving ) {
    MovementUpdate();
  } else {
    lua_State *L = m_gameState->GetLuaState();
    lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
    lua_getfield( L, -1, "AILogic" );
    if ( lua_isfunction( L, -1 )) {
      Lunar<GameObject>::push( L, this );
      lua_call( L, 1, 0 );
    }
    lua_settop( L, 0 );
  }
}


void GameObject::UpdateRendering() {
  AnimSprite::Update();
}


const sf::FloatRect &GameObject::GetCollisionBox() const {
  return m_collisionRect;
}


bool GameObject::BlockingTile() const {
  return m_blockingTile;
}


int GameObject::GetTileX() const {
  int tileDim = m_gameState->GetTileManager().GetTileDim();
  return static_cast<int>(
    ( GetPosition().x + tileDim / 2 ) / tileDim );
}


int GameObject::GetTileY() const {
  int tileDim = m_gameState->GetTileManager().GetTileDim();
  return static_cast<int>(
    (( GetPosition().y +
       GetSubRect().GetHeight() % tileDim ) +
       tileDim / 2) / tileDim );
}


const std::string& GameObject::GetType() const {
  return m_type;
}


int GameObject::LuaMove( lua_State *L ) {
  if( !m_moving ) {
    int nextTileX = static_cast<int>( GetTileX() );
    int nextTileY = static_cast<int>( GetTileY() );

    switch ( m_direction ) {
      case Up: {
        nextTileY--;
        break;
      }
      case Down: {
        nextTileY++;
        break;
      }
      case Left: {
        nextTileX--;
        break;
      }
      case Right: {
        nextTileX++;
        break;
      }
      default: {}
    }

    if ( nextTileX >= 0 && nextTileY >= 0 ) {
      int x = static_cast<int>( nextTileX );
      int y = static_cast<int>( nextTileY );
      if (( m_noClip ) ||
        ( m_gameState->GetTileManager().TileIsCrossable( x, y ) &&
        !m_gameState->ObjectBlockingTile( x, y ))) {
        m_moving = true;
      }
    }
  }
  return 0;
}


int GameObject::LuaSetAnimation( lua_State *L ) {
  if( !lua_isnumber( L, -1 )) {
    return luaL_error( L, "Invalid argument for SetAnimation." );
  }
  int animation = lua_tointeger( L, -1 );
  SetAnimation( animation );
  return 0;
}


int GameObject::LuaSetAnimationReverse( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    return luaL_error( L, "Invalid argument for SetAnimationReverse." );
  }
  int animation = lua_tointeger( L, -1 );
  SetAnimation( animation, true );
  return 0;
}


int GameObject::LuaIsAnimating( lua_State *L ) {
  lua_pushboolean( L, IsAnimating() );
  return 1;
}


int GameObject::LuaGetType( lua_State *L ) {
  lua_pushstring( L, m_type.c_str() );
  return 1;
}


int GameObject::LuaGetTile( lua_State *L ) {
  lua_pushinteger( L, GetTileX() );
  lua_pushinteger( L, GetTileY() );
  return 2;
}


int GameObject::LuaGetDir( lua_State *L ) {
  lua_pushinteger( L, m_direction );
  return 1;
}


int GameObject::LuaSetDir( lua_State *L ) {
  if( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid argument for SetDir." );
  }

  m_direction = static_cast<Dir>( lua_tointeger( L, -1 ) );
  return 0;
}


int GameObject::LuaReverse( lua_State *L ) {
  m_distance = m_gameState->GetTileManager().GetTileDim() - m_distance;

  switch( m_direction ) {
    case Up: {
      m_direction = Down;
      break;
    }
    case Down: {
      m_direction = Up;
      break;
    }
    case Left: {
      m_direction = Right;
      break;
    }
    case Right: {
      m_direction = Left;
      break;
    }
    default: {
      return luaL_error( L, "Object has no direction." );
    }
  }

  m_moving = true;
  return 0;
}


int GameObject::LuaGetTable( lua_State *L ) {
  lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
  return 1;
}


int GameObject::LuaSetNoClip( lua_State *L ) {
  if ( !lua_isboolean( L, -1 )) {
    return luaL_error( L, "Did not pass boolean to SetNoClip" );
  }
  m_noClip = lua_toboolean( L, -1 ) != 0;
  return 0;
}


/************************************************
Private Methods
************************************************/
void GameObject::InitLua() {
  luaL_dofile( m_gameState->GetLuaState(), m_luaScript.c_str() );
  if ( lua_istable( m_gameState->GetLuaState(), -1 )) {
    m_id = luaL_ref( m_gameState->GetLuaState(), LUA_REGISTRYINDEX );
  }
}


void GameObject::MovementUpdate() {
  m_distance += m_speed;

  switch( m_direction ) {
    case Up: {
      Move( 0.0f, -m_speed );
      m_collisionRect.Offset( 0.0f, -m_speed );
      break;
    }
    case Down: {
      Move( 0.0f, m_speed );
      m_collisionRect.Offset( 0.0f, m_speed );
      break;
    }
    case Left: {
      Move( -m_speed, 0.0f );
      m_collisionRect.Offset( -m_speed, 0.0f );
      break;
    }
    case Right: {
      Move( m_speed, 0.0f );
      m_collisionRect.Offset( m_speed, 0.0f );
      break;
    }
    default: {}
  }

  if( m_distance >= m_gameState->GetTileManager().GetTileDim()) {
    m_moving = false;
    CorrectMovement();
    m_distance = 0.0f;
  }
}


void GameObject::CorrectMovement() {
  static float diff = 0.0f;
  //Calculate the amount of distance to move back
  diff = m_distance - m_gameState->GetTileManager().GetTileDim();

  //Find the correct direction to move back
  switch( m_direction ) {
    case Up: {
      Move( 0.0f, diff );
      m_collisionRect.Offset( 0.0f, diff );
      break;
    }
    case Down: {
      Move( 0.0f, -diff );
      m_collisionRect.Offset( 0.0f, -diff );
      break;
    }
    case Left: {
      Move( diff, 0.0f );
      m_collisionRect.Offset( diff, 0.0f );
      break;
    }
    case Right: {
      Move( -diff, 0.0f );
      m_collisionRect.Offset( - diff, 0.0f );
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


void GameObject::CallScriptFunc( std::string &funcName ) {
  lua_State *L = m_gameState->GetLuaState();
  lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
  lua_getfield( L, -1, funcName.c_str() );
  if( lua_isfunction( L, -1 ) ) {
    Lunar<GameObject>::push( L, this );
    lua_call( L, 1, 0 );
  }
  lua_settop( L, 0 );
}


bool GameObject::LoadCollisionData( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );

  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlHandle handleDoc( &doc );
  TiXmlElement *root = handleDoc.FirstChildElement( "game_object" ).Element();

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

  } else {
    LogErr( "No rect specified for GameObject: " + filepath );
    return false;
  }

  TiXmlElement *tile = root->FirstChildElement( "tile" );
  if ( tile ) {
    const char *blockingTile = tile->Attribute( "block" );
    if ( blockingTile ) {
      m_blockingTile = ( ToLowerCase( blockingTile ) == "true" );
    } else {
      LogErr( "No 'block' attribute specified for tile element in GameObject: " +
              filepath );
      return false;
    }
  } else {
    LogErr( "No 'tile' element specified for GameObject: " + filepath );
    return false;
  }

  return true;
}


bool GameObject::LoadObjectData( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );

  if ( !doc.LoadFile() ) {
    LogErr( "Unable to load GameObject file: " + filepath );
    return false;
  }

  m_type = GetXmlFileName( filepath );

  TiXmlHandle handleDoc( &doc );
  TiXmlElement *root = handleDoc.FirstChildElement( "game_object" ).Element();

  TiXmlElement *animation = root->FirstChildElement( "animation" );
  if( animation ) {
    const char *animPath = animation->Attribute( "path" );
    if ( animPath ) {
      LoadAnimData( animPath );
    } else {
      LogErr( "No animation path specified in GameObject: " + filepath );
      return false;
    }
  } else {
    LogErr( "No animation element in GameObject: " + filepath );
    return false;
  }

  TiXmlElement *script = root->FirstChildElement( "script" );
  if ( script ) {
    const char *scriptPath = script->Attribute( "path" );
    if ( scriptPath ) {
      m_luaScript = scriptPath;
    } else {
      LogErr( "No script path specified in GameObject: " + filepath );
      return false;
    }
  }

  TiXmlElement *speed = root->FirstChildElement( "speed" );
  if ( speed ) {
    speed->QueryFloatAttribute( "ppf", &m_speed );
  } else {
    LogErr( "No speed element specified in GameObject: " + filepath );
    return false;
  }

  //Load input data if there is any
  const TiXmlElement *inputListRoot = root->FirstChildElement( "input_list" );
  if ( inputListRoot ) {
    if ( !m_input.LoadInputList( inputListRoot )) {
      LogErr( "Problem loading input list in GameObject: " + m_type );
      return false;
    }
  }

  return true;
}
