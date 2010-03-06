#include "GameObject.h"

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
const Level* GameObject::m_level = 0;

/************************************************
Public Members
************************************************/
const char GameObject::className[] = "GameObject";
Lunar<GameObject>::RegType GameObject::methods[] = {
  { "MoveDir", &GameObject::LuaMoveDir },
  { "SetAnimation", &GameObject::LuaSetAnimation },
  { 0, 0 }
};

/************************************************
Public Methods
************************************************/
GameObject::GameObject( lua_State *L )
 : m_play( false ),
   m_animation( 0 ),
   m_frameTime( 0.0f ),
   m_animData( 0 ),
   m_moving( false ),
   m_id( -1 ),
   m_luaState(luaL_newstate()) {
  if( !lua_isstring( L, -1 ) ) {
    luaL_error( L, "Invalid argument for GameObject." );
  }

  std::string xmlPath( lua_tostring( L, -1 ) );
  if( !LoadFromFile( xmlPath ) ) {
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
   m_animData( 0 ),
   m_moving( false ),
   m_id( -1 ),
   m_luaState(luaL_newstate()) { 
  if( !LoadFromFile( xmlGameObjectPath ) ) {
    lua_close( m_luaState );
    m_luaState = 0;
    throw "Cannot load GameObject XML file";
  }
  
  InitLua();
}

GameObject::GameObject( const std::string &xmlGameObjectPath, Uint tileX, Uint tileY )
 : m_play( false ),
   m_animation( 0 ),
   m_frameTime( 0.0f ),
   m_animData( 0 ),
   m_moving( false ),
   m_id( -1 ),
   m_luaState(luaL_newstate()) { 
  if( !LoadFromFile( xmlGameObjectPath ) ) {
    lua_close( m_luaState );
    m_luaState = 0;
    throw "Cannot load GameObject XML file";
  }
  
  //Calculate the float positions given tileX and tileY
  //Taking into account tilemap's offset location, tile size, and
  //max tiles across/down
  float x =
  static_cast<float>( 
    Config::X_PAD +         //map's offset X location
    Config::TILE_SIZE *     //tile size in pixels
    (tileX%Config::MAP_SIZE)//number of tiles across tilemap
  );
  float y =
  static_cast<float>( 
    Config::Y_PAD +         //map's offset Y location
    Config::TILE_SIZE *     //tile size in pixels
    (tileY%Config::MAP_SIZE)//number of tiles down tilemap
  );
  
  //TODO - I need to handle this every time someone calls SetAnimation as well,
  //just so animations with different frame heights will stll be aligned to the 
  //grid

  //Take into account the sprites that are taller than a normal tile
  y -= m_animData->GetFrameHeight( m_animation )%Config::TILE_SIZE;

  SetPosition( x, y );

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


bool GameObject::LoadFromFile( const std::string& filepath ) {
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


void GameObject::AssignLevel( const Level *level ) {
  m_level = level;
}


void GameObject::MoveDir( Dir direction ) {
  if( !m_moving ) {
    m_direction = direction;

    sf::Vector2f tileToMoveTo( GetPosition() );
    
    //Take into account the sprites that are taller than a normal tile
    tileToMoveTo.y += m_animData->GetFrameHeight( m_animation )%Config::TILE_SIZE;

    switch ( m_direction ) {
      case Up: {
        tileToMoveTo.y -= Config::TILE_SIZE;
        break;
      }
      case Down: {
        tileToMoveTo.y += Config::TILE_SIZE;
        break;
      }
      case Left: {
        tileToMoveTo.x -= Config::TILE_SIZE;
        break;
      }
      case Right: {
        tileToMoveTo.x += Config::TILE_SIZE;
        break;
      }
      default: {}
    }

    if ( !m_level->IsTileSolid( tileToMoveTo ) ) {
      if ( m_level->GetGameObject( tileToMoveTo ) == NULL ) {
        m_moving = true;
      } else {
        m_moving = false; // Do collision stuff here.
      }
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
    static_cast<int>(pos.x-Config::X_PAD)%Config::TILE_SIZE == 0 &&
    static_cast<int>(
      pos.y + 
      m_animData->GetFrameHeight( m_animation ) - 
      Config::TILE_SIZE -
      Config::Y_PAD
    )%Config::TILE_SIZE == 0 
  ) {
    m_moving = false;
  }
}

void GameObject::Update() {
  AnimUpdate();
  if( m_moving ) {
    static const float SPEED = 1.0f;
    switch( m_direction ) {
    case Up:
      Move( 0.0f, -SPEED );
      break;
    case Down:
      Move( 0.0f, SPEED );
      break;
    case Left:
      Move( -SPEED, 0.0f );
      break;
    case Right:
      Move( SPEED, 0.0f );
      break;
    }
  }



  static sf::Vector2f pos;
  pos = GetPosition();
  static int x, y = 0;

  //Compensate for the tilemap's position and the taller gameobjects
  x = static_cast<int>(pos.x - Config::X_PAD);
  y = 
    static_cast<int>(
        (pos.y - Config::Y_PAD) +
        (m_animData->GetFrameHeight( m_animation ) - 
        Config::TILE_SIZE)
    );
    
  //If aligned perfectly in grid
  if( x % Config::TILE_SIZE == 0 && y % Config::TILE_SIZE == 0 ) {
    m_moving = false;
    
    lua_getglobal( m_luaState, "HandleUserInput" );
    if ( lua_isfunction( m_luaState, -1 )) {
      Lunar<GameObject>::push( m_luaState, this );
      lua_call( m_luaState, 1, 0 );
    } else {
      lua_pop( m_luaState, 1 );
    }

    
    //call AILogic lua function
    lua_getglobal( m_luaState, "AILogic" );
    if ( lua_isfunction( m_luaState, -1 )) {
      Lunar<GameObject>::push( m_luaState, this );
      lua_call( m_luaState, 1, 0 );
    } else {
      lua_pop( m_luaState, 1 );
    }
  }
}

void GameObject::SetTile( unsigned int x, unsigned int y ) {
  // Check that the tile is valid
  if ( App::GetApp()->GetConfig()->IsTileValid( x, y ) ) {
    //Converts it to pixel locations and passes it through the
    // sf::Sprite SetPosition fucncton
    this->SetPosition( (float)x * App::GetApp()->GetConfig()->GetTileSize() +
                       App::GetApp()->GetConfig()->GetXPad(),
                       (float)y * App::GetApp()->GetConfig()->GetTileSize() +
                       App::GetApp()->GetConfig()->GetYPad() );
  }
}

unsigned int GameObject::GetTileX() {
  unsigned int xTilePosition = (unsigned int)(this->GetPosition().x -
                                App::GetApp()->GetConfig()->GetXPad()) /
                                App::GetApp()->GetConfig()->GetTileSize();
  return xTilePosition;
}
  
unsigned int GameObject::GetTileY() {
  // Note the addition of the tilesize to get the Y tile from the
  // bottom of the feet.
  unsigned int yTilePosition = (unsigned int)(this->GetPosition().y + 
                                App::GetApp()->GetConfig()->GetTileSize() -
                                App::GetApp()->GetConfig()->GetYPad()) /
                                App::GetApp()->GetConfig()->GetTileSize();
  return yTilePosition;
}

void GameObject::SetId( int id ) {
  m_id = id;
}
  
int GameObject::GetId() {
  return m_id;
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
  luaL_dofile( m_luaState, m_luaScript.c_str() );
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