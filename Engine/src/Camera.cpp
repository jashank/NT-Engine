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
      m_distance.x = = m_distance.y = 0.0;
    } else {
      m_view.Offset( m_distance.x / tileSize, m_distance.y / tileSize );
      nt::window::OffsetView( viewOffX, viewOffY );
    }
  }
}


nt::core::IntRect Camera::GetAdjustedFocus( int x, int y ) {
  int topLeftX = m_view.topLeft.x - x;
  int topLeftY = m_view.topLeft.y - y;
  int bottomRightX = m_view.bottomRight.x + x;
  int bottomRightY = m_view.bottomRight.y + y;

  if ( topLeftX < 0 ) {
    topLeftX = 0;
  }
  if ( topLeftY < 0 ) {
    topLeftY = 0;
  }

  int farTileX = nt::state::GetMapWidth() - 1;
  int farTileY = nt::state::GetMapHeight() - 1;

  if ( bottomRightX > farTileX ) {
    bottomRightX = farTileX;
  }
  if ( bottomRightY > farTileY ) {
    bottomRightY = farTileY;
  }

  nt::core::IntRect modRect( topLeftX, topLeftY, bottomRightX, bottomRightY );
  return modRect;
}

/*****************************
 * Lua Functions
 ****************************/
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
    LogLuaErr( "Number not passed to SetCamSpeed." );
    return 0;
  }
  m_speed = lua_tonumber( L, -1 );
  return 0;
}


int Camera::SpeedUp( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SpeedUpCam." );
    return 0;
  }
  m_speed += lua_tonumber( L, -1 );
  return 0;
}


int Camera::SlowDown( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to SpeedUpCam." );
    return 0;
  }
  m_speed -= lua_tonumber( L, -1 );
  return 0;
}


/***************************
 * Private Member Functions
 **************************/
void Camera::SetOffset( int x, int y ) {
  nt::core::IntRect destRect( m_view );
  destRect.Offset( x, y );

  int farTileX = nt::state::GetMapWidth() - 1;
  int farTileY = nt::state::GetMapHeight() - 1;
  int adjX = 0;
  int adjY = 0;

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
    
  int tileSize = nt::state::GetTileSize();
  m_offset.x = adjX * tileSize;
  m_offset.y = adjY * tileSize;
}

