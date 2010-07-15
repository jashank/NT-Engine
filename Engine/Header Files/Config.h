#ifndef CONFIG_H
#define CONFIG_H

class Config {
 public:
  Config();
  ~Config();

  // Loads Config data from Config file.
  static void Load();

  static void SetScreenWidth( int screenWidth );
  static int GetScreenWidth();

  static void SetScreenHeight( int screenHeight );
  static int GetScreenHeight();

  static void SetFPS( int FPS );
  static int GetFPS();

private:

  static int m_screenWidth;
  static int m_screenHeight;
  static int m_FPS;
};

#endif

