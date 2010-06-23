#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>

#include <vector>

#include "Key.h"

class KeyManager {
 public:
  KeyManager();

  // Takes key string, interprets it, and sets keyResult as the appropriate
  // key code
  static bool InterpretKey( std::string &keyString, sf::Key::Code &keyResult );

  // Returns monitored Key corresponding to sf::Key::Code:: key.
  // Returns default constructed Key if none found.
  Key GetKey( sf::Key::Code key ) const;

  void Init( const sf::Input &input );
  void RegisterKey( sf::Key::Code key );
  void Update();
 private:
  //Contains the keys that are to be monitored
  std::vector< Key > m_keys;

  const sf::Input *m_input; //Used to access keystates

  static const std::string m_extraKeys[];
};

#endif
