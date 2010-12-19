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

#ifndef NT_RESOURCELIB_H
#define NT_RESOURCELIB_H

#include <string>

#include <boost/shared_ptr.hpp>

/**
 * nt::rsrc contains functions to load resources in the application.
 * Resources include sf::Image, sf::Font, sf::SoundBuffer, sf::Music, and
 * AnimData.   
 *
 * All of the functions ensure that duplicates are not loaded.
 */
namespace nt {
namespace rsrc {
  /**
   * Loads resource specified in template parameter from ResourceManager.
   * @param relPath path to file to load that is relative to location
   *        of engine.
   * @return Resource if the load was successful or it has already been
   *         loaded. Exception thrown by ResourceManager if load fails.
   */
  template<class T>
  const boost::shared_ptr<T> &LoadResource( const std::string &relPath );
  
  /**
   * Releases unused resources. An unused resource means that nothing
   * currently holds the resource besides the resource manager.
   */
  void ReleaseUnused();

} // namespace rsrc
} // namespace nt

#endif // NT_RESOURCELIB_H

