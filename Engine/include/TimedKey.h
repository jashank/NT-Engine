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
  bool IsActivated();

 private:
  typedef sf::Key::Code sfk;
  typedef std::map<std::string, sfk> KeyMap;

  /** Mapping of key strings that can't simply be casted to an sf Key Code. */
  static const KeyMap m_extraKeys;

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
  sfk m_kCode;
};

#endif // TIMEDKEY_H

