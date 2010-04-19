#ifndef TIMER_H
#define TIMER_H

#include <SFML/System/Clock.hpp>

/**************************************
Timer
- A timer that adds some functionality 
  onto SFML's Clock and that can be 
  rendered on screen.
**************************************/
class Timer {
 public:
  // Standard timer. Starts at 0 and runs forward.
  Timer();

  // Timer starts at time passed, running forward if 'forward' is true,
  // backwards otherwise.
  Timer( float time, bool forward );
  
  // Resets the timer to its starting point
  void Reset();

  // Resets timer to the specified parameters. See Timer constructor.
  void Reset( float time, bool forward );

  // Returns time currently on timer
  float GetTime();

  // Adds time onto the timer
  void AddTime( float time );

  // Subtracts time from the timer
  void SubtractTime( float time );

 private:
  bool m_forward; // Whether timer runs forward
  float m_offset; // Amount of time added and subtracted directly by user
  float m_startingTime; // Needed for backwards timer functionaltiy
  sf::Clock m_clock; // SFML's clock to be used for the timer
};

#endif