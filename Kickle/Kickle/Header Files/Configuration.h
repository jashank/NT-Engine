#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "BasicTypeDefs.h"

class Configuration {
public:
  Configuration();
  virtual ~Configuration();

  void SetScreenWidth( Uint screenWidth );
  Uint GetScreenWidth() const;

  void SetScreenHeight( Uint screenHeight );
  Uint GetScreenHeight() const;

  void SetFPS( Uint FPS );
  Uint GetFPS() const;

  void SetMapSize( Uint mapSize );
  Uint GetMapSize() const;

  void SetTileSize( Uint tileSize );
  Uint GetTileSize() const;

  void SetXPad( Uint xPad );
  Uint GetXPad() const;

  void SetYPad( Uint yPad );
  Uint GetYPad() const;

  // Returns MAP_SIZE*TILE_SIZE
  Uint GetMapPixelSize();

  bool IsTileValid( Uint x, Uint y );

private:
  Uint m_screenWidth;
  Uint m_screenHeight;
  Uint m_FPS;
  Uint m_mapSize;
  Uint m_tileSize;
  Uint m_xPad;
  Uint m_yPad;
};



// Remove these instances from your code.
namespace Config {
  extern const Uint SCREEN_WIDTH;
  extern const Uint SCREEN_HEIGHT;
  extern const Uint FPS;
  extern const Uint MAP_SIZE;
  extern const Uint TILE_SIZE;
  extern const Uint X_PAD;
  extern const Uint Y_PAD;
}

#endif

