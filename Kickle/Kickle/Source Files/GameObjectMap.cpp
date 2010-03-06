#include "GameObjectMap.h"
#include "BasicTypeDefs.h"
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

  for ( currentEntity = root->FirstChildElement( "entity" ); currentEntity; 
        currentEntity = currentEntity->NextSiblingElement( "entity" ) ) {
    entityData = currentEntity->Attribute( "data" );

    for ( currentEntityInstance = currentEntity->FirstChildElement( "instance" );
      currentEntityInstance ; currentEntityInstance = currentEntityInstance->NextSiblingElement() ) {
        currentEntityInstance->Attribute( "x", &positionX );
        currentEntityInstance->Attribute( "y", &positionY );
        AddGameObject(  new GameObject( entityData, (unsigned int) positionX, (unsigned int) positionY ));
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

  DEBUG_STATEMENT( std::cout << "Adding GameObject: Id[" << nextId << "] x[" <<
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
