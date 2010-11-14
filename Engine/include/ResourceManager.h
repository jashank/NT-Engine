#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <algorithm>
#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

/**
 * String comparison functor used for std::map.
 */
struct strCmp {
  bool operator()( const std::string &s1, const std::string &s2 ) const {
    return s1.compare( s2 ) < 0;
  }
};


/**
 * Loads data into resources stored in ResourceManager. Template parameter is
 * the resource type, which will be the same as the resource type of the 
 * ResourceManager.
 */
template<typename resource_t>
struct ResourceLoader {
  /**
   * Loads data from filePath into resource passed. Returns true if operation
   * succeeded.
   */
  bool Load(
    const std::string& filePath, 
    boost::shared_ptr<resource_t> &rsrc 
  );
};


template<typename resource_t, typename loader_t=ResourceLoader< resource_t > >
class ResourceManager {
public:
  ResourceManager() {}

  /**
   * Clears all contents. Note that manager holds shared pointers, so
   * make sure everything else that may be using a resource is no longer
   * using it before destroying the ResourceManager.
   */
  ~ResourceManager();

  /**
   * Loads resource located at file passed into the ResourceManager. Will not
   * load duplicate resources into memory. Throws runtime_error exception 
   * if unable to load resource.
   * @param filePath path to file to load resource from.
   * @return Resource loaded in. If resource was already loaded, returns it.
   */
  const boost::shared_ptr<resource_t> &Load( const std::string &filePath );

  /**
   * Releases unused resources, meaning resources that aren't being held by
   * anything besides the manager.
   */
  void ReleaseUnused();

private:
  typedef boost::shared_ptr<resource_t> shared_rsrc;
  typedef std::map<const std::string, shared_rsrc, strCmp> map_t;
  
  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  ResourceManager( const ResourceManager& );
  ResourceManager& operator=( const ResourceManager& );
  //@}

  loader_t m_loader; /** ResourceManager's ResourceLoader. */
  map_t m_resources; /** Map containing 'file path/resource' pairs. */
};

#include "ResourceManager.hpp"
#endif

