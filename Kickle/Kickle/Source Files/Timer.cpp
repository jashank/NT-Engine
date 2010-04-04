#include "Timer.h"

/************************************************
Public Methods
************************************************/

Timer::Timer()
  : m_forward( true ),
    m_elapsedTime( 0.0f ),
    m_startingTime( 0.0f ) {}


Timer::Timer( float time )
  : m_forward( false ),
    m_elapsedTime( time ),
    m_startingTime( time ) {}


void Timer::Reset() {
  if ( m_forward ) {
    m_elapsedTime = 0.0f;
  } else {
    m_elapsedTime = m_startingTime;
  }
  m_clock.Reset();
}


float Timer::GetElapsedTime() {
  if ( m_forward ) {
    m_elapsedTime += m_clock.GetElapsedTime() - m_elapsedTime;
  } else {
    m_elapsedTime -= 
      m_clock.GetElapsedTime() - ( m_startingTime - m_elapsedTime );
  }
  return m_elapsedTime;
}


void Timer::AddTime( float time ) {
  m_elapsedTime += time;
}


void Timer::SubtractTime( float time ) {
  m_elapsedTime -= time;
}


