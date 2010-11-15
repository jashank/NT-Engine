#ifndef NAME_PATH_H
#define NAME_PATH_H

#include <string>

/**
 * Can be used for storing name/file path combinations. Overloads some
 * comparison functionality for ease of use.
 */
struct NamePath {
  NamePath() {}
  NamePath( const std::string &name, const std::string &path )
    : name( name ), path( path ) {}

  std::string name;
  std::string path;

  /**
   * Compare paths since these should always be unique.
   */
  bool operator<( const NamePath &other ) const {
    return path < other.path && name < other.name;
  }
};

#endif // NAME_PATH_H

