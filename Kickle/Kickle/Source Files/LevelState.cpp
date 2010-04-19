#include "LevelState.h"

#include "App.h"
#include "Configuration.h"
#include "GameObject.h"
#include "Utilities.h"
#include "TitleState.h"
#include "WorldState.h"
/************************************************
Data Members
************************************************/
LevelState *LevelState::m_instance = 0;

const luaL_Reg LevelState::luaLevelFuncts[] = {
  { "CreateGameObject", LuaCreateGameObject },
  { "DestroyGameObject", LuaDestroyGameObject },
  { "TileIsSolid", LuaTileIsSolid },
  { "TileHasGridObject", LuaTileHasGridObject },
  { "GetGameObject", LuaGetGameObject },
  { "GetGameObjectOnTile", LuaGetGameObjectOnTile },
  { "GetTile", LuaGetTile },
  { "SetTile", LuaSetTile },
  { "NextLevel", LuaNextLevel },
  { NULL, NULL }
};

/************************************************
Public Member Functions
************************************************/
LevelState::LevelState() {}


LevelState::~LevelState() {
	CleanUp();
}


LevelState* LevelState::GetInstance() {
	if( !m_instance ) {
		m_instance = new LevelState();
	}
	return m_instance;
}


void LevelState::DestroyInstance() {
	SAFEDELETE( m_instance );
}


void LevelState::Init() {
	SetInit( true );

  m_luaState = luaL_newstate();
  luaL_openlibs( m_luaState );
  luaL_register( m_luaState, "Level", luaLevelFuncts );
  Lunar<GameObject>::Register( m_luaState );

	App::GetApp()->SetClearColor( sf::Color(0,49,139) );
	// Create a LevelState with a place holder path.
  SetLevel( App::GetApp()->GetCurrentLevel() );
  m_timer.Reset( 100.f, false );
}


void LevelState::CleanUp() {
  //Check to see if m_luaState not equal to 0
  //because it WILL crash if closing a null pointer
  if( m_luaState ) {
    lua_close( m_luaState );
    m_luaState = 0;
  }
	SetInit( false );
}


void LevelState::Pause() {
}


void LevelState::Resume() {

}


void LevelState::HandleEvents() {
  if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Escape ) ) {
    App::GetApp()->SetNextState( TitleState::GetInstance() );
  }
  // Music Toggle
  if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::P ) ) {
    if ( m_soundList.IsPlaying() ) {
      m_soundList.Pause();
    }
  }
  if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::O ) ) {
    if ( !m_soundList.IsPlaying() ) {
      m_soundList.Play();
    }
  }
}


void LevelState::Update() {
  m_gameObjectMap.Update();
  m_soundList.Update();
}


void LevelState::Render() {
  // The rendering order is important.
	m_tileMap.Render();
  m_gameObjectMap.Render();
}


bool LevelState::TileIsSolid( const sf::Vector2f& position ) const {
  Uint tileX = GetVectorXTile( position );
  Uint tileY = GetVectorYTile( position );

  return ( m_collisionMap.TileIsSolid( tileX, tileY ) );
}


bool LevelState::TileIsSolid( Uint x, Uint y ) const {
  return m_collisionMap.TileIsSolid( x, y );
}


bool LevelState::TileHasGridObject( const sf::Vector2f &position ) {
  Uint tileX = GetVectorXTile( position );
  Uint tileY = GetVectorYTile( position );

  return TileHasGridObject( tileX, tileY );
}


bool LevelState::TileHasGridObject( Uint x, Uint y ) {
  GameObject *gameObject = m_gameObjectMap.ObjectOnTile( x, y );

  if ( gameObject != NULL ) {
    return gameObject->HasGridCollision();
  }

  return false;
}


int LevelState::GetTile( int x, int y ) const {
  return m_tileMap.GetTile( x, y );
}


void LevelState::SetTile (int x, int y, int tileId, int collisionId ) {
  m_tileMap.SetTile( x, y, tileId );
  m_collisionMap.SetCollision( x, y, collisionId );
}


lua_State* LevelState::GetLuaState() {
  return m_luaState;
}


GameObject *LevelState::DetectObjectCollision( const GameObject *gameObject ) {
  return m_gameObjectMap.DetectCollision( gameObject );
}


int LevelState::LuaCreateGameObject( lua_State *L ) {
  if( !lua_isstring( L, -3 ) ) {
    return luaL_error( L, "Invalid filepath for CreateGameObject." );
  }
  std::string path = lua_tostring( L, -3 );
  
  if( !lua_isnumber( L, -2 ) ) {
    return luaL_error( L, "Invalid tile x position for CreateGameObject." );
  }
  Uint tileX = static_cast<Uint>( lua_tointeger( L, -2 ) );

  if( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid tile y position for CreateGameObject." );
  }
  Uint tileY = static_cast<Uint>( lua_tointeger( L, -1 ) );

  GameObject *newGameObject = new GameObject( path, tileX, tileY );
  m_instance->m_gameObjectMap.AddGameObject( newGameObject );

  // Now we let the user grab the newGameObject on its creation
  Lunar<GameObject>::push( L, newGameObject );
  return 1;
}


int LevelState::LuaDestroyGameObject( lua_State *L ) {
  GameObject *gameObjectToDestroy = Lunar<GameObject>::check(L, 1);
  lua_remove(L, 1);
  m_instance->m_gameObjectMap.RemoveGameObject( gameObjectToDestroy );

  return 1;
}


