#ifndef NT_TIMEDKEY_H
#define NT_TIMEDKEY_H

#include <map>
#include <string>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

namespace nt {

/**
 * Key that keeps track of whether it is held down, and if so, how
 * long it has been held down.
 */
class TimedKey {
 public:
  /**
   * Constructor.
   * @param keyString string representation of key ("up", "down", etc.).
   * @param repeat True if key continues to return activated after a single
   * activation and still being held down.
   * @param delay amount of time in seconds key needs to be pressed before
   * it returns true for Activated.
   * Param is string representation of key. Like "up", "down", etc.
   */
  TimedKey( std::string &keyString, bool repeat, float delay );
  ~TimedKey() {}

  /**
   * Updates key, activating or deactivating it depending on current state.
   * IsActivated() should be called before the next Update(), or you may
   * miss a point in which the key was activated.
   */
  void Update();

  /**
   * Returns true if key was activated in the last Update.
   */
  bool IsActivated() const;

  /**
   * Returns whether key is currently held down.
   */
  bool IsDown() const;

 private:
  typedef std::map<std::string, sf::Key::Code> keyMap_type;

  /** Mapping of key strings that can't simply be casted to an sf Key Code. */
  static keyMap_type m_extraKeys;

  /** True if key is activated. */
  bool m_activated;

  /** True if key has already been activated this press with repeat off. */
  bool m_noRepeatActiv;

  /** True if key has been pressed and not yet depressed. */
  bool m_pressed;

  /** 
   * True if key will still returned activated after having already
   * returned it in a single press.
   */
  bool m_repeat;

  /** How long key should be held down before it is activated. */
  float m_delay;

  /** Allows key to time itself. */
  sf::Clock m_clock;

  /** SFML key code this key is associated with. */
  sf::Key::Code m_kCode;
};

} // namespace nt

#endif // NT_TIMEDKEY_H

