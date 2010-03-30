#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <vector>

class KeyManager {
 public:
  KeyManager();
  static bool InterpretKey( const std::string &keyString, sf::Key::Code &keyResult );

  float GetKeyTime( sf::Key::Code key ) const;
  void Init( const sf::Input &input );
  void RegisterKey( sf::Key::Code key );
  void Update();
 private:
  //Contains the keys that are to be monitored
  std::vector< std::pair< sf::Key::Code, float > > m_keys;

  const sf::Input *m_input; //Used to access keystates

  static const std::string m_extraKeys[];
};

#endif