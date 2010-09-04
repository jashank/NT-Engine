#ifndef MANAGEDKEY_H
#define MANAGEDKEY_H

#include <SFML/Window/Event.hpp>

/**
 * Key managed by app's KeyManager. Holds its SFML key code, the time
 * it was first pressed, and how long it has been held down.
 */
class Key {
 public:
  Key();
  
  /**
   * @param k SFML key code to initialize this Key's key code to.
   */
  Key( sf::Key::Code k );
 
  /**
   * Compares SFML key code.
   * @param rhs key on right hand side of < to compare to.
   * @return True if this key is less than rhs key.
   */
  bool operator<( const Key &rhs );

  float elapsedTime; /** Time (in seconds) key has been held down. */
  float startTime; /** Time when key was first pressed. */
  sf::Key::Code key; /** SFML key code to monitor for this key. */
};

#endif // MANAGEDKEY_H

