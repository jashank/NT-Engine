#include "Camera.h"

#include <cmath>

#include "StateComm.h"
#include "Utilities.h"
#include "Window.h"

extern "C" {
  #include "lualib.h"
}

/******************************
 * Constructors and Destructors
 ******************************/
Camera::Camera()
  :m_moving( false ), m_speed( 0.0 ) {}

/*****************************
 * Public Member Functions
 ****************************/
void Camera::Update( float dt ) {
  if ( m_moving ) {
    int tileSize = nt::state::GetTileSize();

    // Camera should move instantly
    if ( m_speed < 0.0 ) {
      m_view.Offset( m_offset.x / tileSize, m_offset.y / tileSize );
      nt::window::OffsetView( m_offset.x, m_offset.y );

      m_offset.x = 0;
      m_offset.y = 0;
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
      m_offset.x = 0;
      m_offset.y = 0;
    } else {
      m_view.Offset( m_distance.x / tileSize, m_distance.y / tileSize );
      nt::window::OffsetView( viewOffX, viewOffY );
    }
  }
}


const nt::core::IntRect &Camera::GetFocus() {
}

/*****************************
 * Lua Functions
 ****************************/
int LuaOffset( lua_State *L ) {
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

  int tileSize = nt::state::GetTileSize();
  m_offset.x = xOff * tileSize;
  m_offset.y = yOff * tileSize;

  m_moving = true;
  return 0;
}


int LuaSetCenter( lua_State *L ) {
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

  int tileSize = nt::state::GetTileSize();
  m_offset.x = diffX * tileSize;
  m_offset.y = diffY * tileSize;

  m_moving = true;
  return 0;
}


