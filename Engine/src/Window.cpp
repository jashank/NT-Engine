#include "Window.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Input.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Utilities.h"

// Holds actual window
namespace {
  sf::RenderWindow *win = NULL;
} 


namespace nt {
namespace window {

  void Create( 
    std::string &title, 
    int width, 
    int height 
  ) {
    win = new sf::RenderWindow( sf::VideoMode( width, height ), title );
  }


  void Close() {
    win->Close();
  }


  void Destroy() {
    SAFEDELETE( win );
  }

  void Clear() {
    win->Clear();
  }

  void Display() {
    win->Display();
  }

  void Draw( const sf::Drawable &object ) {
    win->Draw( object );
  }


  bool IsOpen() {
    return win->IsOpened();
  }


  bool GetEvent( sf::Event &event ) {
    return win->GetEvent( event );
  }


  const sf::Input &GetInput() {
    return win->GetInput();
  }


  void OffsetView( float x, float y ) {
    win->GetDefaultView().Move( x, y );
  }

} // namespace window
} // namespace nt

