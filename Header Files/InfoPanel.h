#ifndef INFO_PANEL_H
#define INFO_PANEL_H

#include "Container.h"

// InfoPanel is a "custom container" that provides information and thumbnails.
// it is used in the Stage Selector for previewing levels.
class InfoPanel : public Container {
 public:
  InfoPanel();
  InfoPanel( float x, float y, float width, float height,
             sf::Color baseColor=sf::Color( 0,0,0,0 ), float outline=0.0f ,
             sf::Color outlineColor=sf::Color( 0, 0, 0, 0 ), 
             float padding=0.0f );
  virtual ~InfoPanel();

  virtual void Render();
  virtual void Update();
  virtual void HandleEvents();

  void SetFont( sf::Font font );
  void SetThumbnail( std::string imagePath );
  void SetInfo( std::string info );

  void SetPadding( float padding );
 private:
  sf::String m_info;
  float m_padding;
};

#endif // INFO_PANEL_H