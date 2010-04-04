#ifndef TIMER_H
#define TIMER_H

#include <SFML/Clock.hpp>

/********************************
Timer
- A standard timer that adds some 
  functionality onto SFML's Clock
********************************/
class Timer {
 public:
  // Standard timer. Starts at 0 and runs forward.
  Timer();

  // Timer starts at time passed, running backwards.
  Timer( float time );
  
  // Resets the timer
  void Reset();

  // Returns time elapsed (in seconds) since last reset
  float GetElapsedTime() const;

  // Adds time onto the timer
  void AddTime( float time );

  // Subtracts time from the timer
  void SubtractTime( float time );

 private:
  bool forward; // Whether timer runs forward
  float m_elapsedTime; // Time elapsed for the timer. Can be modified.
  float m_startingTime; // Needed for backwards timer functionaltiy
  sf::Clock m_clock; // SFML's clock to be used for the timer
};

#endif