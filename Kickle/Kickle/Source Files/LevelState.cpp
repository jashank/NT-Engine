#include "LevelState.h"

#include <sstream>

#include "App.h"
#include "Configuration.h"
#include "GameObject.h"
#include "Utilities.h"

/************************************************
Data Members
************************************************/
LevelState *LevelState::m_instance = 0;

const luaL_Reg LevelState::luaLevelFuncts[] = {
  { "CreateGameObject", LuaCreateGameObject },
  { "TileIsSolid", LuaTileIsSolid },
  { "TileHasGridObject", LuaTileHasGridObject },
  { "GetGameObject", LuaGetGameObject },
  { 0, 0 }
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
	App::GetApp()->SetClearColor( sf::Color(0,49,139) );
	// Create a LevelState with a place holder path.
	SetLevel( "Content/Core/Levels/TestLevel.xml" );
  //GameObject::AssignLevel( m_instance );
}


void LevelState::CleanUp() {
	SetInit( false );
}


void LevelState::Pause() {

}


void LevelState::Resume() {

}


void LevelState::HandleEvents() {
  // Does nothing
}


void LevelState::Update() {
  m_gameObjectMap.Update();
}


void LevelState::Render() {
  // The Rendering order is important.
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
    return gameObject->CollisionIsGridBased();
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

  m_instance->m_gameObjectMap.AddGameObject(
    new GameObject( path, tileX, tileY )
  );

  return 0;
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


void LevelState::RegisterLuaLevelFuncts( lua_State *L ) {
  luaL_register( L, "Game", luaLevelFuncts );
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


  // Load in tiles.
  Uint mapSize = Configuration::GetMapSize();

  std::string tileString( root->FirstChildElement( "tile_layout" )->GetText() );
  std::stringstream tileMapStream( tileString, std::ios_base::in );

  int tileLayout[15][15]; // TODO fix to dynamic
 
  int currentTile = 0;
  for( Uint i=0; i < mapSize; i++ ) {
    for ( Uint j=0; j < mapSize; j++ ) {
      if ( tileMapStream >> currentTile ) {
        tileLayout[i][j] = currentTile;
      } else {
          tileLayout[i][j] = -1;
      }
    }
  }

  // Load In Collision Layout
  std::string collisionString( root->FirstChildElement( "collision_layout" )->GetText() );
  std::stringstream collisionMapStream( collisionString, std::ios_base::in );

  int collisionLayout[15][15]; // TODO fix to dynamic
 
  for( Uint i=0; i < mapSize; i++ ) {
    for ( Uint j=0; j < mapSize; j++ ) {
      if ( collisionMapStream >> currentTile ) {
          collisionLayout[i][j] = currentTile;
      } else {
          collisionLayout[i][j] = -1;
      }
    }
  }

  // GameObjectMap parses its section by itself instead of in this method.
  if ( root->FirstChildElement( "game_objects" ) ) {
    m_gameObjectMap.SetGameObjectMap( root->FirstChildElement( "game_objects" ) );
  }

  //Load the appropriate tile animation data 
  //and tilesheet into the tile map
  m_tileMap.LoadTileAnims( tileSheet, tileAnims );
	
  //Set tile/collision layouts
	m_tileMap.SetTileLayout( tileLayout );
  m_collisionMap.SetCollisionMap( collisionLayout );

	return true;
}


