#ifndef NAME_PATH_H
#define NAME_PATH_H

#include <string>

namespace nt {

/**
 * Can be used for storing name/file path combinations. Overloads comparison
 * functionality to allow for comparing for equality via path or name. For
 * example, if one wanted to find a NamePath pair of the name 'foo' in a map
 * they could search for a NamePath with name 'foo' and path 'foo', and the
 * map would return any NamePath with either the name 'foo' or path 'foo'.
 */
struct NamePath {
  NamePath() {}
  NamePath( const std::string &name, const std::string &path )
    : name( name ), path( path ) {}

  std::string name;
  std::string path;

  /**
   * If either path or name are equal then NamePaths are equal.
   */
  bool operator<( const NamePath &other ) const {
    return path < other.path && name < other.name;
  }
};

} // namespace nt

#endif // NAME_PATH_H

