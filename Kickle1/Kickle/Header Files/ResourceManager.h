#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <map>
#include <utility>
#include <string>
#include <memory>
#include <algorithm>

#include "Utilities.h"

#if defined(DEBUG)|defined(_DEBUG)
#include<iostream>
#endif

/*
String Compare functor used for std::map
*/
struct strCmp {
	bool operator()( const std::string& s1, const std::string& s2 ) const {
		return s1.compare( s2 ) < 0;
	}
};

template< typename resource_t >
struct ResourceLoader {
	virtual void operator()( std::pair< const std::string, resource_t* >& resource );
	virtual resource_t* Load( const std::string& filename );
};



template< typename resource_t, typename loader_t=ResourceLoader<resource_t> >
class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	void Clear();
	resource_t& Load( const std::string& filename );

private:
	typedef std::map< const std::string, resource_t*, strCmp > map_t;
	
	//Can't be copied or assigned to
	ResourceManager( const ResourceManager& );
	ResourceManager& operator=( const ResourceManager& );


	loader_t	m_loader;
	map_t		m_resources;
};

#include "ResourceManager.hpp"

#endif
