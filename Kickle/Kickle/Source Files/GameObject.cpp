#include "GameObject.h"

#include <cstdlib>
#include <iostream>

#include "tinyxml.h"

#include "AnimData.h"
#include "App.h"
#include "Configuration.h"
#include "Level.h"
#include "LuaAppFuncts.h"
#include "PlayState.h"

/************************************************
Constant Members
************************************************/
Level* GameObject::m_level = 0;

/************************************************
Public Members
************************************************/
const char GameObject::className[] = "GameObject";
Lunar<GameObject>::RegType GameObject::methods[] = {
  { "MoveDir", &GameObject::LuaMoveDir },
  { "SetAnimation", &GameObject::LuaSetAnimation },
  { "GetType", &GameObject::LuaGetType },
  { "GetTileX", &GameObject::LuaGetTileX },
  { "GetTileY", &GameObject::LuaGetTileY },
  { 0, 0 }
};

/************************************************
Public Methods
************************************************/
GameObject::GameObject( lua_State *L )
 : m_play( false ),
   m_animation( 0 ),
   m_frameTime( 0.0f ),
   m_distance( 0.0f ),
   m_animData( 0 ),
   m_moving( false ),
   m_id( -1 ),
   m_luaState( luaL_newstate() ),
   m_frame( 0 ) {
  if( !lua_isstring( L, -1 ) ) {
    luaL_error( L, "Invalid argument for GameObject." );
  }

  std::string xmlPath( lua_tostring( L, -1 ) );
  if( !LoadObjectData( xmlPath ) &&
      !LoadCollisionData( xmlPath ) ) {
    lua_close( m_luaState );
    m_luaState = 0;
    throw "Cannon load GameObject XML file";
  }

  InitLua();
}


GameObject::GameObject( const std::string &xmlGameObjectPath )
 : m_play( false ),
   m_animation( 0 ),
   m_frameTime( 0.0f ),
   m_distance( 0.0f ),
   m_animData( 0 ),
   m_moving( false ),
   m_id( -1 ),
   m_luaState( luaL_newstate() ),
   m_frame( 0 ) { 
  if( !LoadObjectData( xmlGameObjectPath ) &&
      !LoadCollisionData( xmlGameObjectPath )) {
    lua_close( m_luaState );
    m_luaState = 0;
    throw "Cannot load GameObject XML file";
  }
  
  InitLua();
}


