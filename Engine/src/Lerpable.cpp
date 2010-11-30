#include "Lerpable.h"

namespace nt {
namespace graphics {

/**************************
 * Constructor
 *************************/
Lerpable::Lerpable( float posX, float posY, float lastPosX, float lastPosY )
  : m_logicPos( posX, posY ), m_lastPos( lastPosX, lastPosY ) {
  SetPosition( posX, posY );
}

/***************************
 * Public Methods
 **************************/
void Lerpable::SetStartingPos( float x, float y ) {
  SetPosition( x, y );
  m_logicPos.x = x;
  m_logicPos.y = y;
  m_lastPos = m_logicPos;
}


void Lerpable::Lerp( float alpha ) {
  m_logicPos.x = GetPosition().x;
  m_logicPos.y = GetPosition().y;

  nt::core::FloatVec current = m_logicPos * alpha;

  float diff = 1.0 - alpha;
  nt::core::FloatVec last = m_lastPos * diff;

  SetPosition( current.x + last.x, current.y + last.y );
}


void Lerpable::Unlerp() {
  SetPosition( m_logicPos.x, m_logicPos.y );
  m_lastPos = m_logicPos;
}

} // graphics
} // nt
