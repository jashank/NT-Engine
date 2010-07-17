#include "GameObjectManager.h"

#include <queue>

#include <cstdlib>

#include <SFML/Graphics.hpp>

#include "App.h"
#include "Config.h"
#include "GameObject.h"
#include "GameState.h"
#include "tinyxml.h"
#include "Utilities.h"

/*******************************************
 Data Members
*******************************************/
const luaL_Reg GameObjectManager::LuaFuncs[] = {
  { "CreateGameObject", LuaCreateGameObject },
  { "DestroyGameObject", LuaDestroyGameObject },
  { "GetGameObject", LuaGetGameObject },
  { "GetGameObjects", LuaGetGameObjects },
  { "GetNearestGameObject", LuaGetNearestGameObject },
  { "GetGameObjectOnTile", LuaGetGameObjectOnTile },
  { "ObjectBlockingTile", LuaObjectBlockingTile },
  { NULL, NULL }
};

/********************************
Constructor and Destructor
********************************/
GameObjectManager::~GameObjectManager() {
  for ( GameObjItr gameObj = m_gameObjects.begin();
        gameObj != m_gameObjects.end(); ++gameObj ) {
    if ( *gameObj != NULL ) {
      SAFEDELETE( *gameObj );
    }
  }
}

/*******************************
Public Methods
*******************************/
bool GameObjectManager::LoadData( const TiXmlElement *dataRoot ) {
  const TiXmlElement *objectType = dataRoot->FirstChildElement( "object" );
  if ( objectType ) {
    do {
      const char *path = objectType->Attribute( "path" );
      if ( path ) {
        const TiXmlElement *instance = objectType->FirstChildElement( "inst" );
        if ( instance ) {
          do {
            int x, y, strip = 0;
            instance->QueryIntAttribute( "x", &x );
            instance->QueryIntAttribute( "y", &y );
            instance->QueryIntAttribute( "strip", &strip );
            if ( x >= 0 && y >= 0 && strip >= 0 ) {
              AddGameObject( new GameObject( path, x, y, strip ));
            } else {
              LogErr( "Tile location or strip negative for GameObject in state file." );
              return false;
            }
          } while ( (instance = instance->NextSiblingElement( "inst" )) );
        } else {
          LogErr( "No instances specified for GameObject in state file." );
          return false;
        }
      } else {
        LogErr( "No path specified for GameObject in state file." );
        return false;
      }
    } while ( (objectType = objectType->NextSiblingElement( "object" )) );
  } else {
    LogErr( "No game_object specified in <game_objects>. Thus, not necessary." );
    return false;
  }
  return true;
}


void GameObjectManager::HandleEvents() {
  GameObjItr gameObj = m_gameObjects.begin();
  for ( ; gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->HandleEvents();
    }
  }
}


void GameObjectManager::Update() {
  GameObjItr gameObj = m_gameObjects.begin();
  // Size used because DetectCollision could potentially move
  // elements to the end of the list. Don't need to worry about
  // iterating past end because it is guaranteed that no objects
  // will be removed in UpdateCollision().
  for ( int i = 0; i < m_gameObjects.size(); ++i ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->UpdateCollision( DetectCollision( *gameObj ));
    }
    ++gameObj;
  }

  gameObj = m_gameObjects.begin();
  for ( ; gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->UpdateAI();
    }
  }

  gameObj = m_gameObjects.begin();
  for ( ; gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->UpdateRendering();
    }
  }

  for ( int i = 0; i < m_toBeDestroyed.size(); i++ ) {
    m_gameObjects.remove( m_toBeDestroyed[i] );
    SAFEDELETE( m_toBeDestroyed[i] );
  }
  m_toBeDestroyed.clear();
}


void GameObjectManager::Render() const {
  std::priority_queue< std::pair<float, GameObject*> > renderOrder;

  for ( GameObjItrConst gameObj = m_gameObjects.begin();
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      renderOrder.push(
        std::make_pair( -( (*gameObj)->GetPosition().y ), *gameObj ) );
    }
  }

  while ( !renderOrder.empty() ) {
    App::GetApp()->Draw( *renderOrder.top().second );
    renderOrder.pop();
  }
}


bool GameObjectManager::ObjectBlockingTile( int x, int y ) const {
  for ( GameObjItrConst gameObj = m_gameObjects.begin();
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj ) {
      if (( *gameObj )->GetTileX() == x &&
          ( *gameObj )->GetTileY() == y &&
          ( *gameObj )->BlockingTile()) {
        return true;
      }
    }
  }

  return false;
}

/***************************************
 * Lua Functions
 * ************************************/
void GameObjectManager::RegisterLuaFuncs( lua_State *L ) {
  luaL_register( L, "Game", LuaFuncs );
  Lunar<GameObject>::Register( L );
}


int GameObjectManager::LuaCreateGameObject( lua_State *L ) {
  if( !lua_isstring( L, -3 )) {
    LogLuaErr( "String not passed for file path in CreateGameObject." );
    return luaL_error( L, "String not passed for file path in CreateGameObject." );
  }
  std::string path = lua_tostring( L, -3 );

  if( !lua_isnumber( L, -2 ) || !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to tile location in CreateGameObject." );
    return luaL_error( L,
      "Number not passed to tile location  in CreateGameObject." );
  }
  int tileX = lua_tointeger( L, -2 );
  int tileY = lua_tointeger( L, -1 );

  if ( tileX >= 0 && tileY >= 0 ) {
    GameObject *newGameObject = new GameObject( path, tileX, tileY, 0 );
    Inst().AddGameObject( newGameObject );
    Lunar<GameObject>::push( L, newGameObject );
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to CreateGameObject" );
    return 0;
  }
}


