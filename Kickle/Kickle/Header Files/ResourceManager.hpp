#include <memory>
#include <sstream>

template< typename resource_t >
void ResourceLoader< resource_t >::operator()( 
  std::pair< const std::string, resource_t* > &resource ) {
  DEBUG_STATEMENT( 
    std::cout 
    << "Deleting Resource...\n-->Filename: " << resource.first 
    << "\n-->Address: " << resource.second 
    << std::endl; 
  )
  SAFEDELETE(resource.second);
}

template< typename resource_t >
resource_t* ResourceLoader< resource_t >::Load( const std::string &filename ) {
  std::auto_ptr<resource_t> resource( new resource_t );
  if( !resource->LoadFromFile( filename ) ) {
    LogErr( "Resource " + filename + " not found." );
  }
  
  return resource.release();
}


template< typename resource_t, typename loader_t >
ResourceManager< resource_t, loader_t >::ResourceManager()	{ 
}

template< typename resource_t, typename loader_t >
ResourceManager< resource_t, loader_t >::~ResourceManager()	{ 
  this->Clear(); 
}

template< typename resource_t, typename loader_t >
void ResourceManager<resource_t, loader_t>::Clear() {
  std::for_each<map_t::iterator, loader_t>( 
    m_resources.begin(), 
    m_resources.end(), 
    m_loader );
  m_resources.clear();
}

template< typename resource_t, typename loader_t >
resource_t* ResourceManager< resource_t, loader_t >::Load( 
  const std::string &filename ) {
  DEBUG_STATEMENT( 
    LogMsg( "Loading Resource...\n-->Filename: " + filename + "\n" );
  )

  //Check to see if resource has already been loaded, 
  //if so then return a reference to that resource
  map_t::iterator result = m_resources.find( filename );
  if( result != m_resources.end() ) {
    DEBUG_STATEMENT(
      std::ostringstream ss;
      ss << "-->File already loaded\n-->Address: " << result->second << "\n";
      LogMsg( ss );
    )
    return result->second;
  }

  //Load the resource with loader functor and temporarily store in autoptr
  std::auto_ptr<resource_t> resource( m_loader.Load( filename ) );

  //Insert it into map and return a reference to that resource.
  DEBUG_STATEMENT(
    std::ostringstream ss;
    ss << filename << " --> Address: " << resource.get() << "\n";
    LogMsg( ss );
  )
  m_resources.insert( std::make_pair( filename, resource.get() ) );

  return resource.release();
}