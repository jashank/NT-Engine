#include <SFML/Graphics.hpp>

#include "App.h"
#include "TitleState.h"
#include "Utilities.h"


//Template specialization to handle sf::Music's OpenFromFile() 
//instead of the typical LoadFromFile()
template<>
sf::Music* ResourceLoader<sf::Music>::Load( const std::string &filename ) {
	std::auto_ptr<sf::Music> resource( new sf::Music() );
	if( !resource->OpenFromFile( filename ) ) {
		std::string err = "Unable to Load() resource: ";
		err += filename;

		throw err.c_str();
	}
	return resource.release();
}


App* App::m_instance = 0;

App::App( const std::string &title, 
		 unsigned int width, 
		 unsigned int height, 
		 unsigned int framerate ) 
 : m_window( sf::VideoMode(width,height), title ), 
   m_deltaTime(0.0f), 
   m_fps(0.0f) ,
   m_stateManager( TitleState::GetInstance() ) {
	m_window.SetFramerateLimit( framerate );
}

App::~App() {
	DEBUG_STATEMENT( std::cout << "Closing App..." << std::endl; )
	TitleState::DestroyInstance();
}

App* App::CreateApp( const std::string& title, 
					unsigned int width, 
					unsigned int height, 
					unsigned int framerate ) {

	if(  m_instance == 0 ) {
		DEBUG_STATEMENT( 
			std::cout 
			<< "Creating App...\n-->Title: " << title
			<< "\n-->Width: " << width
			<< "\n-->Height: " << height
			<< "\n-->Framerate: " << framerate 
			<< std::endl; 
		)
		m_instance = new App( title, width, height, framerate );
	}
	else {
		throw "Illegal call to App::CreateApp(): App already exists";
	}

	return m_instance;
}

App* App::GetApp() {
	if( m_instance != 0 ) {
		return m_instance;
	}
	else {
		throw "Illegal call to App::GetApp(): App doesn't exist";
	}
}

void App::DestroyApp()
{
	SAFEDELETE( m_instance );
}

void App::Run() {
	while ( m_window.IsOpened() ) {
		if ( m_window.GetEvent( m_event ) ) {
			if ( m_event.Type == sf::Event::Closed ) {
				m_window.Close();		
			}
		}

		m_stateManager.HandleStateChange();

		m_stateManager->HandleEvents();
		m_stateManager->Update();

		m_window.Clear();

		//Display FPS
		sprintf( m_fpsStrBuff, "FPS: %4.2g", m_fps );
		m_window.Draw( sf::String( m_fpsStrBuff )  );

		//Render and Display
		m_stateManager->Render();
		m_window.Display();

		//Get the time it took to render the last frame and calculate the fps
		m_deltaTime = m_window.GetFrameTime();
		m_fps = 1.0f/m_deltaTime;
	}
}

void App::Draw( const sf::Drawable &object ) {
	m_window.Draw(object);
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

float App::GetDeltaTime() const {
	return m_deltaTime;
}