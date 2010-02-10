#ifndef APP_H
#define APP_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "ResourceManager.h"
#include "StateManager.h"

class StateManager;

//Template specialization to handle sf::Music's OpenFromFile() 
//instead of the typical LoadFromFile()
template<>
sf::Music* ResourceLoader<sf::Music>::Load( const std::string& filename );


/************************************************
	App
	-Initializes SFML
	-Initializes ResoureManagers
	-Initializes StateManager
	-Calculates DeltaTime
	-Calculates FPS
	-Polls for events
	-Loads Resources
*************************************************/
class App {
public:
	static App* CreateApp( const std::string &title, 
		unsigned int width, 
		unsigned int height, 
		unsigned int framerate );
	static App* GetApp();
	static void DestroyApp();
	~App();
	
	void Run();

	void Draw( const sf::Drawable &object );

	sf::Image& LoadImage( const std::string &filename );
	sf::SoundBuffer& LoadSound( const std::string &filename );
	sf::Music& LoadMusic( const std::string &filename );

	float GetDeltaTime() const;

private:
	App( const std::string &title, 
		unsigned int width, 
		unsigned int height, 
		unsigned int framerate );
	App( const App& app );

	static App*			m_instance;

	sf::RenderWindow	m_window;
	sf::Event			m_event;

	char				m_fpsStrBuff[50];
	float				m_deltaTime;
	float				m_fps;

	//Resource Managers
	ResourceManager< sf::Image >		m_images;
	ResourceManager< sf::SoundBuffer >	m_sounds;
	ResourceManager< sf::Music >		m_music;

	StateManager						m_stateManager;

};

#endif