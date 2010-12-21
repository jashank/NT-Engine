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

#ifndef NAMEPATHMANAGER
#define NAMEPATHMANAGER

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

class TiXmlElement;

namespace nt {

/**
 * Allows access to resources via names and paths associated with them.
 * T is assumed to be a resource (compiler will complain if not).
 */
template<class T>
class NamePathManager {
 public:
  /** 
   * Must specify sub-element to search for when loading data from element
   * of state file. e.g. <song> is the sub-element in <music>.
   */
  explicit NamePathManager( const std::string &subElem );

  /**
   * Loads values in from root element of State XML file and searches root
   * for sub-element name currently set.
   * @param root element of file.
   */
  void LoadData( const TiXmlElement *root );

  /**
   * Clears out all resources and their associated NamePaths.
   */
  void Clear();

  /** Returns value matching name or path passed. */
  boost::shared_ptr<T> GetVal( const std::string &nameOrPath ) const;

 private:
  /** Maps path to resource. */
  typedef boost::unordered_map<std::string, boost::shared_ptr<T> > 
    pathMap_type;

  /** Maps name to path. */
  typedef boost::unordered_map<std::string, std::string> nameMap_type;

  /**
   * Returns value associated with path passed.
   * NULL boost::shared_ptr returned if none found.
   */
  boost::shared_ptr<T> GetPathVal( const std::string &path ) const;

  nameMap_type m_nameMap;
  pathMap_type m_pathMap;

  /** Sub element to search for in root element in LoadData. */
  std::string m_subElem;
};

} // namespace nt

#include "NamePathManager.hpp"

#endif // NAMEPATHMANAGER

