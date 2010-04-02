#include "GameObject.h"

#include <cstdlib>
#include <iostream>

#include "tinyxml.h"

#include "App.h"
#include "Configuration.h"
#include "LevelState.h"
#include "LuaAppFuncts.h"
#include "Utilities.h"

/************************************************
Constant Members
************************************************/
LevelState* GameObject::m_level = 0;

/************************************************
Public Members
************************************************/
const char GameObject::className[] = "GameObject";
Lunar<GameObject>::RegType GameObject::methods[] = {
  { "MoveDir", &GameObject::LuaMoveDir },
  { "SetAnimation", &GameObject::LuaSetAnimation },
  { "IsAnimating", &GameObject::LuaIsAnimating },
  { "GetType", &GameObject::LuaGetType },
  { "GetTileX", &GameObject::LuaGetTileX },
  { "GetTileY", &GameObject::LuaGetTileY },
  { "Reverse", &GameObject::LuaReverse },
  { 0, 0 }
};


/************************************************
Public Methods
************************************************/
GameObject::GameObject( lua_State *L )
 : m_moving( false ),
   m_direction( Up ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_prevState( NULL ),
   m_id( -1 ),
   m_luaState( luaL_newstate() ),
   m_keyRegistry(0),
   m_numKeyEntries(0) {
  m_level = LevelState::GetInstance();

  if( !lua_isstring( L, -1 ) ) {
    luaL_error( L, "Invalid argument for GameObject." );
  }

  std::string filepath( lua_tostring( L, -1 ) );
  if( !( LoadObjectData( filepath ) &&
         LoadCollisionData( filepath ) ) ) {
    lua_close( m_luaState );
    m_luaState = 0;
    throw "Cannon load GameObject XML file";
  }

  InitLua();
}


GameObject::GameObject( const std::string &filepath )
 : m_moving( false ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_prevState( NULL ),
   m_id( -1 ),
   m_luaState( luaL_newstate() ),
   m_keyRegistry(0),
   m_numKeyEntries(0) {
  m_level = LevelState::GetInstance();

  if( !( LoadObjectData( filepath ) &&
         LoadCollisionData( filepath ) ) ) {
    lua_close( m_luaState );
    m_luaState = 0;
    throw "Cannot load GameObject XML file";
  }
  
  InitLua();
}


GameObject::GameObject( 
  const std::string &filepath, 
  Uint tileX, 
  Uint tileY
)
 : m_moving( false ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_prevState( NULL ),
   m_id( -1 ),
   m_luaState( luaL_newstate() ),
   m_keyRegistry(0),
   m_numKeyEntries(0) {
  m_level = LevelState::GetInstance();

  if( !LoadObjectData( filepath ) ) {
    lua_close( m_luaState );
    m_luaState = 0;
    throw "Cannot load GameObject XML file";
  }

  //Calculate the float positions given tileX and tileY
  //Taking into account tilemap's offset location, tile size, and
  //max tiles across/down
  float x =
  static_cast<float>( 
    Configuration::GetXPad() +         //map's offset X location
    Configuration::GetTileSize() *     //tile size in pixels
    ( tileX % Configuration::GetMapSize() )//number of tiles across tilemap
  );
  float y =
  static_cast<float>( 
    Configuration::GetYPad() +         //map's offset Y location
    Configuration::GetTileSize() *     //tile size in pixels
    ( tileY % Configuration::GetMapSize() )//number of tiles down tilemap
  );
  
  //TODO - I need to handle this every time someone calls SetAnimation as well,
  //just so animations with different frame heights will stll be aligned to the 
  //grid

  //Take into account the sprites that are taller than a normal tile
  y -= GetAnimData()->GetFrameHeight( GetAnimation() ) % Configuration::GetTileSize();

  SetPosition( x, y );

  if ( !m_gridCollision ) {
    if( !LoadCollisionData( filepath ) ) {
      lua_close( m_luaState );
      m_luaState = 0;
      throw "Cannot load GameObject XML file";
    }
  }

  InitLua();
}


GameObject::~GameObject() {
  //Check to see if m_luaState not equal to 0
  //because it WILL crash if closing a null pointer
  if( m_luaState ) {
    lua_close( m_luaState );
  }
}


void GameObject::MoveDir( Dir direction ) {
  if( !m_moving ) {
    m_direction = direction;

    sf::Vector2f tileToMoveTo( GetPosition() );
    
    //Take into account the sprites that are taller than a normal tile
    tileToMoveTo.y += 
      GetAnimData()->GetFrameHeight( GetAnimation() ) % Configuration::GetTileSize();

    switch ( m_direction ) {
      case Up: {
        tileToMoveTo.y -= Configuration::GetTileSize();
        break;
      }
      case Down: {
        tileToMoveTo.y += Configuration::GetTileSize();
        break;
      }
      case Left: {
        tileToMoveTo.x -= Configuration::GetTileSize();
        break;
      }
      case Right: {
        tileToMoveTo.x += Configuration::GetTileSize();
        break;
      }
      default: {}
    }

    if ( !m_level->TileIsSolid( tileToMoveTo ) &&
         !m_level->TileHasGridObject( tileToMoveTo ) ) {
      m_moving = true;
    } 
  }
}


void GameObject::UpdateCollision() {
  GameObject* collisionObj = m_level->DetectObjectCollision( this );

  if( collisionObj != NULL ) {
    //Call OnCollision lua function
    lua_getglobal( m_luaState, "HandleCollision" );
    if ( lua_isfunction( m_luaState, -1 )) {
      Lunar<GameObject>::push( m_luaState, this );
      Lunar<GameObject>::push( m_luaState, collisionObj );
      lua_call( m_luaState, 2, 0 );
    } else {
      lua_pop( m_luaState, 1 );
    }
  }
}


void GameObject::UpdateMovement() {
  if( m_moving ) {
    MovementUpdate();
  }
  else {

    //Call AILogic lua function
    lua_getglobal( m_luaState, "AILogic" );
    if ( lua_isfunction( m_luaState, -1 )) {
      Lunar<GameObject>::push( m_luaState, this );
      lua_call( m_luaState, 1, 0 );
    } else {
      lua_pop( m_luaState, 1 );
    }

    //Call HandleUserInput lua function
    static float keyTime = 0.0f;
    static App* app = App::GetApp();

    for( Uint i = 0; i < m_numKeyEntries; ++i ) {
      //Check if key is down
      if( app->GetInput().IsKeyDown( m_keyRegistry[i].first.key ) ) {
        keyTime = app->GetKeyTime( m_keyRegistry[i].first.key );
        //Check if key has been held down long enough
        if( keyTime >= m_keyRegistry[i].first.time ) {
          lua_getglobal( m_luaState, m_keyRegistry[i].second.c_str() );
          if( lua_isfunction( m_luaState, -1 ) ) {
            Lunar<GameObject>::push( m_luaState, this );
            lua_call( m_luaState, 1, 0 );
          } else {
            lua_pop( m_luaState, 1 );
          }
        }
      }
    }
  }
}


void GameObject::UpdateRendering() {
  AnimSprite::Update();
}


void GameObject::SetId( int id ) {
  m_id = id;
}


int GameObject::GetId() const {
  return m_id;
}


const sf::FloatRect &GameObject::GetCollisionBox() const {
  return m_collisionRect;
}


bool GameObject::CollisionIsGridBased() const {
  return m_gridCollision;
}


Uint GameObject::GetTileX() const {
  return (Uint)(this->GetPosition().x -
                      Configuration::GetXPad()) /
                      Configuration::GetTileSize();
}


Uint GameObject::GetTileY() const {
return (Uint)( ( this->GetPosition().y +  
                        GetAnimData()->GetFrameHeight( GetAnimation() )  % 
                        Configuration::GetTileSize() ) - 
                        Configuration::GetYPad() ) / 
                        Configuration::GetTileSize();
}


const std::string& GameObject::GetType() const {
  return m_type;
}


int GameObject::LuaMoveDir( lua_State *L ) {
  if( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid argument for MoveDir." );
  }
  
  int dir = lua_tointeger( L, -1 );

  MoveDir( static_cast<Dir>(dir) );
  return 0;
}


int GameObject::LuaSetAnimation( lua_State *L ) {
  if( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid argument for SetAnimation." );
  }
  
  Uint animation = lua_tointeger( L, -1 );

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


int GameObject::LuaGetTileX( lua_State *L ) {
  lua_pushinteger( L, GetTileX() );
  return 1;
}


int GameObject::LuaGetTileY( lua_State *L ) {
  lua_pushinteger( L, GetTileY() );
  return 1;
}


int GameObject::LuaReverse( lua_State *L ) {
  m_distance = Configuration::GetTileSize() - m_distance;

  switch( m_direction ) {
    case Up: {
      m_direction = Down;
      lua_pushinteger( L, Down );
      break;
    }
    case Down: {
      m_direction = Up;
      lua_pushinteger( L, Up );
      break;
    }
    case Left: {
      m_direction = Right;
      lua_pushinteger( L, Right );
      break;
    }
    case Right: {
      m_direction = Left;
      lua_pushinteger( L, Left );
      break;
    }
    default: {
      return luaL_error( L, "Object has no direction." );
    } 
  }

  m_moving = true;

  return 1;
}


/************************************************
Private Methods
************************************************/
GameObject::KeyTime::KeyTime()
 : key( sf::Key::Count ),
   time( 0.0f ) {
}
GameObject::KeyTime::KeyTime(sf::Key::Code key, float delayTime)
 : key( key ),
   time(delayTime) {
}


void GameObject::InitLua() {
  luaL_openlibs( m_luaState );
  Lunar<GameObject>::Register( m_luaState );
  LuaApp::RegisterLuaAppFuncts( m_luaState );
  LevelState::RegisterLuaLevelFuncts( m_luaState );
  luaL_dofile( m_luaState, m_luaScript.c_str() );
}


void GameObject::MovementUpdate() {
  m_prevState = this;
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

  if( m_distance >= Configuration::GetTileSize() ) {
    m_moving = false;
    CorrectMovement();
    m_distance = 0.0f;
  }
}


void GameObject::CorrectMovement() {
  static float diff = 0.0f;
  //Calculate the amount of distance to move back
  diff = m_distance - Configuration::GetTileSize();

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

  m_collisionRect.Left = GetPosition().x;
  std::string rectXOffset( root->FirstChildElement( "rect_x" )->GetText() );
  m_collisionRect.Left += atoi( rectXOffset.c_str() );

  std::string width( root->FirstChildElement( "rect_width" )->GetText() );
  m_collisionRect.Right = m_collisionRect.Left + atoi( width.c_str() );

  m_collisionRect.Top = GetPosition().y;
  std::string rectYOffset( root->FirstChildElement( "rect_y" )->GetText() );
  m_collisionRect.Top += atoi( rectYOffset.c_str() );

  std::string height( root->FirstChildElement( "rect_height" )->GetText() );
  m_collisionRect.Bottom = m_collisionRect.Top + atoi( height.c_str() );
  
  return true;
}


bool GameObject::LoadObjectData( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );
  
  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlHandle handleDoc( &doc );
  TiXmlElement* root = handleDoc.FirstChildElement( "game_object" ).Element();

  //Load in path to GameObject's spritesheet
  std::string spritePath( root->FirstChildElement( "sprite_path" )->GetText() );

  //Load in path to GameObject's lua script
  m_luaScript =  root->FirstChildElement( "script_path" )->GetText();

  // Load in type of object
  m_type = root->FirstChildElement( "typename" )->GetText();

  // Load in speed of object
  std::string speed( root->FirstChildElement( "speed" )->GetText() );
  m_speed = (float)atof( speed.c_str() );

  // Load in whether to use rectangular collision for this object
  std::string collisionType = root->FirstChildElement( "collision_type" )->GetText();
  m_gridCollision = ( collisionType == "grid" );

  //Load in path to animation's xml
  std::string animPath( root->FirstChildElement( "animation_path" )->GetText() );
  LoadAnimData( animPath );

  App* app= App::GetApp();

  //Load the spritesheet image
  sf::Image& img = app->LoadImageW( spritePath.c_str() );
  //Set all pink pixels to be treated as clear
  img.CreateMaskFromColor( sf::Color( 255, 0, 255 ) );
  //Set the spritesheet image
  SetImage( img );

  //Load Input Data
  TiXmlElement* inputListRoot = root->FirstChildElement( "input_list" );
  
  //if an input_list tag exists :D
  if( inputListRoot ) {
    TiXmlElement* input = 0;
    std::vector< std::pair<KeyTime, std::string> > tempList;
    sf::Key::Code tempKey;

    for ( input = inputListRoot->FirstChildElement( "input" ); 
          input; 
          input = input->NextSiblingElement( "input" ) ) {

      std::string key = input->Attribute( "key" );
      std::string delay = input->Attribute( "delay" );
      std::string function = input->Attribute( "function" );

      if( !KeyManager::InterpretKey( key, tempKey ) ) {
        //Invalid key attribute in an input xml tag
        return false;
      }

      KeyTime tempKeyTime( tempKey, static_cast<float>( atof( delay.c_str() ) ) );
      tempList.push_back( std::make_pair( tempKeyTime, function ) );

    }
    m_numKeyEntries = static_cast<Uint>( tempList.size() );
    m_keyRegistry = new KeyEntry[m_numKeyEntries];

    for( Uint i = 0; i < m_numKeyEntries; ++i ) {
      m_keyRegistry[i] = tempList[i];
      App::GetApp()->RegisterKey( m_keyRegistry[i].first.key );

      DEBUG_STATEMENT(
        std::cout << m_type << " Input: " << std::endl;
        std::cout << m_keyRegistry[i].first.key << " " 
          << m_keyRegistry[i].first.time;
        std::cout << " " << m_keyRegistry[i].second << std::endl;
      )
    }
  }
  return true;
}