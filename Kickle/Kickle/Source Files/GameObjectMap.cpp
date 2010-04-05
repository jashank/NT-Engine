#include <iostream>
#include <queue>
#include <algorithm>

#include "GameObjectMap.h"

#include "BasicTypeDefs.h"
#include "Configuration.h"
#include "Utilities.h"

GameObjectMap::GameObjectMap() {
  Init();
}


GameObjectMap::~GameObjectMap() {
  for ( int i = 0; i < m_nextId; i++ ) {
    if ( m_gameObjects[i] != 0 ) {
      SAFEDELETE( m_gameObjects[i] );
    }
  }
}


void GameObjectMap::Update() {
  for ( int i = 0; i < m_nextId; i++ ) {
    if ( m_gameObjects[i] != 0 ) {
      m_gameObjects[i]->UpdateCollision();
    }
  }
  for ( int i = 0; i < m_nextId; i++ ) {
    if ( m_gameObjects[i] != 0 ) {
      m_gameObjects[i]->UpdateMovement();
    }
  }
  for ( int i = 0; i < m_nextId; i++ ) {
    if ( m_gameObjects[i] != 0 ) {
      m_gameObjects[i]->UpdateRendering();
    }
  }
  for ( Uint i = 0; i < m_toBeDestroyed.size(); i++ ) {
    CleanUpGameObject( m_toBeDestroyed[i] );
  }
  m_toBeDestroyed.clear();
}

void GameObjectMap::Render() {
  std::priority_queue< std::pair<float, GameObject*> > renderOrder;
  for ( int i = 0; i < m_nextId; i++ ) {
    if ( m_gameObjects[i] != 0 ) {
      renderOrder.push( 
      std::pair<float, GameObject*>( -(m_gameObjects[i]->GetPosition().y), m_gameObjects[i] ) );
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
        AddGameObject( new GameObject( entityData, (Uint)positionX, (Uint)positionY ));
    }
  }
  return false;
}


void GameObjectMap::AddGameObject( GameObject *gameObject ) {
  int nextId;

  if ( !m_avaliableIds.empty() ) {
    nextId = m_avaliableIds.back();
    m_avaliableIds.pop_back();
  } else {
    nextId = m_nextId;
    m_nextId++;
  }

  m_gameObjects[nextId] = gameObject;
  gameObject->SetId( nextId );
 
  gameObject->Play();
}


void GameObjectMap::RemoveGameObject( GameObject *gameObject ) {
  m_toBeDestroyed.push_back( gameObject );
}


GameObject* GameObjectMap::DetectCollision( const GameObject *gameObject ) {
  const sf::FloatRect &mainObj = gameObject->GetCollisionBox();

  for ( int i = 0; i < m_nextId; ++i ) {
    if ( m_gameObjects[i] != gameObject && m_gameObjects[i] != 0 ) {
      const sf::FloatRect &otherObj = m_gameObjects[i]->GetCollisionBox();
      if( mainObj.Intersects( otherObj ) ) {
        return m_gameObjects[i];
      }
    }
  }

  return NULL;
}


GameObject* GameObjectMap::ObjectOnTile( Uint x, Uint y ) {
  for ( int i = 0; i < m_nextId; ++i ) {
    if ( m_gameObjects[i] != 0 ) {
      if ( m_gameObjects[i]->GetTileX() == x &&
          m_gameObjects[i]->GetTileY() == y ) {
        return m_gameObjects[i];
      }
    }
  }

  return NULL;
}


GameObject* GameObjectMap::GetGameObject( const std::string &objectType ) {
  for ( int i = 0; i < m_nextId; ++i ) {
    if ( m_gameObjects[i]->GetType() == objectType ) {
      return m_gameObjects[i];
    }
  }
  
  return NULL;
}


/******************************************
Private Methods
*******************************************/
void GameObjectMap::Init() {
  m_nextId = 0;
}


void GameObjectMap::CleanUpGameObject( GameObject *gameObject ) {
  int id = gameObject->GetId();
  if ( id < m_nextId && m_gameObjects[id] != 0 ) {
    m_avaliableIds.push_back( id );
    SAFEDELETE( m_gameObjects[id] );
  }
}