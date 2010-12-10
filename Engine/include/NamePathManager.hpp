#include "tinyxml.h"

#include "ResourceLib.h"
#include "Utilities.h"

namespace nt {

/******************************
 * Constructor and Destructor
 *****************************/
template<class T>
NamePathManager<T>::NamePathManager( const std::string &subElem )
  :m_subElem( subElem ) {}

/*********************
 * Public Methods
 ********************/
template<class T>
void NamePathManager<T>::LoadData( const TiXmlElement *root ) {
  const TiXmlElement *subElem = root->FirstChildElement( m_subElem.c_str() );
  if ( subElem ) {
    do {
      const char *name = subElem->Attribute( "name" );
      const char *path = subElem->Attribute( "path" );

      if ( path ) {
        NamePath key;
        key.path = path;
        if ( name ) {
          key.name = name;
        }
        m_map.insert(
          typename map_type::value_type( 
            key, rsrc::LoadResource<T>( key.path )
          )
        );
      } else {
        LogErr( "Path not specified for element in State file." );
        break;
      }
    } while ( (subElem = subElem->NextSiblingElement( m_subElem.c_str() )) );
  }
}


template<class T>
void NamePathManager<T>::Clear() {
  m_map.clear();
}


template<class T>
boost::shared_ptr<T> NamePathManager<T>::GetVal( 
  const std::string &nameOrPath 
) const {
  const NamePath key( nameOrPath, nameOrPath );

  typename map_type::const_iterator itr = m_map.find( key );
  if ( itr != m_map.end() ) {
    return itr->second;
  }
  return boost::shared_ptr<T>();
}

} // namespace nt
