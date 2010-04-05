#include "Timer.h"

/************************************************
Public Methods
************************************************/

Timer::Timer()
  : m_forward( true ),
    m_offset( 0.0f ),
    m_startingTime( 0.0f ) {}


Timer::Timer( float time, bool forward )
  : m_forward( forward ),
    m_offset( 0.0f ),
    m_startingTime( time ) {}


void Timer::Reset() {
  m_clock.Reset();
  m_offset = 0.0f;
}


void Timer::Reset( float time, bool forward ) {
  Reset();
  m_startingTime = time;
  m_forward = forward;
}


float Timer::GetElapsedTime() {
  if ( m_forward ) {
    return m_startingTime + m_clock.GetElapsedTime() + m_offset;
  } else {
    return m_startingTime - m_clock.GetElapsedTime() + m_offset;
  }
}


void Timer::AddTime( float time ) {
  m_offset += time;
}


void Timer::SubtractTime( float time ) {
  m_offset -= time;
}


