#ifndef CONFIG_H
#define CONFIG_H

/**
 * Retrieves information from game's config file (FPS, screen size, 
 * starting state) and holds it for application use.
 */
 class Config {
 public:
  /**
   * Loads data from config file.
   */
  static void Load();

  /**
   * @return Screen's width.
   */
  static int GetScreenWidth();
  
  /**
   * @return Screen's height.
   */
  static int GetScreenHeight();

  /**
   * @return Application's FPS.
   */
  static int GetFPS();

private:
  //@{
  /**
   * Restrict default constructor and destructor.
   */
  Config();
  ~Config();
  //@}

  static int m_screenWidth;
  static int m_screenHeight;
  static int m_FPS;
};

#endif // CONFIG_H

