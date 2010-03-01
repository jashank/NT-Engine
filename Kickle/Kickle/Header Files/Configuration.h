#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "BasicTypeDefs.h"

class Configuration {
public:
  Configuration();
  virtual ~Configuration();

  void SetScreenWidth( unsigned int screenWidth );
  unsigned int GetScreenWidth() const;

  void SetScreenHeight( unsigned int screenHeight );
  unsigned int GetScreenHeight() const;

  void SetFPS( unsigned int FPS );
  unsigned int GetFPS() const;

  void SetMapSize( unsigned int mapSize );
  unsigned int GetMapSize() const;

  void SetTileSize( unsigned int tileSize );
  unsigned int GetTileSize() const;

  void SetXPad( unsigned int xPad );
  unsigned int GetXPad() const;

  void SetYPad( unsigned int yPad );
  unsigned int GetYPad() const;


private:
  unsigned int m_screenWidth;
  unsigned int m_screenHeight;
  unsigned int m_FPS;
  unsigned int m_mapSize;
  unsigned int m_tileSize;
  unsigned int m_xPad;
  unsigned int m_yPad;
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

