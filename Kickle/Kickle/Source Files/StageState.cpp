#include "StageState.h"
#include "App.h"
#include "Configuration.h"

#include "LevelState.h"
#include "WorldState.h"

StageState *StageState::m_instance = 0;

StageState::StageState()
 : m_currentStage( 0 ),
   m_previousStage( -1 ),
   m_numStages( 0 ),
   m_font( NULL ),  
   m_currentStageName( "Stage Name" ),
   m_currentSelection( 0 ),
   m_imageLoaded( false ) {
  LoadStages( "Content\\Core\\Stages\\StageRegistery.xml" );
  
  m_listPanel = sf::Shape::Rectangle( 10.0f, 10.0f, 512.0f,
                       750.0f, sf::Color::Blue,3.0f, 
                       sf::Color::Red );

  m_infoPanel = sf::Shape::Rectangle( 522.0f, 10.0f, 1014.0f,
                       500.0f, sf::Color::Blue, 3.0f, 
                       sf::Color::Red);
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
  m_font = new sf::Font();
  m_font->LoadFromFile( "Content/Core/Fonts/MICKEY.TTF" );
  m_currentStageName.SetFont( *m_font );
}

// Releases the resources acquired by StageState
void StageState::CleanUp() {
  SetInit( false );
  SAFEDELETE( m_font );
}

// Pauses the StageState
void StageState::Pause() {
}

// Unpauses the StageState
void StageState::Resume() {
}

// Handle StageState's user input events
void StageState::HandleEvents() {
  // I will be cleaning this up shortly once I finish the specifications.
  if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Up ) ) {
    if ( m_currentSelection > 0 ) {
      m_currentSelection--;
    } else if ( m_currentSelection != m_currentStage ) {
    }
  } else if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Down ) ) {
    if ( m_currentSelection < 14 ) {
      m_currentSelection++;
    } else if ( m_currentSelection != m_currentStage ) {
    
    }
  } else if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Z ) ) {
    if  ( m_currentSelection < m_numStages ) {
      App::GetApp()->SetCurrentStage( m_stagePaths[m_currentSelection] );
      App::GetApp()->SetNextState( WorldState::GetInstance() );
    }
  }

  // This is currently the best way for the engine to make a list box that
  // is clickable with the mouse.
  if ( App::GetApp()->GetInput().IsMouseButtonDown( sf::Mouse::Left )) {
    float mouseXPos = (float)App::GetApp()->GetInput().GetMouseX();
    if ( mouseXPos >= 10.0f && mouseXPos <= 512.0f ) {
      float mouseYPos = (float)App::GetApp()->GetInput().GetMouseY();
      if ( mouseYPos >=10.0f && mouseYPos < 750.0f ) {
        m_currentSelection = (int)(mouseYPos/50.0f);
      }
    }
  }

  
}

// Update StageState's frame
void StageState::Update() {
}

// Render StageState's frame
void StageState::Render() {

  // Draw the left rectangle scroll bar
  App::GetApp()->Draw( m_listPanel );

  // Draw the right rectangle info panel
  App::GetApp()->Draw( m_infoPanel );

  // Draw the selector.
  App::GetApp()->Draw( sf::Shape::Rectangle( 10.0f,
                       (50.0f*m_currentSelection+10.0f), 512.0f,
                       (50.0f*m_currentSelection+50.0f),
                        sf::Color::Green ) );

  // Draw the selections
  for ( int i = 0; i < m_numStages; i++ ) {
    m_currentStageName.SetText( m_stageNames[i] );
    m_currentStageName.SetPosition( 10.0f, (i*50.0f + 10) );
    App::GetApp()->Draw( m_currentStageName );
  }

}

bool StageState::LoadStages( std::string stageRegisteryPath ) {
 TiXmlDocument doc ( stageRegisteryPath.c_str() );

  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlElement* root = doc.FirstChildElement( "stage_registery" );

  TiXmlElement* currentStage;
  std::string stageName;
  std::string stagePath;
  std::string stageThumb;


  for ( currentStage = root->FirstChildElement( "stage" ); currentStage; 
        currentStage = currentStage->NextSiblingElement( "stage" ) ) {
    stageName = currentStage->Attribute( "name" );
    stagePath = currentStage->Attribute( "path" );
    stageThumb = currentStage->Attribute( "thumb" );
    AddStage( stageName, stagePath, stageThumb );
  }
  return true;
}

void StageState::AddStage( std::string stageName, std::string stagePath, 
                           std::string stageThumb ) {
  m_stageNames.push_back( stageName );
  m_stagePaths.push_back( stagePath );
  m_thumbnailPaths.push_back( stageThumb );
  m_numStages++;
}
