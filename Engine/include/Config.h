#ifndef CONFIG_H
#define CONFIG_H

/**
 * Class Config. Defines some of the basic properties of the game such
 * as the FPS rate and the screen size. This is being phased out in
 * preference of a more generic system that is specified in XML.
 */

class Config {
 public:
  /// Constructor
  Config();
  /// Destructor
  ~Config();

  /// Loads Config data from Config file.
  static void Load();

  /// Sets m_screenWidth to screenWidth
  static void SetScreenWidth( int screenWidth );
  
  /// Returns m_screenWidth
  static int GetScreenWidth();
  
  /// Sets m_screenHeight to screenHeight
  static void SetScreenHeight( int screenHeight );

  /// Returns m_screenHeight
  static int GetScreenHeight();

  /// Sets m_FPS to FPS
  static void SetFPS( int FPS );

  /// Returns m_FPS
  static int GetFPS();

private:

  static int m_screenWidth;
  static int m_screenHeight;
  static int m_FPS;
};

#endif

