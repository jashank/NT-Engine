#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

#include <boost/make_shared.hpp>

#include "Utilities.h"

// Specialize load for sf::Music because it uses OpenFromFile rather than
// LoadFromFile in SFML.
template<>
bool ResourceLoader<sf::Music>::Load(
  const std::string &filepath,
  boost::shared_ptr<sf::Music> &rsrc 
) {
  if( !rsrc->OpenFromFile( filepath ) ) {
    LogErr( "Song in" + filepath + "failed to load." );
    return false;
  }
  return true;
}


template<typename resource_t>
bool ResourceLoader<resource_t>::Load( 
  const std::string &filePath,
  boost::shared_ptr<resource_t> &rsrc
) {
  if( !rsrc->LoadFromFile( filePath ) ) {
    LogErr( "Resource " + filePath + " not found." );
    return false;
  }
  return true;
}


template<typename resource_t, typename loader_t>
ResourceManager< resource_t, loader_t >::~ResourceManager() {
  m_resources.clear();
}


template<typename resource_t, typename loader_t>
const boost::shared_ptr<resource_t> 
  &ResourceManager<resource_t, loader_t>::Load(
  const std::string &filePath
) {

  //Check to see if resource has already been loaded,
  //if so then return a reference to that resource
  typename map_t::iterator result = m_resources.find( filePath );
  if( result != m_resources.end() ) {
    return result->second;
  }

  // Load resource and insert into map, then return it.
  boost::shared_ptr<resource_t> rsrc = boost::make_shared<resource_t>();

  if ( m_loader.Load( filePath, rsrc )) {
    std::pair<typename map_t::iterator, bool> ret;
    ret = m_resources.insert( std::make_pair( filePath, rsrc ));
    return ret.first->second;
  }

  throw std::runtime_error("Resource failed to load.");
}


template<typename resource_t, typename loader_t>
void ResourceManager<resource_t, loader_t>::ReleaseUnused() {
  for ( typename map_t::iterator rsrc = m_resources.begin(); 
        rsrc != m_resources.end(); ) {
    typename map_t::iterator erase_elem = rsrc++;

    if ( erase_elem->second.unique() ) {
      m_resources.erase( erase_elem );
    }
  }
}

