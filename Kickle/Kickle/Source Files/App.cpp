#include <SFML/Graphics.hpp>
#include <SFML/Window/Input.hpp>

#include "App.h"
#include "GameObject.h"
#include "LevelState.h"
#include "TitleState.h"
#include "Utilities.h"



//Template specialization to handle sf::Music's OpenFromFile() 
//instead of the typical LoadFromFile()
template<>
sf::Music* ResourceLoader< sf::Music >::Load( const std::string &filename ) {
	std::auto_ptr< sf::Music > resource( new sf::Music() );
	if( !resource->OpenFromFile( filename ) ) {
		std::string err = "Unable to Load() resource: ";
		err += filename;

		throw err.c_str();
	}
	return resource.release();
}

/************************************************
Data Members
************************************************/
App* App::m_instance = 0;


/************************************************
Constructor and Destructor
************************************************/
App::App( 
  const std::string &title, 
  Uint width, 
  Uint height, 
  Uint framerate 
) 
 : m_window( sf::VideoMode( width, height ), title ), 
   m_time( 0.0f ),
   m_deltaTime( 0.0f ), 
   m_fps( 0.0f ) {
	m_window.SetFramerateLimit( framerate );
  m_keyManager.Init( m_window.GetInput() );
}


App::~App() {
	DEBUG_STATEMENT( std::cout << "Closing App..." << std::endl; )

  LevelState::DestroyInstance();
	TitleState::DestroyInstance();
}


/************************************************
Public Member Functions
************************************************/
App* App::CreateApp( 
  const std::string& title, 
  Uint width, 
  Uint height, 
  Uint framerate 
) {
	if(  m_instance == 0 ) {
		DEBUG_STATEMENT( 
      DebugMsg( " Creating App...\n-->Title: %s \n-->Width: %d  \
                  \n-->Height: %d \n-->Framerate: %d \n" ,
                  title.c_str(), width, height, framerate );
		)
		m_instance = new App( title, width, height, framerate );
	}
	else {
		throw "Illegal call to App::CreateApp(): App already exists";
	}

	return m_instance;
}


void App::DestroyApp()
{
	SAFEDELETE( m_instance );
}


void App::Draw( const sf::Drawable &object ) {
	m_window.Draw( object );
}


App* App::GetApp() {
	if( m_instance != 0 ) {
		return m_instance;
	}
	else {
		throw "Illegal call to App::GetApp(): App doesn't exist";
	}
}

float App::GetTime() const {
  return m_time;
}

float App::GetDeltaTime() const {
	return m_deltaTime;
}


Key App::GetKeyTime( sf::Key::Code key ) const {
  return m_keyManager.GetKeyTime( key );
}


const sf::Event& App::GetEvent() const  {
  return m_event;
}


const sf::Input& App::GetInput() const {
  return m_window.GetInput();
}


sf::Image& App::LoadImage( const std::string &filename ) {
	return m_images.Load( filename );
}


sf::SoundBuffer& App::LoadSound( const std::string &filename ) {
	return m_sounds.Load( filename );
}


sf::Music& App::LoadMusic( const std::string &filename ) {
	return m_music.Load( filename );
}


AnimData& App::LoadAnim( const std::string &filename ) {
	return m_anims.Load( filename );
}

void App::RegisterKey( sf::Key::Code key ) {
  m_keyManager.RegisterKey( key );
}

void App::Run() {
	//Set the Initial State for the statemanager
	m_stateManager.SetNextState( TitleState::GetInstance() );

	//Game Loop
	while ( m_window.IsOpened() ) {

		if ( m_window.GetEvent( m_event ) ) {
			//Handle Close Event
			if ( m_event.Type == sf::Event::Closed ) {
				m_window.Close();		
			}
		}

    m_keyManager.Update();

		m_stateManager.RefreshState();
		m_stateManager->HandleEvents();
		m_stateManager->Update();

		m_window.Clear( m_clearColor );
		//Display FPS
		sprintf_s( m_fpsStrBuff, 50, "FPS: %4.2g", m_fps );
		m_window.Draw( sf::String( m_fpsStrBuff )  );

		//Render and Display
		m_stateManager->Render();
		m_window.Display();
		
		//Get the time it took to render the last frame
		m_deltaTime = m_window.GetFrameTime();
    m_time += m_deltaTime;
		m_fps = 1.0f/m_deltaTime;
	}
}


void App::SetClearColor( const sf::Color& color ) {
  m_clearColor = color;
}


void App::SetNextState( BaseState *state ) {
	m_stateManager.SetNextState( state );
}


std::string App::GetCurrentStage() {
  return m_currentStage;
}

void App::SetCurrentStage( std::string newStage) {
  m_currentStage = newStage;
}

std::string App::GetCurrentLevel() {
  return m_currentLevel;
}

void App::SetCurrentLevel( std::string newLevel ) {
  m_currentLevel = newLevel;
}