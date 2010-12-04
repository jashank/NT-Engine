#ifndef NAMEPATHMANAGER
#define NAMEPATHMANAGER

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

#include "NamePath.h"

class TiXmlElement;

/**
 * Holds map of NamePath/shared_ptr<T> pairs which can be loaded in from a 
 * State file. Each value in the map can be accessed via its name or path
 * to it. 
 * This is an abstract class, and T is assumed to be a resource (compiler
 * will complain if not).
 */
template<class T>
class NamePathManager {
 public:
  /** 
   * Must specify sub-element to search for when loading data from element
   * of state file. e.g. <song> is the sub-element in <music>.
   */
  NamePathManager( const std::string &subElem );
  virtual ~NamePathManager() = 0;

  /**
   * Loads values in from root element of State XML file and searches root
   * for sub-element name currently set.
   * @param root element of file.
   * @return True if no errors in loading.
   */
  bool LoadData( const TiXmlElement *root );

 protected:
  typedef std::map<const NamePath, boost::shared_ptr<T> > map_type;

  /** Returns value matching name or path passed. */
  boost::shared_ptr<T> GetVal( const std::string &nameOrPath ) const;

 private:
  map_type m_map;

  /** Sub element to search for in root element in LoadData. */
  std::string m_subElem;
};

#include "NamePathManager.hpp"

#endif // NAMEPATHMANAGER

