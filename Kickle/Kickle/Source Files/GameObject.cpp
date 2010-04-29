#include "GameObject.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>

#include "tinyxml.h"

extern "C" {
#include "lualib.h"
}

#include "App.h"
#include "Configuration.h"
#include "LevelState.h"
#include "Utilities.h"

/************************************************
Constant Members
************************************************/
LevelState* GameObject::m_level = NULL;

/************************************************
Public Members
************************************************/
const char GameObject::className[] = "GameObject";
Lunar<GameObject>::RegType GameObject::methods[] = {
  { "Move", &GameObject::LuaMove },
  { "SetAnimation", &GameObject::LuaSetAnimation },
  { "IsAnimating", &GameObject::LuaIsAnimating },
  { "GetType", &GameObject::LuaGetType },
  { "GetTilePos", &GameObject::LuaGetTilePos },
  { "GetDir", &GameObject::LuaGetDir },
  { "SetDir", &GameObject::LuaSetDir },
  { "Reverse", &GameObject::LuaReverse },
  { "GetTable", &GameObject::LuaGetTable },
  { NULL, NULL }
};


/************************************************
Public Methods
************************************************/
GameObject::GameObject( lua_State *L )
 : m_moving( false ),
   m_gridCollision( false ),
   m_direction( Up ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ),
   m_keyRegistry(0),
   m_numKeyEntries(0) {
  m_level = LevelState::GetInstance();

  if( !lua_isstring( L, -1 ) ) {
    luaL_error( L, "Invalid argument for GameObject." );
  }

  std::string filepath( lua_tostring( L, -1 ) );
  if( !( LoadObjectData( filepath ) &&
         LoadCollisionData( filepath ) ) ) {
    throw "Cannot load GameObject XML file";
  }

  InitLua();
}


GameObject::GameObject( const std::string &filepath )
 : m_moving( false ),
   m_gridCollision( false ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ),
   m_keyRegistry(0),
   m_numKeyEntries(0) {
  m_level = LevelState::GetInstance();

  if( !( LoadObjectData( filepath ) &&
         LoadCollisionData( filepath ) ) ) {
    throw "Cannot load GameObject XML file";
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
   m_direction( Up ),
   m_distance( 0.0f ),
   m_speed( 0.0f ),
   m_id( -1 ),
   m_keyRegistry(0),
   m_numKeyEntries(0) {
  m_level = LevelState::GetInstance();

  if( !LoadObjectData( filepath ) ) {
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
  

  if( GetAnimData() ) {
    //Take into account the sprites that are taller than a normal tile
    y -= GetAnimData()->GetFrameHeight( GetAnimation() ) % Configuration::GetTileSize();
  }
  SetPosition( x, y );

  if ( !m_gridCollision ) {
    if( !LoadCollisionData( filepath ) ) {
      throw "Cannot load GameObject XML file";
    }
  }

  InitLua();
}


GameObject::~GameObject() {
  lua_State *L = m_level->GetLuaState();
  if ( L ) {
    luaL_unref( m_level->GetLuaState(), LUA_REGISTRYINDEX, m_id );
  }
}


void GameObject::UpdateCollision() {
  GameObject* collisionObj = m_level->DetectObjectCollision( this );

  if( collisionObj != NULL ) {
    //Call OnCollision lua function
    lua_State *L = m_level->GetLuaState();
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
  }
  else {
    lua_State *L = m_level->GetLuaState();
    //Call AILogic lua function
    lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
    lua_getfield( L, -1, "AILogic" );
    if ( lua_isfunction( L, -1 )) {
      Lunar<GameObject>::push( L, this );
      lua_call( L, 1, 0 );
    }
    lua_settop( L, 0 );

    //Call HandleUserInput lua function
    static Key keyTime;
    static App* app = App::GetApp();

    for( unsigned int i = 0; i < m_numKeyEntries; ++i ) {
      //Check if key is down
      if( app->GetInput().IsKeyDown( m_keyRegistry[i].first.key ) ) {
        keyTime = app->GetKeyTime( m_keyRegistry[i].first.key );

        //Check if key has been held down long enough
        if( keyTime.elapsedTime >= m_keyRegistry[i].first.elapsedTime ) {

          //They will only ever be equal if they have already
          //been called once for this particular keypress.
          if( m_keyRegistry[i].first.startTime == keyTime.startTime ) {
            continue; //skip the rest of this iteration
          }
          
          //Once the key is released and repressed 
          //then keyTime.starTime will be a different time

          //Call the lua function
          lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
          lua_getfield( L, -1, m_keyRegistry[i].second.c_str() );
          if( lua_isfunction( L, -1 ) ) {
            Lunar<GameObject>::push( L, this );
            lua_call( L, 1, 0 );
          }
          lua_settop( L, 0 );  

          //If the key isn't supposed to be repeated(ie != -1)
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
  return (unsigned int)(this->GetPosition().x -
                      Configuration::GetXPad() + 
                      Configuration::GetTileSize() / 2) /
                      Configuration::GetTileSize();
}


unsigned int GameObject::GetTileY() const {
return (unsigned int)( ( this->GetPosition().y +  
                        GetSubRect().GetHeight()  % 
                        Configuration::GetTileSize() ) - 
                        Configuration::GetYPad() + 
                        Configuration::GetTileSize() / 2) / 
                        Configuration::GetTileSize();
}


const std::string& GameObject::GetType() const {
  return m_type;
}


int GameObject::LuaMove( lua_State *L ) {
  if( !m_moving ) {
    sf::Vector2f tileToMoveTo( GetPosition() );
    
    //Take into account the sprites that are taller than a normal tile
    tileToMoveTo.y += GetSubRect().GetHeight() % Configuration::GetTileSize();

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


int GameObject::LuaGetTilePos( lua_State *L ) {
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
  m_distance = Configuration::GetTileSize() - m_distance;

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


/************************************************
Private Methods
************************************************/
void GameObject::InitLua() {
  luaL_dofile( m_level->GetLuaState(), m_luaScript.c_str() );
  if ( lua_istable( m_level->GetLuaState(), -1 )) {
    m_id = luaL_ref( m_level->GetLuaState(), LUA_REGISTRYINDEX );
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

  TiXmlHandle handleDoc( &doc );
  TiXmlElement* root = handleDoc.FirstChildElement( "game_object" ).Element();

  //Load in path to GameObject's spritesheet
  std::string spritePath( root->FirstChildElement( "sprite_path" )->
                            Attribute( "data" ) );

  //Load in path to animation's xml
  TiXmlElement* tempElem = root->FirstChildElement( "animation_path" );
  if( tempElem != 0 ) {
    std::string animPath( tempElem->Attribute( "data" ) );
    LoadAnimData( animPath );
  }

  //Load in path to GameObject's lua script
  m_luaScript = root->FirstChildElement( "script_path" )->Attribute( "data" );

  // Load in type of object
  m_type = root->FirstChildElement( "typename" )->Attribute( "data" );

  // Load in speed of object
  std::string speed( root->FirstChildElement( "speed" )->Attribute( "data" ) );
  m_speed = (float)atof( speed.c_str() );

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

      std::transform( repeat.begin(), repeat.end(), repeat.begin(), std::tolower );
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