#ifndef MANAGEDKEY_H
#define MANAGEDKEY_H

#include <SFML/Graphics.hpp>

struct Key {
 public:
  Key();
  Key( sf::Key::Code k );

  sf::Key::Code key;
  float elapsedTime;
  float startTime;
};
#endif

