/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef NT_RESOURCEMANAGER_H
#define NT_RESOURCEMANAGER_H

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
template<typename T>
struct ResourceLoader {
  /**
   * Loads data from filePath into resource passed. Returns true if operation
   * succeeded.
   */
  bool Load(
    const std::string& filePath, 
    boost::shared_ptr<T> &rsrc 
  );
};


template<typename T, typename Loader = ResourceLoader<T> >
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
  const boost::shared_ptr<T> &Load( const std::string &filePath );

  /**
   * Releases unused resources, meaning resources that aren't being held by
   * anything besides the manager.
   */
  void ReleaseUnused();

private:
  typedef boost::shared_ptr<T> shared_rsrc;
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
#endif // NT_RESOURCEMANAGER_H

