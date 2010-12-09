#include "Camera.h"

#include <cstdlib>

#include "Utilities.h"
#include "Window.h"

extern "C" {
  #include "lua.h"
}

namespace nt {

/******************************
 * Constructors and Destructors
 ******************************/
Camera::Camera( const IntRect &mapRect, int tileSize )
  :m_mapRect( mapRect ),
   m_tileSize( tileSize ),
   m_moving( false ), 
   m_speed( -1.0 ) {
  Span( mapRect.GetWidth() + 1, mapRect.GetHeight() + 1 );
}


Camera::~Camera() { 
  window::ResetView();
}

/*****************************
 * Public Member Functions
 ****************************/
void Camera::Update( float dt ) {
  if ( m_moving ) {
    // Camera should move instantly
    if ( m_speed < 0.0 ) {
      m_view.Offset( m_offset.x / m_tileSize, m_offset.y / m_tileSize );
      window::OffsetView( m_offset.x, m_offset.y );

      m_offset.x = m_offset.y = 0;
      m_moving = false;
      return;
    }

    float movePx = m_speed * dt;

    int absOffX = abs( m_offset.x );
    int absOffY = abs( m_offset.y );
    float viewOffX = 0.0;
    float viewOffY = 0.0;

    if ( m_distance.x < absOffX ) {
      m_distance.x += movePx;
      if ( m_distance.x < absOffX ) {
        viewOffX = ( m_offset.x > 0 ) ? movePx : movePx * -1;
      } else {
        float adjust = m_distance.x - absOffX;
        viewOffX = ( m_offset.x > 0 ) ? adjust : adjust * -1;
      }
    }

    if ( m_distance.y < absOffY ) {
      m_distance.y += movePx;
      if ( m_distance.y < absOffY ) {
        viewOffY = ( m_offset.y > 0 ) ? movePx : movePx * -1;
      } else {
        float adjust = m_distance.y - absOffY;
        viewOffY = ( m_offset.y > 0 ) ? adjust : adjust * -1;
      }
    }

    if ( viewOffX == 0.0 && viewOffY == 0.0 ) {
      m_moving = false;
      m_offset.x = m_offset.y = 0;
      m_distance.x = m_distance.y = 0.0;
    } else {
      m_view.Offset( m_distance.x / m_tileSize, m_distance.y / m_tileSize );
      window::OffsetView( viewOffX, viewOffY );
    }
  }
}


IntRect Camera::GetAdjustedFocus( int x, int y ) const {
  int topLeftX = m_view.topLeft.x - x;
  int topLeftY = m_view.topLeft.y - y;
  int bottomRightX = m_view.bottomRight.x + x;
  int bottomRightY = m_view.bottomRight.y + y;

  IntRect rect( topLeftX, topLeftY, bottomRightX, bottomRightY );
  FitRect( m_mapRect, rect );
  return rect;
}


void Camera::Span( int xSpan, int ySpan ) {
  // Adjust span b/c tiles start at 0
  xSpan -= 1;
  ySpan -= 1;

  m_view.Scale( xSpan, ySpan );
  FitRect( m_mapRect, m_view );
}

/*****************************
 * Lua Functions
 ****************************/
int Camera::LuaSpan( lua_State *L ) {
  if ( m_moving ) {
    return 0;
  }
  if ( !lua_isnumber( L, -2 )) {
    LogLuaErr( "Number not passed to width parameter in SpanCam." );
    return 0;
  }
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to height parameter in SpanCam." );
    return 0;
  }

  int width = lua_tointeger( L, -2 );
  int height = lua_tointeger( L, -1 );
  Span( width, height );
  return 0;
}


int Camera::LuaOffset( lua_State *L ) {
  if ( m_moving ) {
    return 0;
  }
  if ( !lua_isnumber( L, -2 )) {
    LogLuaErr( "Number not passed to tile x parameter in MoveCam." );
    return 0;
  }
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to tile y parameter in MoveCam." );
    return 0;
  }

  int xOff = lua_tointeger( L, -2 );
  int yOff = lua_tointeger( L, -1 );

  SetOffset( xOff, yOff );

  m_moving = true;
  return 0;
}


int Camera::LuaSetCenter( lua_State *L ) {
  if ( m_moving ) {
    return 0;
  }
  if ( !lua_isnumber( L, -2 )) {
    LogLuaErr( "Number not passed to tile x parameter in SetCamCenter." );
    return 0;
  }
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to tile y parameter in SetCamCenter." );
    return 0;
  }

  int centerX = lua_tointeger( L, -2 );
  int centerY = lua_tointeger( L, -1 );

  int currCenterX = ( m_view.topLeft.x + m_view.bottomRight.x ) / 2;
  int currCenterY = ( m_view.topLeft.y + m_view.bottomRight.y ) / 2;

  int diffX = centerX - currCenterX;
  int diffY = centerY - currCenterY;

  SetOffset( diffX, diffY );

  m_moving = true;
  return 0;
}


int Camera::LuaSetSpeed( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to Camera.SetSpeed." );
    return 0;
  }
  m_speed = lua_tonumber( L, -1 );
  return 0;
}


int Camera::LuaAdjustSpeed( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to Camera.AdjustSpeed." );
    return 0;
  }
  m_speed += lua_tonumber( L, -1 );
  return 0;
}

/***************************
 * Private Member Functions
 **************************/
void Camera::SetOffset( int x, int y ) {
  IntRect destRect( m_view );
  destRect.Offset( x, y );

  int farTileX = m_mapRect.GetWidth();
  int farTileY = m_mapRect.GetHeight();
  int adjX = x;
  int adjY = y;

  if ( destRect.topLeft.x < 0 ) {
    adjX = -m_view.topLeft.x;
  } else if ( destRect.bottomRight.x > farTileX ) {
    adjX = farTileX - m_view.bottomRight.x;
  }

  if ( destRect.topLeft.y < 0 ) {
    adjY = -m_view.topLeft.y;
  } else if ( destRect.bottomRight.y > farTileY ) {
    adjY = farTileY - m_view.bottomRight.y;
  }
    
  m_offset.x = adjX * m_tileSize;
  m_offset.y = adjY * m_tileSize;
}

} // namespace nt

