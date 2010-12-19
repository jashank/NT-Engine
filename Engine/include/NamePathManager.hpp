/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

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
        m_pathMap.insert(
          typename pathMap_type::value_type(
            path, rsrc::LoadResource<T>( path )
          )
        );
        if ( name ) {
          m_nameMap.insert(
            typename nameMap_type::value_type(
              name, path
            )
          );
        }
      } else {
        LogErr( "Path not specified for element in State file." );
        break;
      }
    } while ( (subElem = subElem->NextSiblingElement( m_subElem.c_str() )) );
  }
}


template<class T>
void NamePathManager<T>::Clear() {
  m_nameMap.clear();
  m_pathMap.clear();
}


template<class T>
boost::shared_ptr<T> NamePathManager<T>::GetVal( 
  const std::string &nameOrPath 
) const {
  typename nameMap_type::const_iterator itr = m_nameMap.find( nameOrPath );

  if ( itr != m_nameMap.end() ) {
    return GetPathVal( itr->second );
  } else {
    return GetPathVal( nameOrPath );
  }
}


/*********************
 Private Methods
 ********************/
template<class T>
boost::shared_ptr<T> NamePathManager<T>::GetPathVal( 
  const std::string &path 
) const {
  typename pathMap_type::const_iterator itr = m_pathMap.find( path );

  if ( itr != m_pathMap.end() ) {
    return itr->second;
  }
  return boost::shared_ptr<T>();
}

} // namespace nt
