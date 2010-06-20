#include "GameObject.h"

#include <algorithm>
#include <cctype>
#include <iostream>

extern "C" {
#include "lualib.h"
}

#include "App.h"
#include "CollisionManager.h"
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
Public Methods
************************************************/
GameObject::GameObject( lua_State *L )
 : m_moving( false ),
   m_gridCollision( false ),
   m_noClip( false ),
   m_direction( Up ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ),
   m_keyRegistry(0),
   m_numKeyEntries(0) {
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


GameObject::GameObject( const std::string &filepath )
 : m_moving( false ),
   m_gridCollision( false ),
   m_noClip( false ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ),
   m_keyRegistry(0),
   m_numKeyEntries(0) {
  m_gameState = App::GetApp()->GetCurrentState();

  if( !( LoadObjectData( filepath ) &&
         LoadCollisionData( filepath ) ) ) {
    LogErr( "GameObject XML file " + filepath + " didn't load correctly." );
  }

  InitLua();
}


GameObject::GameObject(
  const std::string &filepath,
  unsigned int tileX,
  unsigned int tileY
)
 : m_moving( false ),
   m_gridCollision( false ),
   m_noClip( false ),
   m_direction( Up ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ),
   m_keyRegistry(0),
   m_numKeyEntries(0) {
  m_gameState = App::GetApp()->GetCurrentState();

  if( !LoadObjectData( filepath ) ) {
    LogErr( "GameObject XML file " + filepath + " didn't load correctly." );
  }

  //Calculate the float positions given tileX and tileY
  //Taking into account tile size, and max tiles across/down
  int tileDim = m_gameState->GetTileManager()->GetTileDim();

  float x = static_cast<float>( tileDim *
     ( tileX % m_gameState->GetTileManager()->GetMapWidth()));

  float y = static_cast<float>( tileDim *
     ( tileY % m_gameState->GetTileManager()->GetMapHeight()));

  if( GetAnimData() ) {
    //Take into account the sprites that are taller than a normal tile
    y -= GetAnimData()->GetFrameHeight( GetAnimation() ) % tileDim;
  }
  SetPosition( x, y );

  if ( !m_gridCollision ) {
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
    static Key keyTime;
    static App* app = App::GetApp();
    lua_State *L = m_gameState->GetLuaState();

    for( unsigned int i = 0; i < m_numKeyEntries; ++i ) {
      if( app->GetInput().IsKeyDown( m_keyRegistry[i].first.key ) ) {
        keyTime = app->GetKeyTime( m_keyRegistry[i].first.key );

        //Check if key has been held down long enough
        if( keyTime.elapsedTime >= m_keyRegistry[i].first.elapsedTime ) {
          if( m_keyRegistry[i].first.startTime == keyTime.startTime ) {
            continue; // skip because key has already been pressed
          }

          lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
          lua_getfield( L, -1, m_keyRegistry[i].second.c_str() );
          if( lua_isfunction( L, -1 ) ) {
            Lunar<GameObject>::push( L, this );
            lua_call( L, 1, 0 );
          }
          lua_settop( L, 0 );

          //If the key isn't supposed to be repeated
          if( m_keyRegistry[i].first.startTime != -1 ) {
            //Set the key to equal this key presses current startTime
            m_keyRegistry[i].first.startTime = keyTime.startTime;
            //Making sure that this key won't be handled again until
            //keyTime.startTime is changed(a key is re-pressed)
          }
        }
      }
    }
  }
}


void GameObject::UpdateCollision() {
  GameObject *collisionObj =
    m_gameState->GetGameObjectManager()->DetectCollision( this );

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


bool GameObject::HasGridCollision() const {
  return m_gridCollision;
}


unsigned int GameObject::GetTileX() const {
  int tileDim = m_gameState->GetTileManager()->GetTileDim();
  return static_cast<unsigned int>(
    ( GetPosition().x + tileDim / 2 ) / tileDim );
}


unsigned int GameObject::GetTileY() const {
  int tileDim = m_gameState->GetTileManager()->GetTileDim();
  return static_cast<unsigned int>(
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
      unsigned int x = static_cast<unsigned int>( nextTileX );
      unsigned int y = static_cast<unsigned int>( nextTileY );
      if (( m_noClip ) ||
        ( m_gameState->GetCollisionManager()->TileIsCrossable( x, y ) &&
        !m_gameState->TileHasGridObject( x, y ))) {
        m_moving = true;
      }
    }
  }
  return 0;
}



int GameObject::LuaSetAnimation( lua_State *L ) {
  if( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid argument for SetAnimation." );
  }

  unsigned int animation = lua_tointeger( L, -1 );

  SetAnimation( animation );
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
  m_distance = m_gameState->GetTileManager()->GetTileDim() - m_distance;

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

  if( m_distance >= m_gameState->GetTileManager()->GetTileDim()) {
    m_moving = false;
    CorrectMovement();
    m_distance = 0.0f;
  }
}


void GameObject::CorrectMovement() {
  static float diff = 0.0f;
  //Calculate the amount of distance to move back
  diff = m_distance - m_gameState->GetTileManager()->GetTileDim();

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


bool GameObject::LoadCollisionData( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );

  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlHandle handleDoc( &doc );
  TiXmlElement* root = handleDoc.FirstChildElement( "game_object" ).Element();
  TiXmlElement* rect = root->FirstChildElement( "rect" );

  m_collisionRect.Left = GetPosition().x;
  std::string rectXOffset( rect->Attribute( "x" ) );
  m_collisionRect.Left += atoi( rectXOffset.c_str() );

  std::string width( rect->Attribute( "width" ) );
  m_collisionRect.Right = m_collisionRect.Left + atoi( width.c_str() );

  m_collisionRect.Top = GetPosition().y;
  std::string rectYOffset( rect->Attribute( "y" ) );
  m_collisionRect.Top += atoi( rectYOffset.c_str() );

  std::string height( rect->Attribute( "height" ) );
  m_collisionRect.Bottom = m_collisionRect.Top + atoi( height.c_str() );

  std::string gridCollision( root->FirstChildElement( "grid_collision" )->
                                Attribute( "data" ) );
  m_gridCollision = ( gridCollision == "true" );

  return true;
}


bool GameObject::LoadObjectData( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );

  if ( !doc.LoadFile() ) {
    return false;
  }

  // Determine type by the name of the xml file
  size_t lastPeriod = filepath.find_last_of( '.' );
  size_t lastSlash = filepath.find_last_of( '\\' );
  if( lastSlash == std::string::npos ) {
    lastSlash = filepath.find_last_of( '/' );
  }

  if( lastPeriod == std::string::npos ||
      lastSlash == std::string::npos ||
      lastPeriod < lastSlash ) {
    // Improper filepath, unable to determine type
    return false;
  }

  // Grab the filename substring between lastSlash and lastPeriod
  m_type = filepath.substr( lastSlash+1, (lastPeriod-lastSlash)-1 );


  TiXmlHandle handleDoc( &doc );
  TiXmlElement* root = handleDoc.FirstChildElement( "game_object" ).Element();

  //Load in path to animation's xml
  TiXmlElement* tempElem = root->FirstChildElement( "animation" );
  if( tempElem != 0 ) {
    std::string animPath( tempElem->Attribute( "path" ) );
    LoadAnimData( animPath );
  }

  //Load in path to GameObject's lua script
  m_luaScript = root->FirstChildElement( "script" )->Attribute( "path" );

  //// Load in type of object
  //m_type = root->FirstChildElement( "typename" )->Attribute( "data" );

  // Load in speed of object
  std::string speed( root->FirstChildElement( "speed" )->Attribute( "data" ) );
  m_speed = (float)atof( speed.c_str() );

  //Load Input Data
  TiXmlElement* inputListRoot = root->FirstChildElement( "input_list" );

  //if an input_list tag exists :D
  if( inputListRoot ) {
    TiXmlElement* input = 0;
    std::vector< KeyEntry > tempList;
    sf::Key::Code k;

    for ( input = inputListRoot->FirstChildElement( "input" );
          input;
          input = input->NextSiblingElement( "input" ) ) {

      std::string key = input->Attribute( "key" );
      std::string repeat = input->Attribute( "repeat" );
      std::string delay = input->Attribute( "delay" );
      std::string function = input->Attribute( "function" );

      if( !KeyManager::InterpretKey( key, k ) ) {
        //Invalid key attribute in an input xml tag
        return false;
      }

      Key tempKey( k );

      ToLowerCase( repeat );
      tempKey.startTime = ( repeat == "true" ) ? -1.0f : 0.0f;

      tempKey.elapsedTime = static_cast<float>( atof( delay.c_str() ) );
      tempList.push_back( std::make_pair( tempKey, function ) );

    }
    m_numKeyEntries = static_cast<unsigned int>( tempList.size() );
    m_keyRegistry = new KeyEntry[m_numKeyEntries];

    for( unsigned int i = 0; i < m_numKeyEntries; ++i ) {
      m_keyRegistry[i] = tempList[i];
      App::GetApp()->RegisterKey( m_keyRegistry[i].first.key );

      DEBUG_STATEMENT(
        std::cout << m_type << " Input: " << std::endl;
        std::cout << m_keyRegistry[i].first.key << " "
          << m_keyRegistry[i].first.elapsedTime;
        std::cout << " " << m_keyRegistry[i].second << std::endl;
      )
    }
  }
  return true;
}
