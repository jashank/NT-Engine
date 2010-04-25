#include "StageState.h"
#include "App.h"
#include "Configuration.h"

#include "LevelState.h"
#include "WorldState.h"
#include "TitleState.h"

StageState *StageState::m_instance = 0;
const float StageState::BUFFER_SIZE = 20.0f;
const float StageState::OUTLINE_SIZE = 3.0f;
const float StageState::MENU_BUFFER = 300.0f;

StageState::StageState() 
 : m_listbox( 25.0f, 85.0f, 459.0f, 699.0f,
              sf::Color( 0, 0, 0, 0 ), OUTLINE_SIZE, sf::Color( 0, 0, 0, 0 ),
              sf::Color( 119, 255, 255, 100 ), 50.0f ),
   m_infoPanel( (Configuration::GetScreenWidth() + BUFFER_SIZE) / 2,
                BUFFER_SIZE,
                (Configuration::GetScreenWidth() / 2) - 2*BUFFER_SIZE,
                Configuration::GetScreenHeight() - MENU_BUFFER, 
                sf::Color( 20, 80, 200, 100 ), OUTLINE_SIZE, 
                sf::Color::Blue ),
   m_container( 0.0f, 0.0f, (float)Configuration::GetScreenWidth(), 
                (float)Configuration::GetScreenHeight() ),
   m_previousSelection( 0 ) {

  LoadStages( "Content\\Core\\Stages\\StageRegistry.xml" );

  // Init the initial info panel images.
  if (  m_thumbnailPaths.size() > 0 ) {
    m_infoPanel.SetThumbnail( m_thumbnailPaths[ 0 ] );
  }
  if (  m_info.size() > 0 ) {
    m_infoPanel.SetInfo( m_info[0] );
  }

}

StageState::~StageState() {
  CleanUp();
}

StageState* StageState::GetInstance() {
  if( !m_instance ) {
		m_instance = new StageState();
	}
	return m_instance;
}

void StageState::DestroyInstance() {
  SAFEDELETE( m_instance );
}

// Loads the resources required by StageState
void StageState::Init() {
  SetInit( true );
  SetStateData( "Content\\Core\\States\\Stage.xml" );
}

// Releases the resources acquired by StageState
void StageState::CleanUp() {
  SetInit( false );
}

// Pauses the StageState
void StageState::Pause() {
}

// Unpauses the StageState
void StageState::Resume() {
}

// Handle StageState's user input events
void StageState::HandleEvents() {
  if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Escape ) ) {
    App::GetApp()->SetNextState( TitleState::GetInstance() );
  } else if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Z ) ) {
    if ( m_listbox.GetSelection() < m_stagePaths.size() ) {
      App::GetApp()->SetCurrentStage( m_stagePaths[m_listbox.GetSelection()] );
      App::GetApp()->SetNextState( WorldState::GetInstance() );
    }
  }

  m_listbox.HandleEvents();
  m_infoPanel.HandleEvents();
}

// Update StageState's frame
void StageState::Update() {
  m_listbox.Update();
  m_infoPanel.Update();

  if ( m_listbox.GetSelection() != m_previousSelection ) {
    Uint currentSelection = m_listbox.GetSelection();
    m_previousSelection = currentSelection;
    if ( currentSelection < m_thumbnailPaths.size() ) {
      m_infoPanel.SetThumbnail( m_thumbnailPaths[ currentSelection ] );
    }
    if ( currentSelection < m_info.size() ) {
      m_infoPanel.SetInfo( m_info[currentSelection] );
    }
  }

}

// Render StageState's frame
void StageState::Render() {
  m_container.Render();
  m_listbox.Render();
  m_infoPanel.Render();
}

bool StageState::SetStateData( std::string dataPath ) {
  TiXmlDocument doc ( dataPath.c_str() );

  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlElement *root = doc.FirstChildElement( "theme" );
  m_container.SetImage( root->FirstChildElement( "splash" )->Attribute( "path" ) );

  return true;
}

bool StageState::LoadStages( std::string stageRegisteryPath ) {
 TiXmlDocument doc ( stageRegisteryPath.c_str() );

  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlElement* root = doc.FirstChildElement( "stage_registry" );

  TiXmlElement* currentStage;
  std::string stageName;
  std::string stagePath;
  std::string stageThumb;
  std::string stageInfo;


  for ( currentStage = root->FirstChildElement( "stage" ); currentStage; 
        currentStage = currentStage->NextSiblingElement( "stage" ) ) {
    stageName = currentStage->Attribute( "name" );
    stagePath = currentStage->FirstChildElement( "path" )->Attribute( "data" );
    stageThumb = currentStage->FirstChildElement( "thumb")->Attribute( "data" );

    stageInfo = "Author: ";
    stageInfo += currentStage->FirstChildElement( "info" )->Attribute( "author" );
    stageInfo += "\nNumber of Levels: ";
    stageInfo += currentStage->FirstChildElement( "info" )->Attribute( "num_levels" );

    AddStage( stageName, stagePath, stageThumb, stageInfo);
  }
  return true;
}

void StageState::AddStage( std::string stageName, std::string stagePath, 
                           std::string stageThumb, std::string stageInfo ) {
  m_listbox.AddOption( stageName );
  m_info.push_back( stageInfo );
  m_stagePaths.push_back( stagePath );
  m_thumbnailPaths.push_back( stageThumb );
}

