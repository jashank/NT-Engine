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


template<typename T>
bool ResourceLoader<T>::Load( 
  const std::string &filePath,
  boost::shared_ptr<T> &rsrc
) {
  if( !rsrc->LoadFromFile( filePath ) ) {
    LogErr( "Resource " + filePath + " not found." );
    return false;
  }
  return true;
}


template<typename T, typename Loader>
ResourceManager<T, Loader>::~ResourceManager() {
  m_resources.clear();
}


template<typename T, typename Loader>
const boost::shared_ptr<T> 
  &ResourceManager<T, Loader>::Load(
  const std::string &filePath
) {

  //Check to see if resource has already been loaded,
  //if so then return a reference to that resource
  typename map_type::iterator result = m_resources.find( filePath );
  if( result != m_resources.end() ) {
    return result->second;
  }

  // Load resource and insert into map, then return it.
  boost::shared_ptr<T> rsrc = boost::make_shared<T>();

  if ( m_loader.Load( filePath, rsrc )) {
    std::pair<typename map_type::iterator, bool> ret;
    ret = m_resources.insert( 
      typename map_type::value_type( filePath, rsrc )
    );
    return ret.first->second;
  }

  throw std::runtime_error("Resource failed to load.");
}


template<typename T, typename Loader>
void ResourceManager<T, Loader>::ReleaseUnused() {
  for ( typename map_type::iterator rsrc = m_resources.begin(); 
        rsrc != m_resources.end(); ) {
    typename map_type::iterator erase_elem = rsrc++;

    if ( erase_elem->second.unique() ) {
      m_resources.erase( erase_elem );
    }
  }
}

} // namespace nt
