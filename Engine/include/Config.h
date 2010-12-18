#ifndef NT_CONFIG_H
#define NT_CONFIG_H

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
  static float GetFPS();

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
  static float m_FPS;
};

#endif // NT_CONFIG_H

