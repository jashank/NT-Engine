#ifndef TIMEDKEY_H
#define TIMEDKEY_H

#include <map>
#include <string>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

/**
 * Key that keeps track of whether it is held down, and if so, how
 * long it has been held down.
 */
class TimedKey {
 public:
  /**
   * String representation of key.
   */
  TimedKey( std::string &keyString );

 private:
  typedef sf::Key::Code sfk;
  typedef std::map<std::string, sfk> KeyMap;
  /** Mapping of key strings that can't simply be casted to an sf Key Code. */
  static const KeyMap m_extraKeys;

  /** Allows key to time itself. */
  sf::Clock m_clock;

  /** SFML key code this key is associated with. */
  sfk kCode;
};

#endif // TIMEDKEY_H

