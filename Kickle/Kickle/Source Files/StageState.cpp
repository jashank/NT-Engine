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
   m_title( "Stage Selector" ), 
   m_currentStageName( "Stage Name" ),
   m_imageLoaded( false ) {

  //Set text positions
  m_thumbSprite.SetScale( 1.0f, 1.0f );
  m_thumbSprite.Resize( 150, 150 );

  m_title.SetSize( 72 );
  m_title.SetPosition( 190.f, 10.f );
  
  m_currentStageName.SetSize( 50 );
  m_currentStageName.SetPosition( 360.0f, 150.0f );


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
  LoadStages( "Content\\Core\\Stages\\StageRegistery.xml" );

  // Create fonts
  m_font = new sf::Font();
  m_font->LoadFromFile( "Content/Core/Fonts/MICKEY.TTF" );
  
  m_title.SetFont( *m_font);
  m_currentStageName.SetFont( *m_font);

  // Load the first thumbnail
  if ( m_numStages > 0 ) {
      m_currentImage.LoadFromFile( m_thumbnailPaths[0] );
  }

  // Create a sprite, taking the current thumbnail (150,150) and place it
  // on the center of the screen.
  m_thumbSprite = sf::Sprite( m_currentImage );
  m_thumbSprite.SetPosition( 12.0f, 
                             240.0f );

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
  if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Left ) ) {
    if ( m_currentStage > 0  ) {
      m_currentStage--;
    }
  } else if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Right ) ) {
    if ( (m_currentStage+1) < m_numStages ) {
      m_currentStage++;
    }
  } else if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Z ) ) {
    App::GetApp()->SetCurrentStage( m_stagePaths[m_currentStage] );
    App::GetApp()->SetNextState( WorldState::GetInstance() );
  }
}

// Update StageState's frame
void StageState::Update() {
  if ( m_previousStage != m_currentStage ) {
    m_previousStage = m_currentStage;
    m_currentStageName.SetText( m_stageNames[m_currentStage] );
    if ( !m_currentImage.LoadFromFile( m_thumbnailPaths[m_currentStage] ) ) {
      m_imageLoaded = false;
    } else {
      m_imageLoaded = true;
    }
  }

}

// Render StageState's frame
void StageState::Render() {
  App::GetApp()->Draw( m_title );
  App::GetApp()->Draw( m_currentStageName );
  if ( m_imageLoaded ) {
    App::GetApp()->Draw( m_thumbSprite );
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
