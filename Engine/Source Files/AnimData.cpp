#include "AnimData.h"

#include <sstream>

#include "App.h"
#include "tinyxml.h"
#include "Utilities.h"

/************************************************
Public Methods
************************************************/
bool AnimData::LoadFromFile( const std::string &filename ) {
  TiXmlDocument doc ( filename.c_str() );

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
              elem->QueryIntAttribute( "x", &m_common.frameRect.Left );
              elem->QueryIntAttribute( "y", &m_common.frameRect.Top );
              elem->QueryIntAttribute( "width", &m_common.frameRect.Right );
              elem->QueryIntAttribute( "height", &m_common.frameRect.Bottom );
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
                } while ( strip = strip->NextSiblingElement( "strip" ));
              }
            } else if ( strcmp( elem->Value(), "strip") == 0 ) {
              if ( !ParseStrip( elem, loadedSheet, OFF )) {
                return false;
              }
            }
          } while ( elem = elem->NextSiblingElement() );
        }
      } else {
        LogErr( "Sheet in animation file: " + filename + " not found." );
        return false;
      }
    } while ( sheet = sheet->NextSiblingElement( "sheet" ));
  } else {
    LogErr( "File not found: " + filename );
    return false;
  }
  return true;
}


bool AnimData::IsLooped( int animation ) const {
	return m_anims[animation%m_anims.size()].isLooped;
}


float AnimData::GetFrameTime( int animation, int frame ) const {
  static const Animation* a = 0;
  a = &m_anims[animation%m_anims.size()];

	if( a->uniqueFrameTimes ) {
		return a->frameTimes[frame%(a->numFrames)];
	}
	return a->frameTimes[0];
}


int AnimData::GetAnimX( int animation ) const {
	return m_anims[animation%m_anims.size()].frameRect.Left;
}


int AnimData::GetAnimY( int animation ) const {
	return m_anims[animation%m_anims.size()].frameRect.Top;
}


int AnimData::GetFrameWidth( int animation ) const {
	return m_anims[animation%m_anims.size()].frameRect.GetWidth();
}


int AnimData::GetFrameHeight( int animation ) const {
	return m_anims[animation%m_anims.size()].frameRect.GetHeight();
}


int AnimData::GetNumAnims() const {
	return m_anims.size();
}


int AnimData::GetNumFrames( int animation ) const {
	return m_anims[animation%m_anims.size()].numFrames;
}


sf::IntRect AnimData::GetFrameRect(
  int animation,
  int frame
) const {
	static sf::IntRect rect;
  static const Animation* a = 0;

  a = &m_anims[animation%m_anims.size()];
	rect.Top = a->frameRect.Top;
	rect.Left = a->frameRect.Left + (frame%a->numFrames) *
    a->frameRect.GetWidth();
	rect.Bottom = a->frameRect.Bottom;
	rect.Right = rect.Left + a->frameRect.GetWidth();

	return rect;
}


sf::Image& AnimData::GetImage( int animation ) const {
  return *(m_anims[animation%m_anims.size()].image);
}

/************************************************
Private Methods
************************************************/
AnimData::Animation::Animation()
 :uniqueFrameTimes( false ),
  isLooped( false ),
  numFrames( 0 ) {}

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
    clip->QueryIntAttribute( "x", &anim.frameRect.Left );
    clip->QueryIntAttribute( "y", &anim.frameRect.Top );
    clip->QueryIntAttribute( "width", &anim.frameRect.Right );
    clip->QueryIntAttribute( "height", &anim.frameRect.Bottom );
  }
  anim.frameRect.Right += anim.frameRect.Left;
  anim.frameRect.Bottom += anim.frameRect.Top;

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
        if ( anim.frameTimes.size() == anim.numFrames ) {
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

