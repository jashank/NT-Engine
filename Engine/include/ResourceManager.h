#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <algorithm>
#include <map>
#include <string>
#include <utility>

/**
 * String comparison functor used for std::map.
 */
struct strCmp {
  bool operator()( const std::string &s1, const std::string &s2 ) const {
    return s1.compare( s2 ) < 0;
  }
};


/**
 * Loads and deletes resources for the ResourceManager. Template parameter is
 * the resource type, which will be the same as the resource type of the 
 * ResourceManager.
 */
template< typename resource_t >
struct ResourceLoader {
  /**
   * Safely deletes resource passed.
   * @param resource pair storing resource's filename and the resource itself.
   */
  virtual void operator()( std::pair< const std::string, resource_t* > &resource );

  /**
   * Loads resource found at filePath, calling LoadFromFile with filePath
   * passed on a new resource_t object created.
   * @param filePath path to file to load resource from.
   * @return Pointer to the resource.
   */
  virtual resource_t* Load( const std::string& filePath );
};


template< typename resource_t, typename loader_t=ResourceLoader< resource_t > >
class ResourceManager {
public:
  ResourceManager() {}

  /**
   * Calls Clear().
   */
  ~ResourceManager();

  /**
   * Clears ResourceManager, safely deleting all contents from the application 
   * and freeing that memory for further use.
   */
  void Clear();

  /**
   * Loads resource located at file passed into the ResourceManager. Will not
   * load duplicate resources into memory.
   * @param filePath path to file to load resource from.
   * @return Resource loaded in. If resource was already loaded, returns it.
   * If resource failed to load, returns NULL.
   */
  resource_t* Load( const std::string& filePath );

private:
  typedef std::map< const std::string, resource_t*, strCmp > map_t;
  
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

