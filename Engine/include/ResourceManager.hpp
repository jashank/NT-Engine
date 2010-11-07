#include <memory>
#include <sstream>

#include "Utilities.h"

template< typename resource_t >
void ResourceLoader< resource_t >::operator()(
  std::pair< const std::string, resource_t* > &resource ) {
  SAFEDELETE(resource.second);
}


// Specialize load for sf::Music because it uses OpenFromFile rather than
// LoadFromFile in SFML.
template<>
sf::Music* ResourceLoader< sf::Music >::Load( const std::string &filepath ) {
  std::auto_ptr< sf::Music > resource( new sf::Music() );
  if( !resource->OpenFromFile( filepath ) ) {
    LogErr( "Song in" + filepath + "failed to load." );
    return NULL;
  }
  return resource.release();
}


template< typename resource_t >
resource_t* ResourceLoader< resource_t >::Load( const std::string &filePath ) {
  std::auto_ptr<resource_t> resource( new resource_t );
  if( !resource->LoadFromFile( filePath ) ) {
    LogErr( "Resource " + filePath + " not found." );
  }

  return resource.release();
}


template< typename resource_t, typename loader_t >
ResourceManager< resource_t, loader_t >::~ResourceManager() {
  this->Clear();
}


template< typename resource_t, typename loader_t >
void ResourceManager<resource_t, loader_t>::Clear() {
  std::for_each<typename map_t::iterator, loader_t>(
    m_resources.begin(),
    m_resources.end(),
    m_loader );
  m_resources.clear();
}


template< typename resource_t, typename loader_t >
resource_t* ResourceManager< resource_t, loader_t >::Load(
  const std::string &filePath ) {

  //Check to see if resource has already been loaded,
  //if so then return a reference to that resource
  typename map_t::iterator result = m_resources.find( filePath );
  if( result != m_resources.end() ) {
    return result->second;
  }

  //Load the resource with loader functor and temporarily store in autoptr
  std::auto_ptr<resource_t> resource( m_loader.Load( filePath ) );

  //Insert it into map and return a reference to that resource.
  m_resources.insert( std::make_pair( filePath, resource.get() ) );

  return resource.release();
}
