#ifndef CONTAINER_H
#define CONTAINER_H

#include "Control.h"
#include "App.h"
#include "Configuration.h"

class Container: public Control {
 public:
  Container();
  Container( float x, float y, float width, float height );
  virtual ~Container();

  virtual void Render();
  virtual void Update();
  virtual void HandleEvents();

  void SetBaseColor( sf::Color color );
  void SetOutlineColor( sf::Color color );
  void SetOutlineSize( float outline );

  void SetImage( std::string imagePath );

  void AddController( Control *controller );
  void SetTheme( std::string themePath );
 protected:
   float m_outline;
   sf::Color m_baseColor;
   sf::Color m_outlineColor;

   sf::Image m_image; // Needed for the sprite.
   sf::Sprite m_sprite; // This allows you to draw as a sprite.

};

#endif //CONTAINER_H