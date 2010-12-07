#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

#include <boost/make_shared.hpp>

#include "Utilities.h"

namespace nt {

// Specialize load for sf::Music because it uses OpenFromFile rather than
// LoadFromFile in SFML.
template<>
bool ResourceLoader<sf::Music>::Load(
  const std::string &filepath,
  boost::shared_ptr<sf::Music> &rsrc 
) {
  if( !rsrc->OpenFromFile( filepath ) ) {
    LogErr( "Song in" + filepath + " failed to load." );
    return false;
  }
  return true;
}


template<typename Resource>
bool ResourceLoader<Resource>::Load( 
  const std::string &filePath,
  boost::shared_ptr<Resource> &rsrc
) {
  if( !rsrc->LoadFromFile( filePath ) ) {
    LogErr( "Resource " + filePath + " not found." );
    return false;
  }
  return true;
}


template<typename Resource, typename Loader>
ResourceManager< Resource, Loader >::~ResourceManager() {
  m_resources.clear();
}


template<typename Resource, typename Loader>
const boost::shared_ptr<Resource> 
  &ResourceManager<Resource, Loader>::Load(
  const std::string &filePath
) {

  //Check to see if resource has already been loaded,
  //if so then return a reference to that resource
  typename map_type::iterator result = m_resources.find( filePath );
  if( result != m_resources.end() ) {
    return result->second;
  }

  // Load resource and insert into map, then return it.
  boost::shared_ptr<Resource> rsrc = boost::make_shared<Resource>();

  if ( m_loader.Load( filePath, rsrc )) {
    std::pair<typename map_type::iterator, bool> ret;
    ret = m_resources.insert( std::make_pair( filePath, rsrc ));
    return ret.first->second;
  }

  throw std::runtime_error("Resource failed to load.");
}


template<typename Resource, typename Loader>
void ResourceManager<Resource, Loader>::ReleaseUnused() {
  for ( typename map_type::iterator rsrc = m_resources.begin(); 
        rsrc != m_resources.end(); ) {
    typename map_type::iterator erase_elem = rsrc++;

    if ( erase_elem->second.unique() ) {
      m_resources.erase( erase_elem );
    }
  }
}

} // namespace nt
