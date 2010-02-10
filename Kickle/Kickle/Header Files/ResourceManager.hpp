
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
		std::string err = "Unable to Load() resource: ";
		err += filename;
		throw err.c_str();
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
resource_t& ResourceManager< resource_t, loader_t >::Load( 
	const std::string &filename ) {
	DEBUG_STATEMENT( 
		std::cout 
		<< "Loading Resource...\n-->Filename: " << filename 
		<< std::endl; 
	)

	//Check to see if resource has already been loaded, 
	//if so then return a reference to that resource
	map_t::iterator result = m_resources.find( filename );
	if( result != m_resources.end() ) {
		DEBUG_STATEMENT( 
			std::cout 
			<< "-->File already loaded\n-->Address: " << result->second 
			<< std::endl; 
		)
		return *(result->second);
	}

	//Load the resource with loader functor and temporarily store in autoptr
	std::auto_ptr<resource_t> resource( m_loader.Load( filename ) );

	//Insert it into map and return a reference to that resource.
	DEBUG_STATEMENT( 
		std::cout << "-->Address: " << resource.get() << std::endl; 
	)
	m_resources.insert( std::make_pair( filename, resource.get() ) );

	return *(resource.release());
}