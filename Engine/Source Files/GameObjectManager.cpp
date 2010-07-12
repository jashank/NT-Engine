#include "GameObjectManager.h"

#include <algorithm>
#include <queue>

#include <SFML/Graphics.hpp>

#include "App.h"
#include "Config.h"
#include "GameObject.h"
#include "tinyxml.h"
#include "Utilities.h"

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
            int x, y = 0;
            instance->QueryIntAttribute( "x", &x );
            instance->QueryIntAttribute( "y", &y );
            if ( x >= 0 && y >= 0 ) {
              AddGameObject( new GameObject( path, x, y ));
            } else {
              LogErr( "Tile location negative for GameObject in state file." );
              return false;
            }
          } while ( instance = instance->NextSiblingElement( "inst" ));
        } else {
          LogErr( "No instances specified for GameObject in state file." );
          return false;
        }
      } else {
        LogErr( "No path specified for GameObject in state file." );
        return false;
      }
    } while ( objectType = objectType->NextSiblingElement( "object" ));
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


/********************************************
  Private Methods
*********************************************/
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
