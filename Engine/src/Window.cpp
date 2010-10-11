#include "Window.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>


// Holds actual window
namespace {
  sf::RenderWindow *window = NULL;
} 


namespace nt {
namespace window {

  void CreateWindow( 
    std::string &title, 
    int width, 
    int height, 
    int maxFps
  ) {
    window = new Window( sf::VideoMode( width, height ), title );
    window.SetFramerateLimit( framerate );
  }


  void DestroyWindow() {
    delete window;
  }


  void Refresh() {
    window->Clear();
    window->Display();
  }


  void Draw( const sf::Drawable &object ) {
    window->Draw( object );
  }


  bool IsOpen() {
    return window->IsOpened();
  }


  const sf::Event &GetEvent() {
    static sf::Event event;
    window->GetEvent( event );
    return event;
  }


  const sf::Input &GetInput() {
    return window->GetInput();
  }


} // namespace window
} // namespace nt


