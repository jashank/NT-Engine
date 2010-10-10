#include "StateMachine.h"

#include "State.h"
#include "Utilities.h"

/*******************************
 * Constructors and Destructors
 ******************************/
~StateMachine::StateMachine() {
  SAFEDELETE( m_runningState );
} 

/*******************************
 * Public Member Functions
 ******************************/
bool Setup( const std::string &filePath ) {
  m_runningState = new State();
  if ( !m_runningState->Init( filePath )) {
    SAFEDELETE( m_runningState );
    return false;
  }
  return true;
}


