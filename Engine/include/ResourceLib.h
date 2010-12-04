#ifndef RESOURCELIB_H
#define RESOURCELIB_H

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

#endif // RESOURCELIB_H

