#ifndef NAMEPATHMANAGER
#define NAMEPATHMANAGER

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

#include "NamePath.h"

class TiXmlElement;

namespace nt {

/**
 * Holds map of NamePath/shared_ptr<T> pairs which can be loaded in from a 
 * State file. Each value in the map can be accessed via its name or path
 * to it. 
 * T is assumed to be a resource (compiler will complain if not).
 */
template<class T>
class NamePathManager {
 public:
  /** 
   * Must specify sub-element to search for when loading data from element
   * of state file. e.g. <song> is the sub-element in <music>.
   */
  NamePathManager( const std::string &subElem );

  /**
   * Loads values in from root element of State XML file and searches root
   * for sub-element name currently set.
   * @param root element of file.
   */
  void LoadData( const TiXmlElement *root );

  /**
   * Clears out all resources and their associated NamePaths.
   */
  void Clear();

  /** Returns value matching name or path passed. */
  boost::shared_ptr<T> GetVal( const std::string &nameOrPath ) const;

 private:
  typedef std::map<const NamePath, boost::shared_ptr<T> > map_type;

  map_type m_map;

  /** Sub element to search for in root element in LoadData. */
  std::string m_subElem;
};

} // namespace nt

#include "NamePathManager.hpp"

#endif // NAMEPATHMANAGER

