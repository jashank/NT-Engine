#ifndef IMAGE_H
#define IMAGE_H

#include "GUI.h"

/*! \class Image
 * A GUI, just an image
 */
class Image : public GUI {
 public:
  /// Constructor
  Image( const std::string &filepath, int x, int y );
  /// Destructor
  ~Image() {}

 private:
  /// Restrict copy constructor and destructor
  Image( const Image& );
  Image& operator=( const Image& );
};

#endif
