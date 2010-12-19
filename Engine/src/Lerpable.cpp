/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Lerpable.h"

namespace nt {

/**************************
 * Constructor
 *************************/
Lerpable::Lerpable( float posX, float posY, float lastPosX, float lastPosY )
  : m_logicPos( posX, posY, 0 ), m_lastPos( lastPosX, lastPosY, 0 ) {
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

  FloatPoint current = m_logicPos * alpha;

  float diff = 1.0 - alpha;
  FloatPoint last = m_lastPos * diff;

  SetPosition( current.x + last.x, current.y + last.y );
}


void Lerpable::Unlerp() {
  SetPosition( m_logicPos.x, m_logicPos.y );
  m_lastPos = m_logicPos;
}

} // namespace nt