int LevelState::LuaTileIsSolid( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    return luaL_error( L, "Invalid tile x position for IsTileSolid." );
  }
  Uint tileX = static_cast<Uint>( lua_tointeger( L, -2 ) );

  if ( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid tile y position for IsTileSolid." );
  }
  Uint tileY = static_cast<Uint>( lua_tointeger( L, -1 ) );

  lua_pushboolean( L, m_instance->TileIsSolid( tileX, tileY ) );
  
  return 1;
}


int LevelState::LuaTileHasGridObject( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    return luaL_error( L, "Invalid tile x position for TileHasSolidObject." );
  }
  Uint tileX = static_cast<Uint>( lua_tointeger( L, -2 ) );

  if ( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid tile y position for TileHasSolidObject." );
  }
  Uint tileY = static_cast<Uint>( lua_tointeger( L, -1 ) );

  lua_pushboolean( L, m_instance->TileHasGridObject( tileX, tileY ) );

  return 1;
}


int LevelState::LuaGetGameObject( lua_State *L ) {
  if ( !lua_isstring( L, -1 ) ) {
    return luaL_error( L, "Did not pass a string to GetGameObject." );
  }
  std::string objectType = lua_tostring( L, -1 );

  Lunar<GameObject>::push( L, m_instance->
                           m_gameObjectMap.GetGameObject( objectType ));
  return 1;
}


int LevelState::LuaGetGameObjectOnTile( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    return luaL_error( L, "Invalid tile x position for GetGameObjectOnTile." );
  }
  Uint tileX = static_cast<Uint>( lua_tointeger( L, -2 ) );

  if ( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid tile y position for GetGameObjectOnTile." );
  }
  Uint tileY = static_cast<Uint>( lua_tointeger( L, -1 ) );

  Lunar<GameObject>::push( L,
                  m_instance->m_gameObjectMap.ObjectOnTile( tileX, tileY ));
  return 1;
}


int LevelState::LuaGetTile( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    return luaL_error( L, "Invalid tile x position for GetTile." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid tile y position for GetTile." );
  }
  int tileY = lua_tointeger( L, -1 );

  lua_pushinteger( L, m_instance->GetTile( tileX, tileY ));

  return 1;
}


int LevelState::LuaSetTile( lua_State *L ) {
  if ( !lua_isnumber( L, -4 ) ) {
    return luaL_error( L, "Invalid tile x position for SetTile." );
  }
  int tileX = lua_tointeger( L, -4 );

  if ( !lua_isnumber( L, -3 ) ) {
    return luaL_error( L, "Invalid tile y position for SetTile." );
  }
  int tileY = lua_tointeger( L, -3 );
  
  if ( !lua_isnumber( L, -2 ) ) {
    return luaL_error( L, "Invalid tileID for SetTile." );
  }
  int tileID = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid collisionID for SetTile." );
  }
  int collisionID = lua_tointeger( L, -1 );

  m_instance->SetTile( tileX, tileY, tileID, collisionID );
  return 0;
}
  
int LevelState::LuaNextLevel( lua_State *L ) {
  WorldState::GetInstance()->AddPoints( 
    (int) m_instance->m_timer.GetTime() * 10 );
  App::GetApp()->SetNextState( WorldState::GetInstance() );
  return 0;
}

/************************************************
Private Methods
************************************************/

Uint LevelState::GetVectorXTile( const sf::Vector2f &position ) const {
  return (Uint)( position.x - Configuration::GetXPad() ) / 
                 Configuration::GetTileSize();
}


Uint LevelState::GetVectorYTile( const sf::Vector2f &position ) const {
  return (Uint)( position.y - Configuration::GetYPad() ) / 
                 Configuration::GetTileSize();
}


bool LevelState::SetLevel( std::string LevelStatePath ) {
  TiXmlDocument doc ( LevelStatePath.c_str() );
  
  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlElement* root = doc.FirstChildElement( "map" );

  // Get the tile sheet
  std::string tileSheet( root->FirstChildElement( "tile_sheet" )->GetText() );
  // Get the tile animations
  std::string tileAnims( root->FirstChildElement( "tile_anims" )->GetText() );

  //Load the appropriate tile animation data 
  //and tilesheet into the tile map
  m_tileMap.LoadTileAnims( tileSheet, tileAnims );

  // The tile map parses its own layout.
  if ( root->FirstChildElement( "tile_layout") ) {
    m_tileMap.SetTileLayout( root->FirstChildElement( "tile_layout" ) );
  }

  // THe collision map parses its own layout
  if ( root->FirstChildElement( "collision_layout" )->GetText() ) {
    m_collisionMap.SetCollisionMap( root->FirstChildElement( "collision_layout" ) );
  }

  // GameObjectMap parses its section by itself instead of in this method.
  if ( root->FirstChildElement( "game_objects" ) ) {
    m_gameObjectMap.SetGameObjectMap( root->FirstChildElement( "game_objects" ) );
  }

  // SoundList parses its section by itself instead of in this method.
  if ( root->FirstChildElement( "music_playlist" ) ) {
    m_soundList.SetSoundList( root->FirstChildElement( "music_playlist" ) );
  }

	return true;
}


