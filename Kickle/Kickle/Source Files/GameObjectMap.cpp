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


void GameObjectMap::Init() {
  m_nextId = 0;
  for ( int i = 0; i < MAP_SIZE; i++ ) {
    for ( int j = 0; j < MAP_SIZE; j++ ) {
      m_gameObjectLayout[i][j] = NULL_GAME_OBJECT;
    }
  }
}


void GameObjectMap::Update() {
  for ( int i = 0; i < m_nextId; i++ ) {
    if ( m_gameObjects[i] != 0 ) {
      m_gameObjects[i]->Update();
    }
  }
}


void GameObjectMap::Render() {
  for ( int i = 0; i < m_nextId; i++ ) {
    if ( m_gameObjects[i] != 0 ) {
    App::GetApp()->Draw( *m_gameObjects[i] );
    }
  }
}


bool GameObjectMap::SetGameObjectMap( TiXmlElement* root ) {
  TiXmlElement* currentEntity;
  TiXmlElement* currentEntityInstance;

  std::string entityData;
  int positionX;
  int positionY;
  std::string entityType;

  for ( currentEntity = root->FirstChildElement( "entity" ); currentEntity; 
        currentEntity = currentEntity->NextSiblingElement( "entity" ) ) {
    entityData = currentEntity->Attribute( "data" );

    for ( currentEntityInstance = currentEntity->FirstChildElement( "instance" );
      currentEntityInstance ; currentEntityInstance = currentEntityInstance->NextSiblingElement() ) {
        currentEntityInstance->Attribute( "x", &positionX );
        currentEntityInstance->Attribute( "y", &positionY );
        entityType = currentEntityInstance->Attribute( "type" );
        AddGameObject( 
          new GameObject( 
            entityData, 
            (Uint) positionX, 
            (Uint) positionY,
            entityType 
          )
      );
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

  DEBUG_STATEMENT( std::cout << "Adding GameObject: id[" << nextId << "] x[" <<
                   gameObject->GetTileX() << "] y[" << gameObject->GetTileY() 
                   << "]" << std::endl; );

   m_gameObjects[nextId] = gameObject;
   gameObject->SetId( nextId );
 

   gameObject->Play();
}


void GameObjectMap::RemoveGameObject( GameObject *gameObject ) {
  int id = gameObject->GetId();
  if ( id < m_nextId && m_gameObjects[id] != 0 ) {
    m_avaliableIds.push_back( id );
    SAFEDELETE( m_gameObjects[id] );
  }
}


GameObject *GameObjectMap::GetGameObject( Uint x, Uint y ) const {
  if ( Configuration::IsTileValid( x, y ) ) {
    DEBUG_STATEMENT( std::cout << x << " " << y << " " << 
                     m_gameObjectLayout[y][x] << std::endl; );

    if ( m_gameObjects.find( m_gameObjectLayout[y][x] ) != m_gameObjects.end() ) {
      return m_gameObjects.find( m_gameObjectLayout[y][x] )->second;
    }
    return NULL;
  }
  return NULL;
}

void GameObjectMap::UpdatePosition( GameObject* gameObject,  Uint x, Uint y ) {
  if ( Configuration::IsTileValid( x, y ) ) {
    m_gameObjectLayout[gameObject->GetTileY()][gameObject->GetTileX()] = NULL_GAME_OBJECT;
    m_gameObjectLayout[y][x] = gameObject->GetId();
  }
}