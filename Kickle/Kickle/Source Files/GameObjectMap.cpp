#include <algorithm>
#include <iostream>
#include <queue>

#include "GameObjectMap.h"

#include "Configuration.h"
#include "Utilities.h"

GameObjectMap::GameObjectMap() {}


GameObjectMap::~GameObjectMap() {
  for ( GameObjItr gameObj = m_gameObjects.begin(); 
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      SAFEDELETE( *gameObj );
    }
  }
}


void GameObjectMap::Update() {
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

void GameObjectMap::Render() {
  std::priority_queue< std::pair<float, GameObject*> > renderOrder;

  for ( GameObjItr gameObj = m_gameObjects.begin(); 
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      renderOrder.push( 
      std::pair<float, GameObject*>( -(( *gameObj )->GetPosition().y), *gameObj ) );
    }
  }

  while ( !renderOrder.empty() ) {
    App::GetApp()->Draw( *renderOrder.top().second );
    renderOrder.pop();
  }
}


bool GameObjectMap::SetGameObjectMap( TiXmlElement* root ) {
  TiXmlElement* currentEntity;
  TiXmlElement* currentEntityInstance;

  std::string entityData;
  int positionX;
  int positionY;

  for ( currentEntity = root->FirstChildElement( "entity" ); currentEntity; 
        currentEntity = currentEntity->NextSiblingElement( "entity" ) ) {
    entityData = currentEntity->Attribute( "data" );

    for ( currentEntityInstance = currentEntity->FirstChildElement( "instance" );
      currentEntityInstance ; currentEntityInstance = currentEntityInstance->NextSiblingElement() ) {
        currentEntityInstance->Attribute( "x", &positionX );
        currentEntityInstance->Attribute( "y", &positionY );
        AddGameObject( new GameObject( entityData, (unsigned int)positionX, (unsigned int)positionY ));
    }
  }
  return false;
}


void GameObjectMap::AddGameObject( GameObject *gameObject ) {
  m_gameObjects.push_back( gameObject );
  gameObject->Play();
}


void GameObjectMap::RemoveGameObject( GameObject *gameObject ) {
  if ( std::find( m_toBeDestroyed.begin(), m_toBeDestroyed.end(), gameObject ) == 
    m_toBeDestroyed.end() ) {
    m_toBeDestroyed.push_back( gameObject );
  }
}


GameObject* GameObjectMap::DetectCollision( const GameObject *gameObject ) {
  const sf::FloatRect &mainObj = gameObject->GetCollisionBox();

  for ( GameObjItr gameObj = m_gameObjects.begin(); 
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


GameObject* GameObjectMap::ObjectOnTile( unsigned int x, unsigned int y ) {
  for ( GameObjItr gameObj = m_gameObjects.begin(); 
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


GameObject* GameObjectMap::GetGameObject( const std::string &objectType ) {
  for ( GameObjItr gameObj = m_gameObjects.begin(); 
        gameObj != m_gameObjects.end(); gameObj++ ) {
    if ( *gameObj != NULL ) {
      if (( *gameObj )->GetType() == objectType ) {
        return *gameObj;
      }
    }
  }
  
  return NULL;
}
