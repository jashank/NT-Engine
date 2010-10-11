#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <string>
#include <vector>

#include "Key.h"

/**
 * Registers keys for monitoring (keeping track of whether they are pressed
 * and how long they are held down).
 */
class KeyManager {
 public:
  KeyManager();

  /**
   * Takes string representation of key (ex: "up" == up arrow key), and puts
   * associated SFML key code in keyResult.
   * @param keyString string representation of key.
   * @param keyResult SFML key code to assign the found key code to.
   * @return False if there is no key code associated with key string passed. 
   */
  static bool InterpretKey( std::string keyString, sf::Key::Code &keyResult );

  /**
   * @param key SFML key code used to search for registered Key.
   * @return Registered Key corresponding to key code passed. Returns default
   * constructed Key if none found.
   */
  Key GetKey( sf::Key::Code key ) const;

  /**
   * Creates Key corresponding to SFML key code passed and registers it
   * for monitoring.
   * @param key SFML key code to associate Key created with.
   */
  void RegisterKey( sf::Key::Code key );

  /**
   * Refreshes data on all keys monitored (whether they have been pressed and
   * if so, how long they have been pressed).
   */
  void Update();

 private:
  //@{
  /** 
   * Restrict copy constructor and assignment.
   */
  KeyManager( const KeyManager &manager );
  KeyManager& operator=( const KeyManager &manager );
  //@}

  /**
   * SFML input monitoring used.
   */
  const sf::Input *m_input; 

  /**
   * Holds extra key strings for associating with key codes that SFML doesn't
   * string cast appropriately.
   */ 
  static const std::string m_extraKeys[]; 

  /**
   * Keys being monitored.
   */
  std::vector<Key> m_keys;
};

#endif // KEYMANAGER_H

