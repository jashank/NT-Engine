#include "ResourceLib.h"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>

#include "AnimData.h"
#include "ResourceManager.h"

// All of the resources the engine holds
namespace {
  ResourceManager<sf::Image> images;
  ResourceManager<sf::Font> fonts;
  ResourceManager<sf::SoundBuffer> sounds;
  ResourceManager<sf::Music> music;
  ResourceManager<AnimData> anims;
}


namespace nt {
namespace rsrc {

  sf::Image *LoadImg( const std::string &relPath ) {
    return images.Load( relPath );
  }

  sf::Font *LoadFont( const std::string &relPath ) {
    return fonts.Load( relPath );
  }

  sf::SoundBuffer *LoadSound( const std::string &relPath ) {
    return sounds.Load( relPath );
  }

  sf::Music *LoadMusic( const std::string &relPath ) {
    return music.Load( relPath );
  }

  AnimData *LoadAnim( const std::string &relPath ) {
    return anims.Load( relPath );
  }

  void ClearImgs() {
    images.Clear();
  }

  void ClearFonts() {
    fonts.Clear();
  }

  void ClearSounds() {
    sounds.Clear();
  }

  void ClearMusic() {
    music.Clear();
  }

  void ClearAnims() {
    anims.Clear();
  }

  void ClearAll() {
    images.Clear();
    fonts.Clear();
    sounds.Clear();
    music.Clear();
    anims.Clear();
  }

} // namespace rsrc 
} // namespace nt
  
