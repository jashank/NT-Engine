#include "TimedString.h"

using namespace nt::core;

/********************************
 * Constructors and Destructors
 *******************************/
TimedString::TimedString()
  :m_printTime( 0.f ) {}

/********************************
 * Public Methods
 *******************************/
void TimedString::BufferText( const std::string &text ) {
  m_buffer = text;
}


void TimedString::SetPrintTime( float time ) {
  m_printTime = time;
}


void TimedString::StartPrint() {
  if ( m_printTime == 0.f ) {
    sf::String::SetText( m_buffer );
    m_buffer.clear();
    return;
  }

  m_timer.Reset();
}


void TimedString::UpdatePrint() {
  if ( !m_buffer.empty() && m_timer.GetElapsedTime() > m_printTime ) {
    std::string newText = sf::String::GetText();
    newText.append( m_buffer.substr( 0, 1 ));
    sf::String::SetText( newText );

    m_buffer.erase( 0, 1 );
    m_timer.Reset();
  }
}

