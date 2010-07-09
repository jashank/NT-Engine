#include "GameState.h"

#include "App.h"
#include "GameObject.h"
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
  { "ObjectBlockingTile", LuaObjectBlockingTile },
  { "SetTile", LuaSetTile },
  { "NewState", LuaNewState },
  { "LogErr", LuaLogErr },
  { NULL, NULL }
};

/*******************************************
 Public Member Functions
*******************************************/

// NEED TO CONSIDER OPTIONAL ATTRIBUTES
GameState::GameState() {
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
}


bool GameState::LoadFromFile( const std::string &path ) {
  TiXmlDocument doc( path.c_str() );

  if ( doc.LoadFile() ) {
    TiXmlElement *root = doc.FirstChildElement( "state" );
    if ( root ) {
      TiXmlElement *elem = root->FirstChildElement( "tiles" );
      if ( elem ) {
        if ( !m_tileManager.LoadData( elem )) {
          LogErr( "Problem loading tiles in state file " + path );
          return false;
        }
      }

      elem = root->FirstChildElement( "game_objects" );
      if ( elem ) {
        if ( !m_gameObjectManager.LoadData( elem )) {
          LogErr( "Problem loading GameObjects in state file " + path );
          return false;
        }
      }

      elem = root->FirstChildElement( "GUI" );
      if ( elem ) {
        if ( !m_guiManager.LoadData( elem )) {
          LogErr( "Problem loading GUI in state file " + path );
          return false;
        }
      }

      elem = root->FirstChildElement( "sound" );
      if ( elem ) {
        if ( !m_soundManager.LoadData( elem )) {
          LogErr( "Problem loading sound in state file " + path );
          return false;
        }
      }
    } else {
      LogErr( "<state> tag not specified in state file " + path );
      return false;
    }
    return true;
  }

  LogErr( "GameState file not found: " + path );
  return false;
}


void GameState::HandleEvents() {
  m_gameObjectManager.HandleEvents();
  m_guiManager.HandleEvents();
}


void GameState::Update() {
  m_tileManager.Update();
  m_gameObjectManager.Update();
  m_guiManager.Update();
  m_soundManager.Update();
}


void GameState::Render() {
  // The rendering order is important.
  m_tileManager.Render();
  m_gameObjectManager.Render();
  m_guiManager.Render();
}


const TileManager& GameState::GetTileManager() const {
  return m_tileManager;
}


const GameObjectManager& GameState::GetGameObjectManager() const {
  return m_gameObjectManager;
}


bool GameState::ObjectBlockingTile( int x, int y ) {
  GameObject *gameObject = m_gameObjectManager.ObjectOnTile( x, y );
  if ( gameObject ) {
    return gameObject->BlockingTile();
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
    LogLuaErr( "String not passed for file path in CreateGameObject." );
    return luaL_error( L, "String not passed for file path in CreateGameObject." );
  }
  std::string path = lua_tostring( L, -3 );

  if( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in CreateGameObject." );
    return luaL_error( L, "Number not passed to x position in CreateGameObject." );
  }
  int tileX = lua_tointeger( L, -2 );

  if( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in CreateGameObject" );
    return luaL_error( L, "Number not passed to y position in CreateGameObject" );
  }
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    GameObject *newGameObject = new GameObject( path, tileX, tileY );
    App::GetApp()->GetCurrentState()->
      m_gameObjectManager.AddGameObject( newGameObject );

    Lunar<GameObject>::push( L, newGameObject );
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to CreateGameObject" );
    return 0;
  }
}


int GameState::LuaDestroyGameObject( lua_State *L ) {
  GameObject *GameObjectToDestroy = Lunar<GameObject>::check(L, 1);
  if ( GameObjectToDestroy ) {
    lua_remove(L, 1);
    App::GetApp()->GetCurrentState()->
      m_gameObjectManager.RemoveGameObject( GameObjectToDestroy );
  }
  return 0;
}


int GameState::LuaGetGameObject( lua_State *L ) {
  if ( !lua_isstring( L, -1 ) ) {
    LogLuaErr( "String not passed for GameObject type in GetGameObject." );
    return luaL_error( L, "String not passed for GameObject type in GetGameObject." );
  }
  std::string GameObjectType = lua_tostring( L, -1 );

  Lunar<GameObject>::push( L, App::GetApp()->GetCurrentState()->
    m_gameObjectManager.GetGameObject( GameObjectType ));
  return 1;
}


