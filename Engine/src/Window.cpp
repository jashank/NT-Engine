#include "Window.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Input.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Config.h"
#include "Lerpable.h"
#include "Utilities.h"

// Holds actual window
namespace {
  sf::RenderWindow win;
} 


namespace nt {
namespace window {

  void Create( 
    std::string &title, 
    int width, 
    int height 
  ) {
    win.Create(sf::VideoMode( width, height ), title );
  }


  void Close() {
    win.Close();
  }


  void Clear() {
    win.Clear();
  }


  void Draw( nt::Lerpable &toDraw, float alpha ) {
    toDraw.Lerp( alpha );
    win.Draw( toDraw );
    toDraw.Unlerp();
  }


  void Display() {
    win.Display();
  }


  bool IsOpen() {
    return win.IsOpened();
  }


  bool GetEvent( sf::Event &event ) {
    return win.GetEvent( event );
  }


  const sf::Input &GetInput() {
    return win.GetInput();
  }


  void OffsetView( float x, float y ) {
    win.GetDefaultView().Move( x, y );
  }


  void ResetView() {
    sf::FloatRect viewRect = win.GetDefaultView().GetRect();
    int offX = -viewRect.Left;
    int offY = -viewRect.Top;
    OffsetView( offX, offY );
  }

} // namespace window
} // namespace nt

