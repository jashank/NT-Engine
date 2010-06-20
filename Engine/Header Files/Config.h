#ifndef CONFIG_H
#define CONFIG_H

class Config {
 public:
  Config();
  ~Config();

  // Loads Config data from Config file.
  static void Load();

  static void SetScreenWidth( unsigned int screenWidth );
  static unsigned int GetScreenWidth();

  static void SetScreenHeight( unsigned int screenHeight );
  static unsigned int GetScreenHeight();

  static void SetFPS( unsigned int FPS );
  static unsigned int GetFPS();

private:
  static unsigned int m_screenWidth;
  static unsigned int m_screenHeight;
  static unsigned int m_FPS;
};

#endif

