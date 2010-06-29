#ifndef IMAGE_H
#define IMAGE_H

#include "GUI.h"

/*******************************
 * Image
 * A GUI, just an image
 * ****************************/
class Image : public GUI {
 public:
  // Constructor and destructor
  Image( const std::string &filepath, int x, int y );
  ~Image() {}

 private:
  // Restrict copy constructor and destructor
  Image( const Image& );
  Image& operator=( const Image& );
};

#endif
