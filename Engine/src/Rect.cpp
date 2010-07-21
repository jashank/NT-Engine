#include "Rect.h"

namespace nt {
namespace core {

float Rect::GetWidth() const {
  return (bottomRight.x - topLeft.x);
}

float Rect::GetHeight() const {
  return (bottomRight.y - topLeft.y);
}
nt::core::Vector<float> Rect::GetSize() const {
  return nt::core::Vector<float>( GetWidth(), GetHeight() );
}

float Rect::GetArea() const {
  return ( GetWidth()*GetHeight() );
}

nt::core::Vector<float> Rect::GetCenter() const {
  return nt::core::Vector<float>(GetWidth()/2, GetHeight()/2);
}

bool Rect::Contains( const nt::core::Vector<float> pos ) const {
  return ( topLeft.x <= pos.x &&
           topLeft.y <= pos.y &&
           bottomRight.x >= pos.x &&
           bottomRight.y >= pos.y );
}
  
bool Rect::Intersects( const Rect &other ) const {
  return ( topLeft.x < other.bottomRight.x &&
           topLeft.y < other.bottomRight.y &&
           bottomRight.x > other.topLeft.x &&
           bottomRight.y > other.topLeft.x );
}
  
void Rect::SetCenter( nt::core::Vector<float> centerPosition ) {
  topLeft.x = centerPosition.x - GetWidth()/2;
  topLeft.y = centerPosition.y - GetHeight()/2;
  bottomRight.x = centerPosition.x + GetWidth()/2;
  bottomRight.y = centerPosition.y + GetHeight()/2;
}
  
void Rect::SetPosition( nt::core::Vector<float> position ) {
  float width = GetWidth();
  float height = GetHeight();
  topLeft = position;
  bottomRight.x = topLeft.x + width;
  bottomRight.y = topLeft.y + height;
}
  
void Rect::SetScale( nt::core::Vector<float> scale ) {
  bottomRight.x = topLeft.x + scale.x;
  bottomRight.y = topLeft.y + scale.y;
}

/*
Rect Rect::operator+( const Vector<float> &pos ) const {
}
  
Rect Rect::operator-( const Vector<float> &pos ) const {
}

Rect Rect::operator+=( const Vector<float> &pos ) const {
}

Rect Rect::operator-=( const Vector<float> &pos ) const {
}

bool operator==( const Rect &other ) const {
}

bool operator!=( const Rect &other ) const {
}

bool operator<( const Rect &other ) const {
}

bool operator>( const Rect &other ) const {
}

bool operator<=( const Rect &other ) const {
}

bool operator>=( const Rect &other ) const {
}
*/

} // namespace core
} // namespace nt
