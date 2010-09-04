#include "AnimData.h"

#include <sstream>

#include "App.h"
#include "Rect.h"
#include "tinyxml.h"
#include "Utilities.h"
#include "Vector.h"

/************************************************
Public Methods
************************************************/
bool AnimData::LoadFromFile( const std::string &filepath ) {
  TiXmlDocument doc ( filepath.c_str() );

  if ( doc.LoadFile() ) {
    static App *app = App::GetApp();
    TiXmlHandle handleDoc( &doc );

    TiXmlElement *sheet = handleDoc.FirstChildElement( "sheet" ).Element();
    do {
      sf::Image *loadedSheet = app->LoadImage( sheet->Attribute( "path" ));
      if ( loadedSheet ) {
        TiXmlElement *elem = sheet->FirstChildElement();
        if ( elem ) {
          do {
            if ( strcmp( elem->Value(), "common" ) == 0 ) {
              elem->QueryIntAttribute( "x", &m_common.frameRect.topLeft.x );
              elem->QueryIntAttribute( "y", &m_common.frameRect.topLeft.y );
              elem->QueryIntAttribute( "width", &m_common.frameRect.bottomRight.x );
              elem->QueryIntAttribute( "height", &m_common.frameRect.bottomRight.y );

              elem->QueryIntAttribute( "num_frames", &m_common.numFrames );
              const char *looped = elem->Attribute( "looped" );
              if ( looped ) {
                m_common.isLooped = ( strcmp( looped, "true" ) == 0 );
              }

              TiXmlElement *strip = elem->FirstChildElement( "strip" );
              if ( strip ) {
                do {
                  if ( !ParseStrip( strip, loadedSheet, ON )) {
                    return false;
                  }
                } while ( (strip = strip->NextSiblingElement( "strip" )));
              }
            } else if ( strcmp( elem->Value(), "strip") == 0 ) {
              if ( !ParseStrip( elem, loadedSheet, OFF )) {
                return false;
              }
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


nt::core::IntVec AnimData::GetAnimPosition( int animIndex ) const {
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    return nt::core::IntVec( 
             m_anims[animIndex].frameRect.topLeft.x,
             m_anims[animIndex].frameRect.topLeft.y
           );
  } else {
    return nt::core::IntVec( 0, 0 );
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


nt::core::IntRect AnimData::GetFrameRect( 
  int animIndex, 
  int frameIndex 
) const {
  static const Animation *a = NULL;
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    a = &m_anims[animIndex];
    if ( frameIndex > 0 && frameIndex < a->numFrames ) {
      nt::core::IntRect rect = a->frameRect;
      rect.topLeft.x += frameIndex * a->frameRect.GetWidth();
      rect.bottomRight.x = rect.topLeft.x + a->frameRect.GetWidth();
      return rect;
    } else {
      return a->frameRect;
    }
  } else {
    return nt::core::IntRect( 0, 0, 0, 0 );
  }
}


sf::Image* AnimData::GetImage( int animIndex ) const {
  if ( animIndex >= 0 && (unsigned int) animIndex < m_anims.size() ) {
    return m_anims[animIndex].image;
  } else {
    return NULL;
  }
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
  sf::Image *sheet,
  CommonTag flag
) {
  Animation anim;
  if ( flag == ON ) {
    anim = m_common;
  }
  anim.image = sheet;

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
  return true;
}