int GameObjectManager::LuaDestroyGameObject( lua_State *L ) {
  GameObject *GameObjectToDestroy = Lunar<GameObject>::check(L, 1);
  if ( GameObjectToDestroy ) {
    lua_remove( L, 1 );
    Inst().RemoveGameObject( GameObjectToDestroy );
  }
  return 0;
}


int GameObjectManager::LuaGetGameObject( lua_State *L ) {
  if ( !lua_isstring( L, -1 ) ) {
    LogLuaErr( "String not passed for GameObject type in GetGameObject." );
    return luaL_error( L, "String not passed for GameObject type in GetGameObject." );
  }
  std::string GameObjectType = lua_tostring( L, -1 );

  Lunar<GameObject>::push( L, Inst().GetGameObject( GameObjectType ));
  return 1;
}


int GameObjectManager::LuaGetGameObjects( lua_State *L ) {
  if ( !lua_isstring( L, -1 ) ) {
    LogLuaErr( "String not passed for GameObject type in GetGameObject." );
    return luaL_error( L, "String not passed for GameObject type in GetGameObject." );
  }
  std::string type = lua_tostring( L, -1 );

  lua_newtable( L );
  int newTable = lua_gettop( L );
  int index = 1; // Lua table indices start at 1

  std::list<GameObject*>::const_iterator obj = Inst().m_gameObjects.begin();
  while ( obj != Inst().m_gameObjects.end()) {
    if ( *obj && ( *obj )->GetType() == type ) {
      Lunar<GameObject>::push( L, *obj );
      lua_rawseti( L, newTable, index );
      ++index;
    }
    ++obj;
  }
  return 1;
}


int GameObjectManager::LuaGetNearestGameObject( lua_State *L ) {
  if ( !lua_isstring( L, -3 )) {
    LogLuaErr( "String not passed for object type to GetNearestGameObject." );
    return luaL_error( L,
      "String not passed for object type to GetNearestGameObject." );
  }
  std::string type = lua_tostring( L, -3 );

  if ( !lua_isnumber( L, -2 ) || !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed for tile location to GetNearestGameObject" );
    return luaL_error( L,
      "Number not passed for tile location to GetNearestGameObject" );
  }
  int x = lua_tointeger( L, -2 );
  int y = lua_tointeger( L, -1 );

  int distanceX = 100; // Turn into width of 3d vector when implemented
  int distanceY = 100; // Turn into height of 3d vector when implemented
  GameObject *nearestObj = NULL;

  std::list<GameObject*>::const_iterator obj = Inst().m_gameObjects.begin();
  while ( obj != Inst().m_gameObjects.end()) {
    if ( *obj ) {
      if (( type != "" && ( *obj )->GetType() == type ) || ( type == "" )) {
        int distanceX2 = abs(( *obj )->GetTileX() - x );
        int distanceY2 = abs(( *obj )->GetTileY() - y );
        if (( distanceX2 + distanceY2 ) < ( distanceX + distanceY )) {
          distanceX = distanceX2;
          distanceY = distanceY2;
          nearestObj = *obj;
        }
      }
    }
    ++obj;
  }
  Lunar<GameObject>::push( L, nearestObj );
  return 1;
}


int GameObjectManager::LuaGetGameObjectOnTile( lua_State *L ) {
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
    Lunar<GameObject>::push( L, Inst().ObjectOnTile( tileX, tileY ));
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to GetGameObjectOnTile" );
    return 0;
  }
}


int GameObjectManager::LuaObjectBlockingTile( lua_State *L ) {
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
    lua_pushboolean( L, Inst().ObjectBlockingTile( tileX, tileY ));
    return 1;
  } else {
    LogLuaErr( "Negative tile passed to ObjectBlockingTile" );
    return 0;
  }
}

/********************************************
  Private Methods
*********************************************/
void GameObjectManager::AddGameObject( GameObject *gameObject ) {
  m_gameObjects.push_back( gameObject );
  gameObject->Play();
}


void GameObjectManager::RemoveGameObject( GameObject *gameObject ) {
  if ( std::find( m_toBeDestroyed.begin(),
    m_toBeDestroyed.end(), gameObject ) == m_toBeDestroyed.end() ) {
    m_toBeDestroyed.push_back( gameObject );
  }
}


GameObject* GameObjectManager::GetGameObject( const std::string &type ) const {
  for ( GameObjItrConst gameObj = m_gameObjects.begin();
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      if (( *gameObj )->GetType() == type ) {
        return *gameObj;
      }
    }
  }

  return NULL;
}


GameObject* GameObjectManager::ObjectOnTile( int x, int y ) const {
  for ( GameObjItrConst gameObj = m_gameObjects.begin();
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      if (( *gameObj )->GetTileX() == x &&
          ( *gameObj )->GetTileY() == y ) {
        return *gameObj;
      }
    }
  }

  return NULL;
}


GameObject* GameObjectManager::DetectCollision( const GameObject *
  gameObject ) {
  const sf::FloatRect &mainObj = gameObject->GetCollisionBox();

  for ( GameObjItr gameObj = m_gameObjects.begin();
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj && *gameObj != gameObject ) {
      const sf::FloatRect &otherObj = ( *gameObj )->GetCollisionBox();
      if( mainObj.Intersects( otherObj ) ) {
        // So next collision check will return a different object colliding
        // with 'gameObject' if there is one
        m_gameObjects.splice( m_gameObjects.end(), m_gameObjects, gameObj );
        return m_gameObjects.back();
      }
    }
  }

  return NULL;
}


GameObjectManager& GameObjectManager::Inst() {
  return App::GetApp()->GetCurrentState()->GetGameObjectManager();
}
