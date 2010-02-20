#ifndef BUTTON_H
#define BUTTON_H

#include <boost/function.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/String.hpp>
#include <SFML/System/Vector2.hpp>
class sf::Image;
class sf::RenderTarget;



/********************************************************
Button
- Performs action when clicked by user.
********************************************************/
class Button : public sf::Sprite {
 public:
  /********************************************************
  Typedefs
  - Boost function pointer typedef'd to buttonAction.
  ********************************************************/
  typedef boost::function< void() > ButtonAction;

  /********************************************************
  Constructors
  - All constructors take a buttonAction, an sf::vector2<float>,
    and an sf::IntRect
  - Separate constructors for just an sf::string, and an 
    string with an image.
  ********************************************************/
  Button( 
    const ButtonAction &action, 
    const sf::Vector2f &position,
    const sf::IntRect &clip,
    const sf::String &text
  );

  Button(
    const ButtonAction &action,
    const sf::Vector2f &position,
    const sf::IntRect &clip,
    const sf::String &text,
    const sf::Image &image
  );

  /********************************************************
  ContainsCursor
  - Checks to see if cursor is positioned over button.
  ********************************************************/
  bool ContainsCursor( int cursorX, int cursorY ) const;

  /********************************************************
  Activate
  - Activates the button's action.
  ********************************************************/
  void Activate();

 protected:
  /********************************************************
  Render
  - Renders the button to the screen.
  ********************************************************/
  virtual void Render( sf::RenderTarget &Target ) const;

 private:
  ButtonAction m_action; // Action button will perform
  sf::String m_text; // Text on the button
};
  
#endif
  


  

    