GameObject::GameObject( 
  const std::string &xmlGameObjectPath, 
  Uint tileX, 
  Uint tileY
)
 : m_play( false ),
   m_animation( 0 ),
   m_frameTime( 0.0f ),
   m_distance( 0.0f ),
   m_animData( 0 ),
   m_moving( false ),
   m_id( -1 ),
   m_luaState( luaL_newstate() ),
   m_frame( 0 ) { 
  if( !LoadObjectData( xmlGameObjectPath ) ) {
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
  y -= m_animData->GetFrameHeight( m_animation ) % Configuration::GetTileSize();

  SetPosition( x, y );

  if( !LoadCollisionData( xmlGameObjectPath ) ) {
    lua_close( m_luaState );
    m_luaState = 0;
    throw "Cannot load GameObject XML file";
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


const AnimData *GameObject::GetAnimData() const {
	  return m_animData;
}


Uint GameObject::GetFrame() const {
	return m_frame;
}


Uint GameObject::GetAnimation() const {
	return m_animation;
}


void GameObject::AssignLevel( Level *level ) {
  m_level = level;
}


void GameObject::MoveDir( Dir direction ) {
  if( !m_moving ) {
    m_direction = direction;

    m_distance = 0.0f;

    sf::Vector2f tileToMoveTo( GetPosition() );
    
    //Take into account the sprites that are taller than a normal tile
    tileToMoveTo.y += 
      m_animData->GetFrameHeight( m_animation ) % Configuration::GetTileSize();

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

    if ( !m_level->IsTileSolid( tileToMoveTo ) &&
         !m_level->TileHasSolidObject( tileToMoveTo ) ) {
      m_moving = true;
    } 
  }
}


void GameObject::Play() {
  if( m_animData ) {
  	m_play = true;
    //Set the GameObject's clip to the correct animation frame
  	SetSubRect( m_animData->GetFrameRect( m_animation, m_frame ) );
  }
}


void GameObject::Pause() {
	m_play = false;
}


void GameObject::Restart() {
  //If GameObject is animated
	if( m_animData ) {
    //Stop animation setting frame to 0
	  Stop();
    //Reset frame time
	  m_frameTime = m_animData->GetFrameTime( m_animation, 0 );
    //Start animation again
	  Start();
  }
}


void GameObject::SetFrame( Uint frame ) {
	m_frame = frame;
}


void GameObject::SetAnimation( Uint animation ) {
		m_animation = animation;
    //m_frameTime = m_animData->GetFrameTime( m_animation );
}


void GameObject::SetAnimData( const AnimData &animData ) {
	m_animData = &animData;
}


void GameObject::Start() {
	Play();
}


void GameObject::Stop() {
	Pause();
  SetFrame( 0 );
}


void GameObject::StopMoving() {
  static sf::Vector2f pos;
  pos = GetPosition();
  if( //aligned perfectly in grid(taking into account tilemap's offset)
    static_cast<int>( pos.x - Configuration::GetXPad() ) % 
                      Configuration::GetTileSize() == 0 &&
    static_cast<int>(
      pos.y + m_animData->GetFrameHeight( m_animation ) - 
      Configuration::GetTileSize() -
      Configuration::GetYPad() ) % 
      Configuration::GetTileSize() == 0 
  ) {
    m_moving = false;
  }
}


void GameObject::Update() {
  AnimUpdate();

  if( m_moving ) {
    MovementUpdate();
  }
  else {
    //Call HandleUserInput lua function
    lua_getglobal( m_luaState, "HandleUserInput" );
    if( lua_isfunction( m_luaState, -1 ) ) {
      Lunar<GameObject>::push( m_luaState, this );
      lua_call( m_luaState, 1, 0 );
    } else {
      lua_pop( m_luaState, 1 );
    }
    
    //Call AILogic lua function
    lua_getglobal( m_luaState, "AILogic" );
    if ( lua_isfunction( m_luaState, -1 )) {
      Lunar<GameObject>::push( m_luaState, this );
      lua_call( m_luaState, 1, 0 );
    } else {
      lua_pop( m_luaState, 1 );
    }
  }

  GameObject* collisionObj = m_level->DetectObjectCollision( this );

  if( collisionObj != NULL ) {
    //DEBUG_STATEMENT(  
    //  std::cout 
    //  << "Object Collision Detected..\n"
    //  << m_type << "\n"
    //  //<< collisionObj->GetType() 
    //  << std::endl;
    //)

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


void GameObject::SetId( int id ) {
  m_id = id;
}
  

int GameObject::GetId() const {
  return m_id;
}


const sf::FloatRect &GameObject::GetCollisionBox() const {
  return m_collisionRect;
}


bool GameObject::IsSolid() const {
  return m_solid;
}


Uint GameObject::GetTileX() const {
  return (Uint)(this->GetPosition().x -
                      Configuration::GetXPad()) /
                      Configuration::GetTileSize();
}
  

Uint GameObject::GetTileY() const {
return (Uint)( ( this->GetPosition().y +  
                        m_animData->GetFrameHeight( m_animation )  % 
                        Configuration::GetTileSize() ) - 
                        Configuration::GetYPad() ) / 
                        Configuration::GetTileSize();
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


/************************************************
Private Methods
************************************************/

void GameObject::AnimUpdate() {
  //If GameObject is set to play
	if( m_play ) {
  
    //Count down the time to the next frame shift
	  m_frameTime -= App::GetApp()->GetDeltaTime();

    //If animation is ready to go to next frame
	  if( m_frameTime <= 0.0f ) {
      //Move on to the next frame
		  NextFrame();
      //Set the GameObject's clip to the correct animation frame
		  SetSubRect( m_animData->GetFrameRect( m_animation, m_frame ) );
      //Reset frame time
		  m_frameTime = m_animData->GetFrameTime( m_animation, m_frame );
	  }
  }
} 


void GameObject::InitLua() {
  luaL_openlibs( m_luaState );
  Lunar<GameObject>::Register( m_luaState );
  LuaApp::RegisterLuaAppFuncts( m_luaState );
  PlayState::RegisterLuaPlayFuncts( m_luaState );
  luaL_dofile( m_luaState, m_luaScript.c_str() );
}


void GameObject::MovementUpdate() {
  static const float SPEED = 1.0f;
  m_distance += SPEED;

  switch( m_direction ) {
  case Up:
    Move( 0.0f, -SPEED );
    m_collisionRect.Offset( 0.f, -SPEED );
    break;
  case Down:
    Move( 0.0f, SPEED );
    m_collisionRect.Offset( 0.f, SPEED );
    break;
  case Left:
    Move( -SPEED, 0.0f );
    m_collisionRect.Offset( -SPEED, 0.f );
    break;
  case Right:
    Move( SPEED, 0.0f );
    m_collisionRect.Offset( SPEED, 0.f );
    break;
  }

  //Corrects movements that exceed the next grid location
  if( m_distance >= Configuration::GetTileSize() ) {
    m_moving = false;

    static float diff = 0.0f;
    //Calculate the amount of distance to move back
    diff = m_distance - Configuration::GetTileSize();

    //Find the correct direction to move back
    switch( m_direction ) {
    case Up:
      Move( 0.0f, diff );
      break;
    case Down:
      Move( 0.0f, -diff );
      break;
    case Left:
      Move( diff, 0.0f );
      break;
    case Right:
      Move( -diff, 0.0f );
      break;
    }
  }
}


void GameObject::NextFrame() {
  //Increment frame
	++m_frame;

  //If the animation has reached it's end
	if( m_frame >= m_animData->GetNumFrames( m_animation ) ) {
    //If animation is looped
		if( m_animData->IsLooped( m_animation ) ) {
      //Reset animation to first frame
			m_frame = 0;
		}
		else {
      //Set animation to last frame
      m_frame = m_animData->GetNumFrames( m_animation )-1;
      //Pause animation at last frame
			Pause();
		}
	}
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

  //Load in path to animation's xml
  std::string animPath( root->FirstChildElement( "animation_path" )->GetText() );

  //Load in path to GameObject's lua script
  m_luaScript =  root->FirstChildElement( "script_path" )->GetText();

  // Load in type of object
  m_type = root->FirstChildElement( "typename" )->GetText();

  // Load in whether to use rectangular collision for this object
  std::string solid = root->FirstChildElement( "solid" )->GetText();
  m_solid = ( solid == "true" ) ? true : false;

  App* app= App::GetApp();

  //Load the spritesheet image
  sf::Image& img = app->LoadImageW( spritePath.c_str() );
  //Set all pink pixels to be treated as clear
  img.CreateMaskFromColor( sf::Color( 255, 0, 255 ) );
  //Set the spritesheet image
  SetImage( img );

  //Load/Set the animation data
  AnimData& anim = app->LoadAnim( animPath.c_str() );
  SetAnimData( anim );

  return true;
}


bool GameObject::LoadCollisionData( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );
  
  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlHandle handleDoc( &doc );
  TiXmlElement* root = handleDoc.FirstChildElement( "game_object" ).Element();

  m_collisionRect.Left = GetPosition().x;
  std::string rectXOffset( root->FirstChildElement( "solid_area_x" )->GetText() );
  m_collisionRect.Left += atoi( rectXOffset.c_str() );

  std::string width( root->FirstChildElement( "solid_area_width" )->GetText() );
  m_collisionRect.Right = m_collisionRect.Left + atoi( width.c_str() );

  m_collisionRect.Top = GetPosition().y;
  std::string rectYOffset( root->FirstChildElement( "solid_area_y" )->GetText() );
  m_collisionRect.Top += atoi( rectYOffset.c_str() );

  std::string height( root->FirstChildElement( "solid_area_height" )->GetText() );
  m_collisionRect.Bottom = m_collisionRect.Top + atoi( height.c_str() );
  
  return true;
}