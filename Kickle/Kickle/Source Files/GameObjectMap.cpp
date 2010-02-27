#include "GameObjectMap.h"

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
}

void GameObjectMap::RemoveGameObject( int id ) {
  if ( id < m_nextId && m_gameObjects[id] != 0 ) {
    m_avaliableIds.push_back( id );
    SAFEDELETE( m_gameObjects[id] );
  }
}
