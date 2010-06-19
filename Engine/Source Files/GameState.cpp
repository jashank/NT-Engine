#include "GameState.h"

#include "App.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "TileManager.h"
#include "tinyxml.h"
#include "Utilities.h"

/*******************************************
 Data Members
*******************************************/
const luaL_Reg GameState::LuaGameStateFuncts[] = {
  { "CreateGameObject", LuaCreateGameObject },
  { "DestroyGameObject", LuaDestroyGameObject },
  { "GetGameObject", LuaGetGameObject },
  { "GetGameObjectOnTile", LuaGetGameObjectOnTile },
  { "GetTileInfo", LuaGetTileInfo },
  { "TileIsCrossable", LuaTileIsCrossable },
  { "TileHasGridObject", LuaTileHasGridObject },
  { "SetTile", LuaSetTile },
  { "NewState", LuaNewState },
  { NULL, NULL }
};

/*******************************************
 Public Member Functions
*******************************************/

// NEED TO CONSIDER OPTIONAL ATTRIBUTES
GameState::GameState()
 : m_tileManager( NULL ),
   m_collisionManager( NULL ),
   m_gameObjectManager( NULL ),
   m_soundManager( NULL ) {
  m_luaState = luaL_newstate();
  luaL_openlibs( m_luaState );
  luaL_register( m_luaState, "Game", LuaGameStateFuncts );
  Lunar<GameObject>::Register( m_luaState );
}


GameState::~GameState() {
  // Must check if lua state isn't null, otherwise app will crash
  if( m_luaState ) {
    lua_close( m_luaState );
    m_luaState = 0;
  }
  SAFEDELETE( m_soundManager );
  SAFEDELETE( m_gameObjectManager );
  SAFEDELETE( m_collisionManager );
  SAFEDELETE( m_tileManager );
}


bool GameState::LoadFromFile( const std::string &path ) {
  TiXmlDocument doc( path.c_str() );

  if ( doc.LoadFile() ) {
    TiXmlElement *root = doc.FirstChildElement( "state" );

    m_tileManager = new TileManager( root->FirstChildElement( "tiles" ));
    m_collisionManager = new CollisionManager( root->FirstChildElement( "collision_layout" ));
    m_gameObjectManager = new GameObjectManager( root->FirstChildElement( "game_objects" ));
    // LOAD GUI DATA
    m_soundManager = new SoundManager( root->FirstChildElement( "sound" ));
    return true;
  }

  LogErr( "GameState file not found." );
  return false;
}


void GameState::HandleEvents() {
  m_gameObjectManager->HandleEvents();
}


void GameState::Update() {
  m_tileManager->Update();
  m_gameObjectManager->Update();
  m_soundManager->Update();
}


void GameState::Render() {
  // The rendering order is important.
	m_tileManager->Render();
  m_gameObjectManager->Render();
}


TileManager* GameState::GetTileManager() const {
  return m_tileManager;
}


CollisionManager* GameState::GetCollisionManager() const {
  return m_collisionManager;
}


GameObjectManager* GameState::GetGameObjectManager() const {
  return m_gameObjectManager;
}


bool GameState::TileHasGridObject( unsigned int x, unsigned int y ) {
  GameObject *gameObject = m_gameObjectManager->ObjectOnTile( x, y );
  if ( gameObject ) {
    return gameObject->HasGridCollision();
  }
  return false;
}


//-------------------------------------------------------
// Lua Functions
//-------------------------------------------------------

lua_State* GameState::GetLuaState() {
  return m_luaState;
}


int GameState::LuaCreateGameObject( lua_State *L ) {
  if( !lua_isstring( L, -3 ) ) {
    LogErr( "String not passed for file path in CreateGameObject." );
    return luaL_error( L, "String not passed for file path in CreateGameObject." );
  }
  std::string path = lua_tostring( L, -3 );

  if( !lua_isnumber( L, -2 ) ) {
    LogErr( "Number not passed to x position in CreateGameObject." );
    return luaL_error( L, "Number not passed to x position in CreateGameObject." );
  }
  int tileX = lua_tointeger( L, -2 );

  if( !lua_isnumber( L, -1 ) ) {
    LogErr( "Number not passed to y position in CreateGameObject" );
    return luaL_error( L, "Number not passed to y position in CreateGameObject" );
  }
  int tileY = lua_tointeger( L, -1 );

  GameObject *newGameObject = new GameObject( path, tileX, tileY );
  App::GetApp()->GetCurrentState()->
    m_gameObjectManager->AddGameObject( newGameObject );

  Lunar<GameObject>::push( L, newGameObject );
  return 1;
}


