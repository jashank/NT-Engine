#ifndef CONTROL_H
#define CONTROL_H

#include <SFML/Graphics.hpp>
#include "BasicTypeDefs.h"

class Control {
 public:
  Control();
  virtual ~Control();

  virtual void Render();
  virtual void Update();
  virtual void HandleEvents();

  virtual float GetX();
  virtual float GetY();

  virtual float GetWidth();
  virtual float GetHeight();

  virtual void SetPosition( float x, float y );
  virtual void SetX( float x );
  virtual void SetY( float y );
  
  virtual void SetWidth( float width );
  virtual void SetHeight( float height );
 protected:
  void SetId( Uint id );
  Uint GetId();

  sf::Rect< float > m_rect;
  Uint m_id; // A distinct idenifier for all controllers.
};

#endif