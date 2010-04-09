#include "WorldState.h"
#include "Utilities.h"
#include "LevelState.h"

WorldState *WorldState::m_instance = 0;

WorldState::WorldState()
 : m_numLevels(0),
   m_currentLevel(-1),
   m_nextLevel( 0 ){
}

WorldState::~WorldState() {
  CleanUp();
}

WorldState* WorldState::GetInstance() {
  if( !m_instance ) {
		m_instance = new WorldState();
	}
	return m_instance;
}

void WorldState::DestroyInstance() {
  SAFEDELETE( m_instance );
}

// Loads the resources required by WorldState
void WorldState::Init() {
  SetInit( true );
  LoadStage( App::GetApp()->GetCurrentStage() );
}

// Releases the resources acquired by WorldState
void WorldState::CleanUp() {
  SetInit( false );
}

// Pauses the WorldState
void WorldState::Pause() {
}

// Unpauses the WorldState
void WorldState::Resume() {
}

// Handle WorldState's user input events
void WorldState::HandleEvents() {
}

// Update WorldState's frame
void WorldState::Update() {
  if ( m_currentLevel < m_nextLevel ) {
    if ( m_nextLevel >= m_numLevels) {
      CleanUp(); // This need to "pop" back the previous state, the StageState
    } else {
      m_currentLevel = m_nextLevel;
      App::GetApp()->SetCurrentLevel( m_levelPaths[m_currentLevel] );
      App::GetApp()->SetNextState( LevelState::GetInstance() );
      // After the state completes it should return here...
      m_nextLevel++;
    }
  }
}

// Render WorldState's frame
void WorldState::Render() {
}

bool WorldState::LoadStage( std::string currentStage ) {
 TiXmlDocument doc ( currentStage.c_str() );

  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlElement* root = doc.FirstChildElement( "stage" );
  TiXmlElement* levelPlaylist = root->FirstChildElement( "level_playlist" );
  TiXmlElement* currentLevel;

  std::string levelPath;

  for ( currentLevel = levelPlaylist->FirstChildElement( "level" ); currentLevel; 
        currentLevel = currentLevel->NextSiblingElement( "level" ) ) {
    levelPath = currentLevel->Attribute( "path" );
    AddLevel( levelPath );
  }
  return true;
}

void WorldState::AddLevel( std::string levelPath ) {
  m_levelPaths.push_back( levelPath );
  m_numLevels++;
}
