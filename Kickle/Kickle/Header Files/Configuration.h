#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "BasicTypeDefs.h"

class Configuration {
 public:
  Configuration();
  virtual ~Configuration();

  // Loads the configuration data.
  static void Load();

  static void SetScreenWidth( Uint screenWidth );
  static Uint GetScreenWidth();

  static void SetScreenHeight( Uint screenHeight );
  static Uint GetScreenHeight();

  static void SetFPS( Uint FPS );
  static Uint GetFPS();

  static void SetMapSize( Uint mapSize );
  static Uint GetMapSize();

  static void SetTileSize( Uint tileSize );
  static Uint GetTileSize();

  static void SetXPad( Uint xPad );
  static Uint GetXPad();

  static void SetYPad( Uint yPad );
  static Uint GetYPad();

  // Returns MAP_SIZE*TILE_SIZE
  static Uint GetMapPixelSize();

  static bool IsTileValid( Uint x, Uint y );

private:
  static Uint m_screenWidth;
  static Uint m_screenHeight;
  static Uint m_FPS;
  static Uint m_mapSize;
  static Uint m_tileSize;
  static Uint m_xPad;
  static Uint m_yPad;
};

#endif

