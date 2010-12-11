#ifndef NAMEPATHMANAGER
#define NAMEPATHMANAGER

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

class TiXmlElement;

namespace nt {

/**
 * Allows access to resources via names and paths associated with them.
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
  /** Maps path to resource. */
  typedef boost::unordered_map<std::string, boost::shared_ptr<T> > 
    pathMap_type;

  /** Maps name to path. */
  typedef boost::unordered_map<std::string, std::string> nameMap_type;

  /**
   * Returns value associated with path passed.
   * NULL boost::shared_ptr returned if none found.
   */
  boost::shared_ptr<T> GetPathVal( const std::string &path ) const;

  nameMap_type m_nameMap;
  pathMap_type m_pathMap;

  /** Sub element to search for in root element in LoadData. */
  std::string m_subElem;
};

} // namespace nt

#include "NamePathManager.hpp"

#endif // NAMEPATHMANAGER

