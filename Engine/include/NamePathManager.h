#ifndef NAMEPATHMANAGER
#define NAMEPATHMANAGER

#include "NamePath.h"
/**
 * Holds map of NamePath/T pairs which can be loaded in from a State
 * file. Each value in the map can be accessed via its name or path
 * to it. Assumes T is a 
 */
template<typename T>
class NamePathManager {
 public:
  NamePathManager() {}

 private:
  typedef std::map<const NamePath, T> map_type;
  
};

#include "NamePathManager.hpp"

#endif // NAMEPATHMANAGER
