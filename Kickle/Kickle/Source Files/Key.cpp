#include "Key.h"

Key::Key()
 : key( sf::Key::Count ),
   elapsedTime( 0.0f ),
   startTime( 0.0f ) {
}

Key::Key( sf::Key::Code k )
 : key( k ),
   elapsedTime( 0.0f ),
   startTime( 0.0f ) {
}