/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

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

#include "AnimData.h"

#include <sstream>
#include <stdexcept>

#include "ResourceLib.h"
#include "tinyxml.h"
#include "Utilities.h"

namespace nt {

/************************************************
Public Methods
************************************************/
bool AnimData::LoadFromFile( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );

  if ( doc.LoadFile() ) {
    TiXmlHandle handleDoc( &doc );

    TiXmlElement *root = handleDoc.FirstChildElement( "animations" ).Element();
    TiXmlElement *sheet = root->FirstChildElement( "sheet" );
    do {
      const boost::shared_ptr<sf::Image> &loadedSheet =
        rsrc::LoadResource<sf::Image>( sheet->Attribute( "path" ));
      if ( loadedSheet ) {
        // A strip
        TiXmlElement *elem = sheet->FirstChildElement();
        if ( elem ) {
          do {
            if ( !ParseStrip( elem, loadedSheet )) {
              return false;
            }
          } while ( (elem = elem->NextSiblingElement()) );
        }
      } else {
        LogErr( "Sheet in animation file: " + filepath + " not found." );
        return false;
      }
    } while ( (sheet = sheet->NextSiblingElement( "sheet" )) );
  } else {
    LogErr( "File not found: " + filepath );
    return false;
  }
  return true;
}


bool AnimData::IsLooped( int animIndex ) const {
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    return m_anims[animIndex].isLooped;
  } else {
    return false;
  }
}


float AnimData::GetFrameTime( int animIndex, int frameIndex ) const {
  static const Animation* a = NULL;
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    a = &m_anims[animIndex];
    if ( a->uniqueFrameTimes && frameIndex > 0 && frameIndex < a->numFrames ) {
      return a->frameTimes[frameIndex];
    } else {
      return a->frameTimes[0];
    }
  } else {
    return 0.f;
  }
}


IntPoint AnimData::GetAnimPosition( int animIndex ) const {
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    return IntPoint( 
             m_anims[animIndex].frameRect.topLeft.x,
             m_anims[animIndex].frameRect.topLeft.y,
             0
           );
  } else {
    return IntPoint( 0, 0, 0 );
  }
}


int AnimData::GetFrameWidth( int animIndex ) const {
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    return m_anims[animIndex].frameRect.GetWidth();
  } else {
    return 0;
  }
}


int AnimData::GetFrameHeight( int animIndex ) const {
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    return m_anims[animIndex].frameRect.GetHeight();
  } else {
    return 0;
  }
}


int AnimData::GetNumAnims() const {
  return m_anims.size();
}


int AnimData::GetNumFrames( int animIndex ) const {
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    return m_anims[animIndex].numFrames;
  } else {
    return 0;
  }
}


IntRect AnimData::GetFrameRect( 
  int animIndex, 
  int frameIndex 
) const {
  static const Animation *a = NULL;
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    a = &m_anims[animIndex];
    if ( frameIndex > 0 && frameIndex < a->numFrames ) {
      IntRect rect = a->frameRect;
      rect.topLeft.x += frameIndex * a->frameRect.GetWidth();
      rect.bottomRight.x = rect.topLeft.x + a->frameRect.GetWidth();
      return rect;
    } else {
      return a->frameRect;
    }
  } else {
    return IntRect( 0, 0, 0, 0 );
  }
}


const boost::shared_ptr<sf::Image> &AnimData::GetImage( 
  int animIndex
) const {
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    return m_anims[animIndex].image;
  }
  throw std::range_error("Invalid index into animation data.");
}

/************************************************
Private Methods
************************************************/
AnimData::Animation::Animation()
 :uniqueFrameTimes( false ),
  isLooped( false ),
  numFrames( 1 ) {}


bool AnimData::ParseStrip(
  const TiXmlElement *strip,
  const boost::shared_ptr<sf::Image> &sheet ) {
  Animation anim;

  const char *name = strip->Attribute( "name" );
  if ( name ) {
    anim.name = name;
  }

  const TiXmlElement *clip = strip->FirstChildElement( "clip" );
  if ( clip ) {
    clip->QueryIntAttribute( "x", &anim.frameRect.topLeft.x );
    clip->QueryIntAttribute( "y", &anim.frameRect.topLeft.y );
    clip->QueryIntAttribute( "width", &anim.frameRect.bottomRight.x );
    clip->QueryIntAttribute( "height", &anim.frameRect.bottomRight.y );
  }
  anim.frameRect.bottomRight.x += anim.frameRect.topLeft.x;
  anim.frameRect.bottomRight.y += anim.frameRect.topLeft.y;

  const TiXmlElement *flow = strip->FirstChildElement( "flow" );
  if ( flow ) {
    flow->QueryIntAttribute( "num_frames", &anim.numFrames );
    const char *looped = flow->Attribute( "looped" );
    if ( looped ) {
      anim.isLooped = ( ToLowerCase( looped ) == "true" );
    }
  }

  if ( anim.numFrames > 1 ) {
    const TiXmlElement *frameTimes = strip->FirstChildElement( "frame_times" );
    if ( frameTimes ) {
      std::stringstream ss;
      ss << frameTimes->GetText();
      float time = 0.0f;
      while ( !ss.eof() ) {
        ss >> time;
        anim.frameTimes.push_back( time );
      }
      if ( anim.frameTimes.size() > 1 ) {
        if ( static_cast<int>(anim.frameTimes.size()) == anim.numFrames ) {
          anim.uniqueFrameTimes = true;
        } else {
          LogErr( "When specifying multiple frame times, didn't specify time "
                  "for each frame. Req: 1 for all or 1 for each." );
          return false;
        }
      }
    } else {
      LogErr( "No frame time(s) specified in strip that has multiple frames." );
      return false;
    }
  } else {
    anim.frameTimes.push_back( 0.0f );
  }

  m_anims.push_back( anim );
  m_anims.back().image = sheet;
  return true;
}

} // nt
