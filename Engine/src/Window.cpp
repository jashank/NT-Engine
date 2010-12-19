/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

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

