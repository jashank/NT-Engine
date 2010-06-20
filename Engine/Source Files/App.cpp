#include "App.h"

#include <SFML/Window/Input.hpp>

#include "GameObject.h"
#include "GameState.h"
#include "Utilities.h"

//Template specialization to handle sf::Music's OpenFromFile()
//instead of the typical LoadFromFile()
template<>
sf::Music* ResourceLoader< sf::Music >::Load( const std::string &filename ) {
	std::auto_ptr< sf::Music > resource( new sf::Music() );
	if( !resource->OpenFromFile( filename ) ) {
    LogErr( "Song in" + filename + "failed to load." );
    return NULL;
	}
	return resource.release();
}

/************************************************
Data Members
************************************************/
App* App::m_instance = NULL;


/************************************************
Constructor and Destructor
************************************************/
App::App(
  const std::string &title,
  unsigned int width,
  unsigned int height,
  unsigned int framerate
)
 : m_time( 0.0f ),
   m_deltaTime( 0.0f ),
   m_fps( 0.0f ),
   m_window( sf::VideoMode( width, height ), title ),
   m_currentState( NULL ) {
  m_window.UseVerticalSync( true );
  m_window.SetFramerateLimit( framerate );
  m_keyManager.Init( m_window.GetInput() );
}


App::~App() {
  DEBUG_STATEMENT( std::cout << "Closing App..." << std::endl; )
}


/************************************************
Public Member Functions
************************************************/
App* App::CreateApp(
  const std::string& title,
  unsigned int width,
  unsigned int height,
  unsigned int framerate
) {
	if(  m_instance == 0 ) {
		m_instance = new App( title, width, height, framerate );
	}
	else {
		LogErr( "Attempt to call CreateApp when App already exiss." );
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
  LogErr( "Illegal call to App::GetApp(). App does not yet exist." );
  return NULL;
}


GameState* App::GetCurrentState() const {
  return m_currentState;
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


sf::Image* App::LoadImage( const std::string &filename ) {
	return m_images.Load( filename );
}


sf::SoundBuffer* App::LoadSound( const std::string &filename ) {
	return m_sounds.Load( filename );
}


sf::Music* App::LoadMusic( const std::string &filename ) {
	return m_music.Load( filename );
}


AnimData* App::LoadAnim( const std::string &filename ) {
	return m_anims.Load( filename );
}

void App::RegisterKey( sf::Key::Code key ) {
  m_keyManager.RegisterKey( key );
}

void App::Run() {
  // TEMPORARY
  m_currentState = new GameState();
  m_currentState->LoadFromFile( "Kickle_Pack/States/test_state.xml" );

	//Game Loop
	while ( m_window.IsOpened() ) {

		if ( m_window.GetEvent( m_event ) ) {
			//Handle Close Event
			if ( m_event.Type == sf::Event::Closed ) {
				m_window.Close();
			}
		}

    m_keyManager.Update();

		m_currentState->HandleEvents();
		m_currentState->Update();

		m_window.Clear( m_clearColor );
		//Display FPS
		sprintf( m_fpsStrBuff, "FPS: %4.2g", m_fps );
		m_window.Draw( sf::String( m_fpsStrBuff )  );

		//Render and Display
		m_currentState->Render();
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