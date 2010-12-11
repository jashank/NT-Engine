#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <algorithm>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

namespace nt {

/**
 * Loads data into resources stored in ResourceManager. Template parameter is
 * the resource type, which will be the same as the resource type of the 
 * ResourceManager.
 */
template<typename Resource>
struct ResourceLoader {
  /**
   * Loads data from filePath into resource passed. Returns true if operation
   * succeeded.
   */
  bool Load(
    const std::string& filePath, 
    boost::shared_ptr<Resource> &rsrc 
  );
};


template<typename Resource, typename Loader = ResourceLoader< Resource > > 
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
  const boost::shared_ptr<Resource> &Load( const std::string &filePath );

  /**
   * Releases unused resources, meaning resources that aren't being held by
   * anything besides the manager.
   */
  void ReleaseUnused();

private:
  typedef boost::shared_ptr<Resource> shared_rsrc;
  typedef boost::unordered_map<const std::string, shared_rsrc> map_type;
  
  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  ResourceManager( const ResourceManager& );
  ResourceManager& operator=( const ResourceManager& );
  //@}

  Loader m_loader; /** ResourceManager's ResourceLoader. */
  map_type m_resources; /** Map containing 'file path/resource' pairs. */
};

} // namespace nt

#include "ResourceManager.hpp"
#endif

