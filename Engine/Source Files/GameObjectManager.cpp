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
GameObjectManager::GameObjectManager( const TiXmlElement *dataRoot ) {
  LoadData( dataRoot );
}


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
void GameObjectManager::LoadData( const TiXmlElement *dataRoot ) {
  int positionX;
  int positionY;

  const TiXmlElement *objectType = dataRoot->FirstChildElement( "game_object" );
  do {
    std::string dataPath = objectType->Attribute( "path" );
    const TiXmlElement *objectInstance = objectType->FirstChildElement( "instance" );
    do {
      objectInstance->Attribute( "x", &positionX );
      objectInstance->Attribute( "y", &positionY );
      AddGameObject( 
        new GameObject( 
          dataPath, 
          static_cast<unsigned int>( positionX ), 
          static_cast<unsigned int>( positionY )
        )
      );
    } while ( objectInstance = objectInstance->NextSiblingElement( "instance" ));
  } while ( objectType = objectType->NextSiblingElement( "game_object" ));
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
  for ( ; gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->UpdateCollision();
    }
  }

  gameObj = m_gameObjects.begin();
  for ( ; gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->UpdateMovement();
    }
  }

  gameObj = m_gameObjects.begin();
  for ( ; gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->UpdateRendering();
    }
  }

  for ( unsigned int i = 0; i < m_toBeDestroyed.size(); i++ ) {
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


GameObject* GameObjectManager::DetectCollision( const GameObject *
  gameObject ) const {
  const sf::FloatRect &mainObj = gameObject->GetCollisionBox();

  for ( GameObjItrConst gameObj = m_gameObjects.begin(); 
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != gameObject && *gameObj != NULL ) {
      const sf::FloatRect &otherObj = ( *gameObj )->GetCollisionBox();
      if( mainObj.Intersects( otherObj ) ) {
        return *gameObj;
      }
    }
  }

  return NULL;
}


GameObject* GameObjectManager::ObjectOnTile( 
  unsigned int x, 
  unsigned int y 
) const {
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


GameObject* GameObjectManager::GetGameObject( const std::string &
  objectType ) const {
  for ( GameObjItrConst gameObj = m_gameObjects.begin(); 
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      if (( *gameObj )->GetType() == objectType ) {
        return *gameObj;
      }
    }
  }
  
  return NULL;
}
