#include "Key.h"

Key::Key()
 : elapsedTime( 0.f ),
   startTime( 0.f ),
   key( sf::Key::Count ) {}


Key::Key( sf::Key::Code k )
 : elapsedTime( 0.f ),
   startTime( 0.f ),
   key( k ) {}


bool Key::operator<( const Key &rhs ) {
  return key < rhs.key;
}