int GameState::LuaGetGameObjectOnTile( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in GetGameObjectOnTile." );
    return luaL_error( L, "Number not passed to x position in GetGameObjectOnTile." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in GetGameObjectOnTile." );
    return luaL_error( L, "Number not passed to y position in GetGameObjectOnTile." );
  }
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    Lunar<GameObject>::push( L, App::GetApp()->GetCurrentState()->
      m_gameObjectManager.ObjectOnTile( tileX, tileY ));
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to GetGameObjectOnTile" );
    return 0;
  }
}


int GameState::LuaGetTileInfo( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in GetTile." );
    return luaL_error( L, "Number not passed to x position in GetTile." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in GetTile." );
    return luaL_error( L, "Number not passed to y position in GetTile." );
  }
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    Tile tile = App::GetApp()->GetCurrentState()->
      m_tileManager.GetTile( tileX, tileY );
    lua_pushstring( L, tile.type.c_str() );
    lua_pushstring( L, tile.name.c_str() );
    lua_pushinteger( L,tile.id );
    lua_pushinteger( L, tile.cid );
    return 4;
  } else {
    LogLuaErr( "Negative tile passed to GetTileInfo" );
    return 0;
  }
}


int GameState::LuaTileIsCrossable( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in TileIsCrossable." );
    return luaL_error( L, "Number not passed to x position in TileIsCrossable." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in TileIsCrossable." );
    return luaL_error( L, "Number not passed to y position in TileIsCrossable." );
  }
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    lua_pushboolean( L, App::GetApp()->GetCurrentState()->
      m_tileManager.TileIsCrossable( tileX, tileY ));
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to TileIsCrossable" );
    return 0;
  }
}


int GameState::LuaObjectBlockingTile( lua_State *L ) {
  if ( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in ObjectBlockingTile" );
    return luaL_error( L, "Number not passed to x position in ObjectBlockingTile." );
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in ObjectBlockingTile." );
    return luaL_error( L, "Number not passed to y position in ObjectBlockingTile." );
  }
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    GameObject *gameObj = App::GetApp()->GetCurrentState()->
      m_gameObjectManager.ObjectOnTile( tileX, tileY );

    gameObj ? lua_pushboolean( L, gameObj->BlockingTile() ) : lua_pushboolean( L, false );
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to ObjectBlockingTile" );
    return 0;
  }
}


int GameState::LuaSetTile( lua_State *L ) {
  if ( !lua_isnumber( L, -4 ) ) {
    LogLuaErr( "Number not passed to x position in SetTile." );
    return luaL_error( L, "Number not passed to x position in SetTile." );
  }
  int tileX = lua_tointeger( L, -4 );

  if ( !lua_isnumber( L, -3 ) ) {
    LogLuaErr( "Number not passed to y position in SetTile." );
    return luaL_error( L, "Number not passed to y position in SetTile." );
  }
  int tileY = lua_tointeger( L, -3 );

  if ( !lua_isstring( L, -2 ) ) {
    LogLuaErr( "String not passed to tile name in SetTile." );
    return luaL_error( L, "String not passed to tile name in SetTile." );
  }
  std::string tileName = lua_tostring( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to collision ID in SetTile." );
    return luaL_error( L, "Number not passed to collision ID in SetTile." );
  }
  int collisionID = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    App::GetApp()->GetCurrentState()->
      m_tileManager.SetTile( tileX, tileY, tileName );
    App::GetApp()->GetCurrentState()->
      m_tileManager.SetCollision( tileX, tileY, collisionID );
  } else {
    LogLuaErr( "Negative tile passed to SetTile" );
  }

  return 0;
}


int GameState::LuaNewState( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to NewState" );
    return luaL_error( L, "String not passed to NewState" );
  }
  App::GetApp()->SetNextState( lua_tostring( L, -1 ));
  return 0;
}


int GameState::LuaLogErr( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to LuaLogLuaErr" );
    return luaL_error( L, "String not passed to LuaLogLuaErr" );
  }
  LogLuaErr( lua_tostring( L, -1 ));
  return 0;
}
