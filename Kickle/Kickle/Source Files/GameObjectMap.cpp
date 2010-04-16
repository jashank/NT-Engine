#include <iostream>
#include <queue>
#include <algorithm>

#include "GameObjectMap.h"

#include "BasicTypeDefs.h"
#include "Configuration.h"
#include "Utilities.h"

GameObjectMap::GameObjectMap() {}


GameObjectMap::~GameObjectMap() {
  GameObjItr gameObj = m_gameObjects.begin();
  while ( gameObj != m_gameObjects.end() ) {
    if ( *gameObj != NULL ) {
      SAFEDELETE( *gameObj );
    }
    ++gameObj;
  }
}


void GameObjectMap::Update() {
  GameObjItr gameObj = m_gameObjects.begin();
  while( gameObj != m_gameObjects.end() ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->UpdateCollision();
    }
    ++gameObj;
  }

  gameObj = m_gameObjects.begin();
  while( gameObj != m_gameObjects.end() ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->UpdateMovement();
    }
    ++gameObj;
  }

  gameObj = m_gameObjects.begin();
  while ( gameObj != m_gameObjects.end() ) {
    if ( *gameObj != NULL ) {
      ( *gameObj )->UpdateRendering();
    }
    ++gameObj;
  }

  for ( Uint i = 0; i < m_toBeDestroyed.size(); i++ ) {
    m_gameObjects.remove( m_toBeDestroyed[i] );
    SAFEDELETE( m_toBeDestroyed[i] );
  }
  m_toBeDestroyed.clear();
}

void GameObjectMap::Render() {
  std::priority_queue< std::pair<float, GameObject*> > renderOrder;

  GameObjItr gameObj = m_gameObjects.begin();
  while( gameObj != m_gameObjects.end() ) {
    if ( *gameObj != NULL ) {
      renderOrder.push( 
      std::pair<float, GameObject*>( -(( *gameObj )->GetPosition().y), *gameObj ) );
    }
    ++gameObj;
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
        AddGameObject( new GameObject( entityData, (Uint)positionX, (Uint)positionY ));
    }
  }
  return false;
}


void GameObjectMap::AddGameObject( GameObject *gameObject ) {
  m_gameObjects.push_back( gameObject );
  gameObject->Play();
}


void GameObjectMap::RemoveGameObject( GameObject *gameObject ) {
  m_toBeDestroyed.push_back( gameObject );
}


GameObject* GameObjectMap::DetectCollision( const GameObject *gameObject ) {
  const sf::FloatRect &mainObj = gameObject->GetCollisionBox();

  GameObjItr gameObj = m_gameObjects.begin();
  while ( gameObj != m_gameObjects.end() ) {
    if ( *gameObj != gameObject && *gameObj != NULL ) {
      const sf::FloatRect &otherObj = ( *gameObj )->GetCollisionBox();
      if( mainObj.Intersects( otherObj ) ) {
        return *gameObj;
      }
    }
    ++gameObj;
  }

  return NULL;
}


GameObject* GameObjectMap::ObjectOnTile( Uint x, Uint y ) {
  GameObjItr gameObj = m_gameObjects.begin();
  while ( gameObj != m_gameObjects.end() ) {
    if ( *gameObj != NULL ) {
      if (( *gameObj )->GetTileX() == x &&
          ( *gameObj )->GetTileY() == y ) {
        return *gameObj;
      }
    }
    ++gameObj;
  }

  return NULL;
}


GameObject* GameObjectMap::GetGameObject( const std::string &objectType ) {
  GameObjItr gameObj = m_gameObjects.begin();
  while( gameObj != m_gameObjects.end() ) {
    if ( *gameObj != NULL ) {
      if (( *gameObj )->GetType() == objectType ) {
        return *gameObj;
      }
    }
    ++gameObj;
  }
  
  return NULL;
}