int GameState::LuaDestroyGameObject( lua_State *L ) {
  GameObject *GameObjectToDestroy = Lunar<GameObject>::check(L, 1);
  if ( GameObjectToDestroy ) {
    lua_remove(L, 1);
    App::GetApp()->GetCurrentState()->
      m_gameObjectManager->RemoveGameObject( GameObjectToDestroy );
  }

  return 0;
}


int GameState::LuaGetGameObject( lua_State *L ) {
  if ( !lua_isstring( L, -1 ) ) {
    LogErr( "String not passed for GameObject type in GetGameObject." );
    return luaL_error( L, "String not passed for GameObject type in GetGameObject." );
  }
  std::string GameObjectType = lua_tostring( L, -1 );

  Lunar<GameObject>::push( L, App::GetApp()->GetCurrentState()->
    m_gameObjectManager->GetGameObject( GameObjectType ));
  return 1;
}


int GameState::LuaGetGameObjectOnTile( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogErr( "Number not passed to x position in GetGameObjectOnTile." );
    return luaL_error( L, "Number not passed to x position in GetGameObjectOnTile." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogErr( "Number not passed to y position in GetGameObjectOnTile." );
    return luaL_error( L, "Number not passed to y position in GetGameObjectOnTile." );
  }
  int tileY = lua_tointeger( L, -1 );

  Lunar<GameObject>::push( L, App::GetApp()->GetCurrentState()->
    m_gameObjectManager->ObjectOnTile( tileX, tileY ));
  return 1;
}


int GameState::LuaGetTileInfo( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogErr( "Number not passed to x position in GetTile." );
    return luaL_error( L, "Number not passed to x position in GetTile." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogErr( "Number not passed to y position in GetTile." );
    return luaL_error( L, "Number not passed to y position in GetTile." );
  }
  int tileY = lua_tointeger( L, -1 );

  TileManager::tileInfo tile = App::GetApp()->GetCurrentState()->
    m_tileManager->GetTile( tileX, tileY );
  lua_pushstring( L, std::tr1::get<0>( tile ).c_str() );
  lua_pushstring( L, std::tr1::get<1>( tile ).c_str() );
  lua_pushinteger( L, std::tr1::get<2>( tile ));

  return 3;
}


int GameState::LuaTileIsCrossable( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogErr( "Number not passed to x position in TileIsCrossable." );
    return luaL_error( L, "Number not passed to x position in TileIsCrossable." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogErr( "Number not passed to y position in TileIsCrossable." );
    return luaL_error( L, "Number not passed to y position in TileIsCrossable." );
  }
  int tileY = lua_tointeger( L, -1 );

  lua_pushboolean( L, App::GetApp()->GetCurrentState()->
    m_collisionManager->TileIsCrossable( tileX, tileY ));

  return 1;
}


int GameState::LuaTileHasGridObject( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogErr( "Number not passed to x position in TileHasGridGameObject." );
    return luaL_error( L, "Number not passed to x position in TileHasGridGameObject." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogErr( "Number not passed to y position in TileHasGridGameObject." );
    return luaL_error( L, "Number not passed to y position in TileHasGridGameObject." );
  }
  int tileY = lua_tointeger( L, -1 );

  GameObject *gameObj = App::GetApp()->GetCurrentState()->
    m_gameObjectManager->ObjectOnTile( tileX, tileY );

  gameObj ? lua_pushboolean( L, gameObj->HasGridCollision() ) : lua_pushboolean( L, false );

  return 1;
}


int GameState::LuaSetTile( lua_State *L ) {
  if ( !lua_isnumber( L, -4 ) ) {
    LogErr( "Number not passed to x position in SetTile." );
    return luaL_error( L, "Number not passed to x position in SetTile." );
  }
  int tileX = lua_tointeger( L, -4 );

  if ( !lua_isnumber( L, -3 ) ) {
    LogErr( "Number not passed to y position in SetTile." );
    return luaL_error( L, "Number not passed to y position in SetTile." );
  }
  int tileY = lua_tointeger( L, -3 );

  if ( !lua_isstring( L, -2 ) ) {
    LogErr( "String not passed to tile name in SetTile." );
    return luaL_error( L, "String not passed to tile name in SetTile." );
  }
  std::string tileName = lua_tostring( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogErr( "Number not passed to collision ID in SetTile." );
    return luaL_error( L, "Number not passed to collision ID in SetTile." );
  }
  int collisionID = lua_tointeger( L, -1 );

  App::GetApp()->GetCurrentState()->
    m_tileManager->SetTile( tileX, tileY, tileName );
  App::GetApp()->GetCurrentState()->
    m_collisionManager->SetCollision( tileX, tileY, collisionID );

  return 0;
}


int GameState::LuaNewState( lua_State *L ) {
  //TODO
  return 0;
}

