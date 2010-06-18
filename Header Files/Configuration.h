#ifndef CONFIGURATION_H
#define CONFIGURATION_H


class Configuration {
 public:
  Configuration();
  virtual ~Configuration();

  // Loads configuration data from configuration file.
  static void Load();

  static void SetScreenWidth( unsigned int screenWidth );
  static unsigned int GetScreenWidth();

  static void SetScreenHeight( unsigned int screenHeight );
  static unsigned int GetScreenHeight();

  static void SetFPS( unsigned int FPS );
  static unsigned int GetFPS();

  static void SetMapSize( unsigned int mapSize );
  static unsigned int GetMapSize();

  static void SetTileSize( unsigned int tileSize );
  static unsigned int GetTileSize();

  static void SetXPad( unsigned int xPad );
  static unsigned int GetXPad();

  static void SetYPad( unsigned int yPad );
  static unsigned int GetYPad();

  // Returns MAP_SIZE*TILE_SIZE
  static unsigned int GetMapPixelSize();

  static bool IsTileValid( unsigned int x, unsigned int y );

private:
  static unsigned int m_screenWidth;
  static unsigned int m_screenHeight;
  static unsigned int m_FPS;
  static unsigned int m_mapSize;
  static unsigned int m_tileSize;
  static unsigned int m_xPad;
  static unsigned int m_yPad;
};

#endif

