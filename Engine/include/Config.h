/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

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

